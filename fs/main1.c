#include <stdio.h>
#include <regex.h>
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>

#define _DOT(c) ((c) == '.')
#define DOTS(s) ((s) && _DOT((s)[0]))
#define DOT1(s) (DOTS(s) && !((s)[1]))
#define DOT2(s) (DOTS(s) && _DOT((s)[1]) && !((s)[2]))

static int dirent_filter (const struct dirent *ent) {
	return (DOT1(ent->d_name) || DOT2(ent->d_name)) ? 0 : 1;


}

static int re_scandir (const char *root, int depth, typeof(int (const char *, int idx, struct stat *)) *hook) {
	int i, n;
	bool slash;
	struct stat s;
	struct dirent **namelist;
	char *p, *path = NULL;
	size_t pathsize, size;

	if (0 == depth) {
		if (hook) hook(root, depth, NULL);
		return 0;
	}

	slash = (root[strlen(root) - 1] != '/') ? true : false;
	if ((n = scandir(root, &namelist, dirent_filter, alphasort)) < 0) {
		perror(root);
		if (hook) hook(root, depth, NULL);
		return 0;
	}

	for (i = 0; i < n; i++) {
		/* */
		size = strlen(root) + strlen(namelist[i]->d_name) + (slash ? 2 : 1);
		if (!path) {
			path = malloc(size);
			pathsize = size;
			sprintf(path, "%s", root);
			p = path + strlen(path);
			if (slash) p[0] = '/';
		} else if (path && pathsize <= size) {
			path = realloc(path, size);
			pathsize = size;
		}
		p = path + strlen(root) + (slash ? 1 : 0);
		sprintf(p, "%s", namelist[i]->d_name);
		/* */
		if (lstat(path, &s) < 0) {
			printf("%s | error: %d - %s\n", path, errno, strerror(errno));
			continue;
		}
		if (S_ISLNK(s.st_mode)) {
			if (hook) hook(path, depth, &s);
			// printf("%s\n", path);
		} else if (S_ISDIR(s.st_mode)) {
			regex_t regex;
			size_t nmatch = 2;
			regmatch_t pmatch[2];

			regcomp(&regex, "^([0-9]*)$", REG_EXTENDED);
			if (0 == regexec(&regex, namelist[i]->d_name, nmatch, pmatch, 0)) {
				re_scandir(path, depth - 1, hook);
			}
			regfree(&regex);
		} else {
			if (hook) hook(path, depth, &s);
			// printf("%s\n", path);
		}
		/* */
		free(namelist[i]);
	}
	if (n == 0) {
		if (hook) hook(root, depth, NULL);
		// printf("%s/\n", root);
	}

	free(path);
	free(namelist);
	return 0;
}

int hook(const char *path, int idx, struct stat *s) {
	int id;
	const char *name;
	char *buf = strdup(path);
	if (idx != 1)
		return 0;

	name = basename(path);
	id = atoi(basename(dirname(buf)));
	free(buf);
	if (strcmp(name, "stat") != 0) {
		return 0;
	}
	printf("%-8d %s\n", id, name);
	// printf("path: %s\n", path);

	return 0;
}

int main (int agrc, char *argv[]) {
	const char *root = argv[1] ? argv[1] : "/proc";
	re_scandir(root, 2, hook);
	return 0;
}
