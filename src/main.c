#include "common.h"
#include "config.h"
#include "context.h"
#include "lexer.h"
#include "main.h"
#include "repl.h"
#include "var.h"

tgc_t gc;

struct var eval(struct var expr);
struct var progn(struct var list);
struct var unless(struct var test, struct var then, struct var otherwise);

int _main(int argc, char **argv);

int main(int argc, char **argv)
{
	volatile void *dummy;
	tgc_start(&gc, &dummy);
	int ret = _main(argc, argv);
	tgc_stop(&gc);
	return ret;
}

int _main(int argc, char **argv)
{
	struct context context = get_context(argc, argv);
	if (context.error) {
		if (context.error_arg) {
			fprintf(stderr, "error: %s '%s'\n",
				context.error, context.error_arg);
			return EXIT_FAILURE;
		}
		fprintf(stderr, "error: %s\n", context.error);
		return EXIT_FAILURE;
	}
	if (context.license) {
		printf(LICENSE);
		return EXIT_SUCCESS;
	}
	if (context.version) {
		printf("Moolisp version: %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
		return EXIT_SUCCESS;
	}
	if (context.repl) {
		repl();
	}
	return EXIT_SUCCESS;
}

struct var eval(struct var expr)
{

}

struct var progn(struct var list)
{
	assert(list.type == VAR_CONS);
	struct var ret;
	do {
		ret = eval(list.cons->x);
		list = list.cons->y;
	} while (!nil(list));
	return ret;
}

struct var unless(struct var test, struct var then, struct var otherwise)
{
	if (nil(test)) {
		return progn(then);
	} else {
		return progn(otherwise);
	}
}
