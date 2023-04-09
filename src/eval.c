#include "eval.h"
#include "core.h"

static struct var *eval_list(struct env *env, struct var *list);

struct var *eval(struct env *env, struct var *ast)
{
	assert(env);
	assert(ast);

	if (!_var2bool(ast)) {
		return ast;
	}

	switch(ast->type) {
	case VAR_NUMBER:
	case VAR_STRING:
	case VAR_C_FUNCTION:
	case VAR_CLOSURE:
		return ast;
	case VAR_SYMBOL:
		return env_get(env, ast);
	case VAR_CONS:
		return eval_list(env, ast);
	default:
		fprintf(stderr, "error: unknown variant type '%d'\n", ast->type);
		exit(EXIT_FAILURE);
	}
}

struct var *eval_list(struct env *env, struct var *list)
{
	assert(env);
	assert(list);
	assert(_var2bool(listp(list)));

	/* special functions */
	if (!_var2bool(list)) {
		return list;
	}
	/* TODO: handle special cases better */
	if (_quotep(list)) {
		return car(cdr(list));
	}
	if (_var2bool(symbolp(car(list)))) {
		if (strcmp(car(list)->as.symbol, "lambda") == 0) {
			assert(length(cons(cdr(list), nil()))->as.number == 2);
			return closure(env, car(cdr(list)), car(cdr(cdr(list))));
		}
		if (strcmp(car(list)->as.symbol, "define") == 0) {
			assert(length(cons(cdr(list), nil()))->as.number == 2);
			assert(_var2bool(symbolp(car(cdr(list)))));
			env_set(env, car(cdr(list)), eval(env, car(cdr(cdr(list)))));
			return car(cdr(list));
		}
	}
	/* TODO: handle and, or, cond short-circuiting */

	/* eval contents */
	struct var *evaluated_list;
	struct var **tail = &evaluated_list;
	do {
		*tail = cons(eval(env, car(list)), nil());
		tail = &((*tail)->as.cons->y);
		list = cdr(list);
	} while (_var2bool(list));

	return apply(evaluated_list);
}
