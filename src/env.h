#pragma once

#include "common.h"
#include "var.h"

struct env {
	struct env* outer;
	struct hashmap *data;
};

struct env *env_make(struct env *outer, struct var *binds, struct var *exprs);
void env_set(struct env *current, struct var *symbol, struct var *value);
struct var *env_get(struct env *current, struct var *symbol);
struct env *env_find(struct env *current, struct var *symbol);
