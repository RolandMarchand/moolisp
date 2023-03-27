#pragma once

#include "common.h"

typedef enum {
	VAR_NIL = 0,
	VAR_SYMBOL,
	VAR_INT,
	VAR_STRING,
	VAR_CONS,
	VAR_FUNCTION
} VarType;

struct var {
	VarType type;
	union {
		char *string;
		char *symbol;
		int64_t i64;
		struct cons *cons;
		struct function *function;
	};
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

struct var create_int(int64_t i64);
struct var lambda(size_t param_cnt, char **param_names, struct var body);
struct var cons(struct var x, struct var y);
struct var create_string(char *string);
struct var car(struct var list);
struct var cdr(struct var list);
bool atom(struct var v);
bool eq(struct var a, struct var b);
bool nil(struct var v);