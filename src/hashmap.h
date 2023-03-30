#pragma once

#include "common.h"
#include "var.h"

typedef enum {
	HASHMAP_STATE_EMPTY,
	HASHMAP_STATE_FILLED,
	HASHMAP_STATE_ZOMBIE
} HASHMAP_STATE;

struct bucket {
	struct var *symbol;
	uint32_t hash;
	HASHMAP_STATE state;
};

struct table {
	struct bucket *buckets;
	size_t count;
	size_t size;
};

struct hashmap {
	struct table *table;
};

struct hashmap hashmap_init(size_t size);
/* return the symbol associated with the key, or NULL */
struct var *hashmap_get(struct hashmap *, const char *key);
void hashmap_set(struct hashmap *, const char *key, struct var *symbol);
void hashmap_delete(struct hashmap *, const char *key);
