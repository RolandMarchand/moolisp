#pragma once

#include "common.h"

typedef enum {
	VAR_CONS = 0,
	VAR_SYMBOL,
	VAR_NUMBER,
	VAR_STRING,
	VAR_FUNCTION,
} VarType;

struct var {
	VarType type;
	union {
		char *string;
		char *symbol;
		double number;
		struct cons *cons;
		struct function *function;
	} as;
};

struct cons {
	struct var *x;
	struct var *y;
};

struct function {
	size_t param_cnt;
	char **param_names;
	struct var *body;
};

struct var *number(double number);
struct var *nil();
struct var *string(const char *string);
struct var *symbol(const char *symbol);
struct var *lambda(size_t param_cnt, const char **param_names,
		  const struct var *body);
struct var *cons(struct var *x, struct var *y);
struct var *car(const struct var *list);
struct var *cdr(const struct var *list);
struct var *quote(struct var *expr);
struct var *dolist(const struct var *list, struct var *(f)(const struct var *));
struct var *atom(const struct var *v);
struct var *functionp(const struct var *f);
struct var *not(const struct var *v);
struct var *eq(const struct var *a, const struct var *b);
struct var *equal(const struct var *a, const struct var *b);
struct var *nilp(const struct var *v);
struct var *listp(const struct var *v);
struct var *print(const struct var *v);
