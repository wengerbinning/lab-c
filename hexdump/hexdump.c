#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEXDUMP_COLUM	16
#define HEXDUMP_CHAR1	' '
#define HEXDUMP_ASCII	true
#define HEXDUMP_CHAR2	' '

#ifndef note
#define  note(fmt, ...) printf(fmt"\n", ## __VA_ARGS__)
#endif

static inline bool str2bool(const char *str) {
	if (!str)
		return false;

	if (0 == strcmp(str, "true") || 0 == strcmp(str, "True"))
		return true;

	if (0 == strcmp(str, "yes") || 0 == strcmp(str, "YES"))
		return true;

	if (1 == atoi(str))
		return true;

	return false;
}

static inline char printchar(char chr) {
	return (chr > 31 && chr < 127) ? chr : '.';
}

void hexdump (const void *data, unsigned int size) {
	int i;
	char *buf1, *p;
	char *buf2, *q;
	const char *env;
	char chr1, chr2;
	int col, oft;
	bool ascii __attribute__((unused));

	if (!data)
		return;

	env = getenv("HEXDUMP_COLUM");
	col = env ? atoi(env) : HEXDUMP_COLUM;
	env = getenv("HEXDUMP_CHAR1");
	chr1 = env ? *(char *)env : HEXDUMP_CHAR1;
	env = getenv("HEXDUMP_ASCII");
	ascii = env ? str2bool(env) : HEXDUMP_ASCII;
	env = getenv("HEXDUMP_CHAR2");
	chr2 = env ? *(char *)env : HEXDUMP_CHAR2;

	buf1 = malloc(col * (2 + 1));
	buf2 = malloc(col * (1 + 1));
	memset(buf1, 0, col * 3);
	memset(buf2, 0, col * 2);
	p = buf1;
	q = buf2;
	oft = 0;
	for (i = 0; i < size; i++) {
		sprintf(p, "%02X", *(unsigned char *)(data + i));
		sprintf(q, "%c", printchar(*(unsigned char *)(data + i)));
		p = p + 2; q++;

		if (((i + 1) % col)) {
			if (chr1) {
				sprintf(p, "%c", chr1); p++;
			}
			if (chr2) {
				sprintf(q, "%c", printchar(chr2)); q++;
			}
		}

		if (!((i + 1) % col)) {
			if (ascii)
				note("%08x %s | %s", oft, buf1, buf2);
			else
				note("%08x %s", oft, buf1);

			memset(buf1, 0, col * 3);
			memset(buf2, 0, col * 2);
			p = buf1; q = buf2;
			oft += col;
		}
	}

	if (size % col) {
		for (i = 0; i < (col - (size % col)); i++) {
			sprintf(p, "  "); p = p + 2;
			sprintf(q, "%c", printchar(' ')); q++;
			if ((i != (col - (size % col) - 1))) {
				sprintf(p, "%c", chr1);
				sprintf(q, "%c", chr2);
				p++; q++;
			}
		}

		if (ascii)
			note("%08x %s | %s", oft, buf1, buf2);
		else
			note("%08x %s", oft, buf1);
	}

	free(buf1);
	free(buf2);
}
