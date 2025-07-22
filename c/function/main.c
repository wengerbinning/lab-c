#include <stdio.h>
#include <stdbool.h>

#define MSGSIZE 1024



typedef union module {
	unsigned int value;
	struct {
		unsigned int system: 1;
		unsigned int network: 1;
		unsigned int firewall: 1;
		unsigned int mwan3: 1;
	};
} module_t;


typedef struct result {
	int id;
	bool enable;
	const char *name;
	module_t module;
	unsigned int state;
	char message[MSGSIZE + 1];
} result_t;

typedef struct handle {
	int action;
} handle_t;




int func (handle_t *handle, result_t *result, int a, int b) {
	result->module.firewall = 1;
	snprintf(result->message, MSGSIZE, "nothing");
	return 0;
}






int main (int agrc, char *argv[]) {
	handle_t handle = { 0 };
	result_t result = { 0 };
	int ret;

	ret = func(&handle, &result, 1, 2);

	printf("return code: %d - %s\n", ret, result.message);

	return 0;
}