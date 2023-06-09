#include "repl.h"
#include "common.h"
#include "parser.h"
#include "eval.h"
#include "core.h"
#include "linenoise/linenoise.h"

extern tgc_t gc;
/* Replace manually allocated string with a garbage collected one */
static char *tgc_replace_str(tgc_t *gc, char *str);
static size_t count_unmatched_paren(const char *);
static bool only_whitespace(const char *);

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

size_t count_unmatched_paren(const char *input)
{
	size_t paren = 0;
	while (*input) {
		switch (*input) {
		case '(': paren++; break;
		case ')': paren--; break;
		default: break;
		}
		input++;
	}
	return paren;
}

bool only_whitespace(const char *input)
{
  next:
	switch(*input) {
	case ' ':
	case '\n':
	case '\t':
	case '\v':
	case '\r':
		input++;
		goto next;
	case '\0':
		return true;
	default:
		return false;
	}
}

void repl() {
	printf("Welcome to Moolisp v%d.%d.%d\
<https://github.com/RolandMarchand/moolisp>\n\
Copyright (c) 2023 Roland Marchand roland.marchand@protonmail.com\n\n",
	       VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
	char *input;
	linenoiseSetMultiLine(1);
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
	env_set(env, symbol("length"), c_function(curried_length));
	env_set(env, symbol("cons"), c_function(curried_cons));
	env_set(env, symbol("car"), c_function(curried_car));
	env_set(env, symbol("cdr"), c_function(curried_cdr));
	env_set(env, symbol("apply"), c_function(curried_apply));
	env_set(env, symbol("print"), c_function(curried_print));
	while ((input = linenoise(" > ")) != NULL) {
		input = tgc_replace_str(&gc, input);
		if (only_whitespace(input)) {
			continue;
		}
		while (count_unmatched_paren(input) > 0) {
			char *next_input = linenoise("");
			char *tmp = tgc_alloc(&gc, strlen(input)
					      + strlen(next_input)
					      + 2);
			strcpy(tmp, input);
			strcat(tmp, " ");
			strcat(tmp, next_input);
			input = tmp;
			free(next_input);
		}

		linenoiseHistoryAdd(input);
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
