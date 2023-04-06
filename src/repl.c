#include "repl.h"
#include "config.h"
#include "parser.h"

extern tgc_t gc;

/* Replace manually allocated string with a garbage collected one */
static char *tgc_replace_str(tgc_t *gc, char *str);

char *tgc_replace_str(tgc_t *gc, char *str)
{
	assert(gc);
	if (!str) {
		return NULL;
	}
	char *tmp = tgc_alloc(gc, strlen(str) + 1);
	strcpy(tmp, str);
	free(str);
	return tmp;
}

void repl() {
	printf("Welcome to Moolisp v%d.%d.%d\
<https://github.com/RolandMarchand/moolisp>\n\
Copyright (c) 2023 Roland Marchand roland.marchand@protonmail.com\n\n",
	       VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	rl_bind_key('\t', rl_insert);
	char *input;
	while ((input = readline("🐑) ")) != NULL) {
		input = tgc_replace_str(&gc, input);
		if (!input || !*input) {
			break;
		}
		add_history(input);
		struct var v = parse(input);
		dolist(&v, print);
	}
	printf("Bye!\n");
}

