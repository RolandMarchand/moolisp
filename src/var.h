#pragma once

#include "common.h"

typedef enum {
	VAR_NIL = 0,
	VAR_SYMBOL,
	VAR_NUMBER,
	VAR_STRING,
	VAR_CONS,
	VAR_FUNCTION,
	VAR_QUOTE
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
	struct var x;
	struct var y;
};

struct function {
	size_t param_cnt;
	char **param_names;
	struct var body;
};

struct var number(double number);
struct var nil();
struct var string(char *string);
struct var symbol(char *symbol);
struct var lambda(size_t param_cnt, char **param_names, struct var body);
struct var cons(struct var x, struct var y);
struct var *car(struct var *list);
struct var *cdr(struct var *list);
struct var quote(struct var expr);
bool atom(struct var *v);
bool eq(struct var *a, struct var *b);
bool nilp(struct var *v);
