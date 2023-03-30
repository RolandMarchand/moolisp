#include "context.h"

extern tgc_t gc;

struct context get_context(int argc, char **argv)
{
#define RESET(c) do {					\
		memset(c, 0, sizeof(struct context));	\
	} while (false)

	struct context c;
	RESET(&c);

	if (argc == 1) {
		c.repl = true;
	}

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--version") == 0) {
			RESET(&c);
			c.version = true;
		} else if (strcmp(argv[i], "--license") == 0
			   || strcmp(argv[i], "-l") == 0) {
			RESET(&c);
			c.license = true;
		} else if (strcmp(argv[i], "--verbose") == 0
			   || strcmp(argv[i], "-v") == 0) {
			c.verbose = true;
		} else if (strcmp(argv[i], "--repl") == 0) {
			if (c.license || c.version) {
				continue;
			}
			c.repl = true;
		} else if (strcmp(argv[i], "-c") == 0) {
			i++;
			if (i == argc || *argv[i] == '-') {
				RESET(&c);
				c.error = CONTEXT_ERROR_EXPECTED_FILE;
				break;
			}
			c.source_count = 0;
			c.source = tgc_alloc(&gc, sizeof(char*));
			do {
				c.source_count++;
				c.source = tgc_realloc(&gc, c.source, c.source_count * sizeof(char*));
				c.source[c.source_count - 1] = argv[i];
				i++;
			} while (i != argc && *argv[i] != '-');
			i--;
			if (c.license || c.version) {
				c.source = NULL;
				c.source_count = 0;
			}
		} else {
			RESET(&c);
			c.error = CONTEXT_ERROR_UNKNOWN_ARG;
			c.error_arg = argv[i];
			break;
		}
	}
	return c;
#undef RESET
}
