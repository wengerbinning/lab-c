
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

#include "database.h"

static db_context_t * db_context_created (db_config_t *conf) {
	db_context_t *ctx;

	if (!conf)
		return NULL;

	ctx = malloc(sizeof(db_context_t));
	memset(ctx, 0, sizeof(db_context_t));

	if (conf->hostname)
		ctx->hostname = strdup(conf->hostname);
	if (conf->username)
		ctx->username = strdup(conf->username);
	if (conf->password)
		ctx->password = strdup(conf->password);
	if (conf->database)
		ctx->database = strdup(conf->database);

	ctx->port = conf->port;
	ctx->type = conf->type;
	return ctx;
}

static db_context_t * db_context_destroy (db_context_t *ctx) {
	if (!ctx)
		return NULL;

	if (ctx->hostname)
		free(ctx->hostname);
	if (ctx->username)
		free(ctx->username);
	if (ctx->password)
		free(ctx->password);
	if (ctx->database)
		free(ctx->database);

	free(ctx);
	return NULL;
}

db_context_t * db_init (db_config_t *conf) {
	db_context_t *ctx;

	if (!(ctx = db_context_created(conf)))
		return NULL;

	switch (ctx->type) {
	case Mariadb: {
		MYSQL *mysql;

		if (!(mysql = mysql_init(NULL))) {
			ctx = db_context_destroy(ctx);
			return NULL;
		}

		ctx->context = mysql;
		if (!ctx->hostname || !ctx->username || !ctx->password)
			return ctx;

		if (!(mysql_real_connect(ctx->context, ctx->hostname,
			ctx->username, ctx->password, ctx->database, ctx->port, NULL, 0))) {
			mysql_close(mysql);
			ctx = db_context_destroy(ctx);
			return NULL;
		}
		break;
	}
	default:
		ctx = db_context_destroy(ctx);
		return NULL;
	}

	return ctx;
}

db_context_t * db_exit (db_context_t *ctx) {
	if (!ctx)
		return NULL;

	switch (ctx->type) {
	case Mariadb:
		if (ctx->context)
			mysql_close(ctx->context);
		break;
	}

	db_context_destroy(ctx);
	return NULL;
}

/* */

int db_connect (db_context_t *ctx) {

}



int db_config_dump (db_config_t *conf) {
	return 0;
}

int db_create_database (db_context_t *ctx, const char *name) {
	char sql_cmd[SQL_CMD_MAXSIZE + 1];

	snprintf(sql_cmd, sizeof(sql_cmd), "CREATE DATABASE %s", name);
	if (mysql_query(ctx->context, sql_cmd)) {
		// fprintf(stderr, "CREATE DATABASE failed: %s\n", mysql_error(conn));
		return -1;
	}
	return 0;
}

int db_select_database (db_context_t *ctx, const char *name) {
	if (mysql_select_db(ctx->context, name) < 0) {
		return -1;
	}
	if (ctx->database)
		free(ctx->database);

	ctx->database = strdup(name);
	return 0;
}

int db_delete_database (db_context_t *ctx, const char *name) {
	return 0;
}

/* */

int db_dump (db_context_t *ctx) {

	return 0;
}

int db_table_dump (db_context_t *ctx) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int num_fields, i;
	// 3. 执行SQL查询
	if (mysql_query(conn, "SELECT * FROM your_table")) {
		// fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
		mysql_close(conn);
		return EXIT_FAILURE;
	}

	// 4. 获取并处理结果集
	res = mysql_store_result(conn);
	if (res == NULL) {
		// fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
		mysql_close(conn);
		return EXIT_FAILURE;
	}

	num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res))) {
		for (i = 0; i < num_fields; i++) {
			printf("%s\t", row[i] ? row[i] : "NULL");
		}
		printf("\n");
	}

    // 5. 释放资源并关闭连接
    mysql_free_result(res);

	return 0;
}
