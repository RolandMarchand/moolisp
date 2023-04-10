#include "common.h"
#include "context.h"

tgc_t gc;

void _main(void);
static bool compare_source(size_t argc, char **s1, char **s2)
{
	if (!s1 || !s2) {
		return s1 == s2;
	}
	bool equal = true;
	for (size_t i = 0; equal && i < argc; i++) {
		equal &= strcmp(s1[i], s2[i]) == 0;
	}
	return equal;
}

static bool compare_strings(char *e1, char *e2)
{
	if (!e1 || !e2) {
		return e1 == e2;
	}
	return strcmp(e1, e2) == 0;
}

static bool compare_contexts(struct context *c1, struct context *c2)
{
	return c1->license == c2->license
		&& c1->repl == c2->repl
		&& c1->verbose == c2->verbose
		&& c1->version == c2->version
		&& compare_strings(c1->error, c2->error)
		&& compare_strings(c1->error_arg, c2->error_arg)
		&& c1->source_count == c2->source_count
		&& compare_source(c1->source_count, c1->source, c2->source);
}

int main(void)
{
	volatile void *dummy;
	tgc_start(&gc, &dummy);
	_main();
	tgc_stop(&gc);
}

void _main(void)
{
	char **argv = (char*[]){"test"};
	struct context c = get_context(1, argv);
	assert(compare_contexts(&c, &(struct context){
				.repl = true
			}));

	argv = (char*[]){"test", "-c"};
	c = get_context(2, argv);
	assert(compare_contexts(&c, &(struct context){
				.error = CONTEXT_ERROR_EXPECTED_FILE
			}));

	argv = (char*[]){"test", "-c", "/home/user/file.ml"};
	c = get_context(3, argv);
	assert(compare_contexts(&c, &(struct context){
				.source_count = 1,
				.source = (char*[]){
					"/home/user/file.ml"
				}
			}));

	argv = (char*[]){"test", "-c", "/home/user/file1.ml",
		"/home/user/file2.ml", "/home/user/file3.ml", "--repl"};
	c = get_context(6, argv);
	assert(compare_contexts(&c, &(struct context){
				.repl = true,
				.source_count = 3,
				.source = (char*[]){
					"/home/user/file1.ml",
					"/home/user/file2.ml",
					"/home/user/file3.ml"
				}
			}));

	argv = (char*[]){"test", "--verbose", "--repl", "-c",
		"/home/user/file1.ml", "/home/user/file2.ml",
		"/home/user/file3.ml"};
	c = get_context(7, argv);
	assert(compare_contexts(&c, &(struct context){
				.repl = true,
				.verbose = true,
				.source_count = 3,
				.source = (char*[]){
					"/home/user/file1.ml",
					"/home/user/file2.ml",
					"/home/user/file3.ml"
				}
			}));

	argv = (char*[]){"test", "--license"};
	c = get_context(2, argv);
	assert(compare_contexts(&c, &(struct context){
				.license = true
			}));

	argv = (char*[]){"test", "-v"};
	c = get_context(2, argv);
	assert(compare_contexts(&c, &(struct context){
				.verbose = true
			}));

	argv = (char*[]){"test", "--version"};
	c = get_context(2, argv);
	assert(compare_contexts(&c, &(struct context){
				.version = true
			}));

	argv = (char*[]){"test", "--verbose"};
	c = get_context(2, argv);
	assert(compare_contexts(&c, &(struct context){
				.verbose = true
			}));

	argv = (char*[]){"test", "--"};
	c = get_context(2, argv);
	assert(compare_contexts(&c, &(struct context){
				.error = CONTEXT_ERROR_UNKNOWN_ARG,
				.error_arg = "--"
			}));

	argv = (char*[]){"test", "--version", "--license", "error!"};
	c = get_context(4, argv);
	assert(compare_contexts(&c, &(struct context){
				.error = CONTEXT_ERROR_UNKNOWN_ARG,
				.error_arg = "error!"
			}));

	argv = (char*[]){"test", "--version", "--license", "--verbose",
		"--version", "--license"};
	c = get_context(6, argv);
	assert(compare_contexts(&c, &(struct context){
				.license = true
			}));

	argv = (char*[]){"test", "-c", "--license", "--verbose",
		"--version", "--license"};
	c = get_context(6, argv);
	assert(compare_contexts(&c, &(struct context){
				.error = CONTEXT_ERROR_EXPECTED_FILE
			}));

}
