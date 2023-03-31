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

inline struct var number(double number) { return (struct var){ .type = VAR_NUMBER, .as.number = number }; }
inline struct var nil() { return (struct var){}; }
struct var string(char *string);
struct var symbol(char *symbol);
struct var lambda(size_t param_cnt, char **param_names, struct var body);
struct var cons(struct var x, struct var y);
struct var car(struct var list);
struct var cdr(struct var list);
inline struct var quote(struct var expr) { return cons((struct var){ .type = VAR_QUOTE }, expr); }
inline bool atom(struct var v) { return v.type != VAR_CONS; };
inline bool eq(struct var a, struct var b) { return a.type == b.type && a.as.cons == b.as.cons; }
inline bool nilp(struct var v) { return v.type == VAR_NIL; }
