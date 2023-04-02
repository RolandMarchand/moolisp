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
	struct var *param_cnt;
	struct var *param_names;
	struct var *body;
};

struct var *nil();
struct var *number(double number);
struct var *quote(struct var *expr);
struct var *string(const char *string);
struct var *symbol(const char *symbol);
struct var *cons(struct var *x, struct var *y);
struct var *lambda(struct var *param_cnt, struct var *param_names,
		   struct var *body);

struct var *car(const struct var *list);
struct var *cdr(const struct var *list);
/* struct var *dolist(const struct var *list, struct var *(f)(const struct var *)); */
struct var *atom(const struct var *v);
struct var *functionp(const struct var *f);
struct var *not(const struct var *v);
struct var *eq(const struct var *a, const struct var *b);
struct var *equal(const struct var *a, const struct var *b);
struct var *nilp(const struct var *v);
struct var *listp(const struct var *v);
struct var *print(const struct var *v);

bool _var2bool(const struct var *v);
