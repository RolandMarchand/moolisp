#include "var.h"

struct var lambda(size_t param_cnt, char **param_names, struct var body)
{
	char **names = param_cnt ? calloc(param_cnt, sizeof(char*)) : NULL;
	for (unsigned int i = 0; i < param_cnt; i++) {
		names[i] = strdup(param_names[i]);
	}

	struct function *f = malloc(sizeof(struct function));
	f->param_cnt = param_cnt;
	f->param_names = names;
	f->body = body;

	return (struct var){
		.type = VAR_FUNCTION,
		.function = f
	};
}

struct var cons(struct var x, struct var y)
{
	struct cons *c = calloc(1, sizeof(struct cons));
	c->x = x, c->y = y;
	return (struct var){
		.type = VAR_CONS,
		.cons = c
	};
}

struct var create_string(char *string)
{
	return (struct var){
		.type = VAR_STRING,
		.string = strdup(string)
	};
}

/* TODO implement symbol */
void free_var(struct var* v)
{
	switch(v->type) {
	case VAR_INT:
	case VAR_NIL:
		break;
	case VAR_SYMBOL:
		/* to implement */
		break;
	case VAR_STRING:
		free(v->string);
		break;
	case VAR_CONS:
		free_var(&v->cons->x);
		free_var(&v->cons->y);
		free(v->cons);
		break;
	case VAR_FUNCTION: {
		struct function *func = v->function;
		free_var(&func->body);
		for (unsigned int i = 0; i < func->param_cnt; i++) {
			free(func->param_names[i]);
		}
		if (func->param_cnt) {
			free(func->param_names);
		}
		free(func);
	}
	}
	v->type = VAR_NIL;
}

struct var car(struct var list)
{
	assert(list.type == VAR_CONS);
	return list.cons->x;
}

struct var cdr(struct var list)
{
	assert(list.type == VAR_CONS);
	return list.cons->y;
}

bool atom(struct var v)
{
	return v.type != VAR_CONS;
}

bool eq(struct var a, struct var b)
{
	return a.type == b.type && a.cons == b.cons;
}

bool nil(struct var v)
{
	return v.type == VAR_NIL;
}
