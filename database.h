#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <stdint.h>

#define SQL_CMD_MAXSIZE 1024

typedef enum db_type {
	Mariadb,
} db_type_t;

typedef struct db_config {
	uint32_t port;
	char *hostname;
	char *username;
	char *password;
	char *database;
	db_type_t type;
} db_config_t;

typedef struct db_context {
	void *context;
	uint32_t port;
	char *hostname;
	char *username;
	char *password;
	char *database;
	db_type_t type;
} db_context_t;

db_context_t * db_init (db_config_t *conf);
db_context_t * db_exit (db_context_t *ctx);

#define _STR(name) #name
#define  STR(name) _STR(name)


#define devel(fmt, ...) printf("[%s:%s:%d] " fmt "\n", \
	__FILE__, __func__, __LINE__, ##__VA_ARGS__);

#endif /* __DATABASE_H__ */
