#include "eval.h"

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
	if (!_var2bool(list)) {
		return list;
	}
	if (_quotep(list)) {
		return car(cdr(list));
	}
	/* eval contents */
	struct var *evaluated_list;
	struct var **tail = &evaluated_list;
	do {
		*tail = cons(eval(env, car(list)), nil());
		tail = &((*tail)->as.cons->y);
		list = cdr(list);
	} while (_var2bool(list));

	return evaluated_list;
}
