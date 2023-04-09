#include "repl.h"
#include "config.h"
#include "parser.h"
#include "eval.h"
#include "core.c"

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
	struct env *env = env_make(NULL, nil(), nil());
	env_set(env, symbol("+"), c_function(add));
	env_set(env, symbol("-"), c_function(substract));
	env_set(env, symbol("*"), c_function(multiply));
	env_set(env, symbol("/"), c_function(divide));
	env_set(env, symbol("="), c_function(number_equal));
	env_set(env, symbol("<"), c_function(lesser_than));
	env_set(env, symbol(">"), c_function(greater_than));
	env_set(env, symbol("<="), c_function(lesser_equal));
	env_set(env, symbol(">="), c_function(greater_equal));
	env_set(env, symbol("eq"), c_function(eq));
	env_set(env, symbol("equal"), c_function(equal));
	env_set(env, symbol("length"), c_function(length));
	env_set(env, symbol("cons"), c_function(curried_cons));
	env_set(env, symbol("car"), c_function(curried_car));
	env_set(env, symbol("cdr"), c_function(curried_cdr));
	while ((input = readline("🐑) ")) != NULL) {
		input = tgc_replace_str(&gc, input);
		if (!input || !*input) {
			break;
		}
		add_history(input);
		struct var v = parse(input);
		struct var *tail = &v;
		while (_var2bool(tail)) {
			struct var *evaluated = eval(env, car(tail));
			print(evaluated);
			printf("\n");
			tail = cdr(tail);
		}
	}
	printf("Bye!\n");
}
