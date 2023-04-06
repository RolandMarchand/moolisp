#pragma once

#include "common.h"
#include "var.h"

struct env {
	struct env* outer;
	struct hashmap *data;
};

struct env *env_make(struct env *outer, struct var *binds, struct var *exprs);
void env_set(struct env *current, struct var *symbol, struct var *value);
void env_set_C_fn(struct env *current, char* symbol_name,
			 struct var *(*fn)(struct var *));
struct var *env_get(struct env *current, struct var *symbol);
struct env *env_find(struct env *current, struct var *symbol);
