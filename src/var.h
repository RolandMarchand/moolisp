#pragma once

#include "common.h"
#define t() number(1)

typedef enum {
	VAR_CONS = 0,
	VAR_SYMBOL,
	VAR_NUMBER,
	VAR_STRING,
	VAR_C_FUNCTION,
	VAR_CLOSURE
} VarType;

struct var {
	VarType type;
	union {
		char *string;
		char *symbol;
		double number;
		struct cons *cons;
		struct var *(*c_function)(const struct var *);
		struct closure *closure;
	} as;
};

struct cons {
	struct var *x;
	struct var *y;
};

#include "env.h"
struct closure {
	struct env *env;
	struct var *params;
	struct var *body;
};

struct var *nil();
struct var *number(double number);
struct var *quote(struct var *expr);
struct var *string(const char *string);
struct var *symbol(const char *symbol);
struct var *cons(struct var *x, struct var *y);
struct var *closure(struct env *, struct var *params, struct var *body);
struct var *c_function(struct var *(*f)(const struct var *));
struct var *car(const struct var *list);
struct var *cdr(const struct var *list);
struct var *atom(const struct var *);
struct var *stringp(const struct var *);
struct var *numberp(const struct var *);
struct var *symbolp(const struct var *);
struct var *c_functionp(const struct var *);
struct var *closurep(const struct var *);
struct var *functionp(const struct var *);
struct var *not(const struct var *);
struct var *nilp(const struct var *);
struct var *listp(const struct var *);

bool _var2bool(const struct var *);
bool _quotep(const struct var *);
