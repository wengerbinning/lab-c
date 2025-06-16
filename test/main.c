#include <time.h>
#include <stdio.h>

#include "database.h"

int main (int agrc, char *argv[]) {
	db_context_t *ctx;
	db_config_t conf = {
		.hostname = "localhost",
		.port = 1001,
		.username = "wenger",
		.password = "wenger",

	};

	if (!(ctx = db_init(&conf))) {
		fprintf(stderr, "mysql_init() failed\n");
		return -1;
	}

	printf("已连接数据库服务 ...\n");
	sleep(5);

	ctx = db_exit(ctx);
	return 0;
}
