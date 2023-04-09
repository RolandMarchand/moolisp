#include "env.h"
#include "hashmap.h"

extern tgc_t gc;

struct env *env_make(struct env *outer, struct var *binds, struct var *exprs)
{
	assert(binds);
	assert(exprs);
	struct env *env = tgc_alloc(&gc, sizeof(*env));
	env->outer = outer;
	env->data = hashmap_init(8);

	while (_var2bool(car(binds))) {
		assert(symbolp(car(binds)));
		env_set(env, car(binds), car(exprs));
		binds = cdr(binds);
		exprs = cdr(exprs);
	}

	return env;
}

void env_set(struct env *current, struct var *symbol, struct var *value)
{
	assert(current);
	assert(symbol);
	assert(value);
	assert(_var2bool(symbolp(symbol)));
	hashmap_set(&current->data, symbol->as.symbol, value);
}

struct env *env_find(struct env *current, struct var *symbol)
{
	assert(current);
	assert(symbol);
	struct var *val = hashmap_get(current->data, symbol->as.symbol);

	if (_var2bool(val)) {
		return current;
	}
	if (current->outer) {
		return env_find(current->outer, symbol);
	}
	return NULL;
}

struct var *env_get(struct env *current, struct var *symbol)
{
	assert(current);
	assert(symbol);
	struct env *env = env_find(current, symbol);
	if (env) {
		return hashmap_get(env->data, symbol->as.symbol);
	}
	return nil();
}
