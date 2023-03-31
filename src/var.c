#include "var.h"

extern tgc_t gc;

struct var lambda(size_t param_cnt, char **param_names, struct var body)
{
	char **names = param_cnt
		? tgc_calloc(&gc, param_cnt, sizeof(char*))
		: NULL;
	for (size_t i = 0; i < param_cnt; i++) {
		names[i] = strdup(param_names[i]);
	}

	struct function *f = tgc_alloc(&gc, sizeof(struct function));
	f->param_cnt = param_cnt;
	f->param_names = names;
	f->body = body;

	return (struct var){
		.type = VAR_FUNCTION,
		.as.function = f
	};
}

struct var cons(struct var x, struct var y)
{
	struct cons *c = tgc_calloc(&gc, 1, sizeof(struct cons));
	c->x = x, c->y = y;
	return (struct var){
		.type = VAR_CONS,
		.as.cons = c
	};
}

struct var string(char *string)
{
	char *string_cpy = tgc_alloc(&gc, strlen(string) + 1);
	strcpy(string_cpy, string);
	return (struct var){
		.type = VAR_STRING,
		.as.string = string_cpy
	};
}

struct var symbol(char *symbol)
{
	char *symbol_cpy = tgc_alloc(&gc, strlen(symbol) + 1);
	strcpy(symbol_cpy, symbol);
	return (struct var){
		.type = VAR_SYMBOL,
		.as.symbol = symbol_cpy
	};
}

struct var *car(struct var *list)
{
	assert(list->type == VAR_CONS);
	return &list->as.cons->x;
}

struct var *cdr(struct var *list)
{
	assert(list->type == VAR_CONS);
	return &list->as.cons->y;
}

struct var number(double number)
{
	return (struct var){
		.type = VAR_NUMBER,
		.as.number = number
	};
}

struct var quote(struct var expr)
{
	return cons((struct var){.type = VAR_QUOTE}, expr);
}

struct var nil()
{
	return (struct var){};
}

bool nilp(struct var *v)
{
	return v->type == VAR_NIL;
}

bool eq(struct var *a, struct var *b)
{
	return a->type == b->type && a->as.cons == b->as.cons;
}

bool atom(struct var *v)
{
	return v->type != VAR_CONS;
}

