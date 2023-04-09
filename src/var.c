#include "var.h"

extern tgc_t gc;

static struct var *create_var(const struct var *v);

bool _var2bool(const struct var *v)
{
	assert(v);
	return memcmp(v, &(struct var){}, sizeof(struct var)) != 0;
}

struct var *create_var(const struct var *v)
{
	assert(v);
	struct var *ret = tgc_alloc(&gc, sizeof(struct var));
	*ret = *v;
	return ret;
}

struct var *c_function(struct var *(*f)(const struct var *))
{
	assert(f);
	return create_var(&(struct var){
			.type = VAR_C_FUNCTION,
			.as.c_function = f
		});
}

struct var *closure(struct env *env, struct var *params, struct var *body)
{
	assert(env);
	assert(params);
	assert(body);
	struct closure *c = tgc_alloc(&gc, sizeof(struct closure));
	c->env = env;
	c->params = params;
	c->body = body;
	return create_var(&(struct var){
			.type = VAR_CLOSURE,
			.as.closure = c
		});
}

struct var *cons(struct var *x, struct var *y)
{
	assert(x);
	assert(y);
	struct cons *c = tgc_calloc(&gc, 1, sizeof(struct cons));
	c->x = x, c->y = y;
	return create_var(&(struct var){
			.type = VAR_CONS,
			.as.cons = c
		});
}

struct var *string(const char *string)
{
	assert(string);
	char *string_cpy = tgc_alloc(&gc, strlen(string) + 1);
	strcpy(string_cpy, string);
	return create_var(&(struct var){
			.type = VAR_STRING,
			.as.string = string_cpy
		});
}

struct var *symbol(const char *symbol)
{
	assert(symbol);
	char *symbol_cpy = tgc_alloc(&gc, strlen(symbol) + 1);
	strcpy(symbol_cpy, symbol);
	return create_var(&(struct var){
			.type = VAR_SYMBOL,
			.as.symbol = symbol_cpy
		});
}

struct var *car(const struct var *list)
{
	assert(list);
	if (!_var2bool(list)) {
		return (struct var *)list;
	}
	assert(list->type == VAR_CONS);
	return list->as.cons->x;
}

struct var *cdr(const struct var *list)
{
	assert(list);
	if (!_var2bool(list)) {
		return (struct var *)list;
	}
	assert(list->type == VAR_CONS);
	return list->as.cons->y;
}

struct var *number(double number)
{
	return create_var(&(struct var){
			.type = VAR_NUMBER,
			.as.number = number
		});
}

struct var *quote(struct var *expr)
{
	assert(expr);
	return cons(symbol("quote"), cons(expr, nil()));
}

struct var *nil()
{
	return create_var(&(struct var){});
}

struct var *nilp(const struct var *v)
{
	assert(v);
	if (_var2bool(v)) {
		return nil();
	}
	return t();
}

struct var *listp(const struct var *v)
{
	assert(v);
	if (v->type == VAR_CONS) {
		return t();
	}
	return nil();
}

struct var *not(const struct var *v)
{
	assert(v);
	if (_var2bool(nilp(v))) {
		return t();
	}
	return nil();
}

struct var *atom(const struct var *a)
{
	if (a->type != VAR_CONS) {
		return t();
	}
	return nilp(a);
}

struct var *stringp(const struct var *s)
{
	if (s->type == VAR_STRING) {
		return t();
	}
	return nil();
}

struct var *numberp(const struct var *n)
{
	assert(n);
	if (n->type == VAR_NUMBER) {
		return t();
	}
	return nil();
}

struct var *symbolp(const struct var *s)
{
	assert(s);
	if (s->type == VAR_SYMBOL) {
		return t();
	}
	return nil();
}

struct var *c_functionp(const struct var *f)
{
	assert(f);
	if (f->type == VAR_C_FUNCTION) {
		return t();
	}
	return nil();
}

struct var *closurep(const struct var *c)
{
	assert(c);
	if (c->type == VAR_CLOSURE) {
		return t();
	}
	return nil();
}

struct var *functionp(const struct var *f)
{
	assert(f);
	if (f->type == VAR_C_FUNCTION || f->type == VAR_CLOSURE) {
		return t();
	}
	return nil();
}

bool _quotep(const struct var *v)
{
	assert(v);
	if (v->type != VAR_CONS || car(v)->type != VAR_SYMBOL
	    || strcmp(car(v)->as.symbol, "quote") != 0) {
		return false;
	}
	return true;
}
