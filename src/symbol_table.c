#include "symbol_table.h"

#define SYMBOL_TABLE_MAX_LOAD 0.75	/* TODO: test the optimal max load */

static struct table *table_init(size_t size);
static struct table *symbol_table_increase_size(struct table *tbl);
static uint32_t murmur3_hash(const char *key, size_t len);
static uint32_t murmur3_scramble(uint32_t k);

struct table *table_init(size_t size)
{
	struct table *tbl = malloc(sizeof(struct symbol_table));
	tbl->buckets = calloc(size, sizeof(struct bucket));
	tbl->count = 0;
	tbl->size = size;
	return tbl;
}

struct symbol_table symbol_table_init(size_t size)
{
	return (struct symbol_table){.table = table_init(size)};
}

/* useless now that it is garbage collected */
/* void symbol_table_free(struct symbol_table *sym_tbl) */
/* { */
/* 	assert(sym_tbl != NULL && sym_tbl->table != NULL); */

/* 	struct table *table = sym_tbl->table; */
/* 	for (size_t i = 0; i < table->size; i++) { */
/* 		if (table->buckets[i].state == SYMBOL_TABLE_STATE_FILLED) { */
/* 			free(table->buckets[i].symbol.name); */
/* 		} */
/* 	} */

/* 	free(table->buckets); */
/* 	free(table); */
/* 	sym_tbl->table = NULL; */
/* } */

struct type *symbol_table_get(struct symbol_table *sym_tbl, const char *key)
{
	assert(sym_tbl != NULL && sym_tbl->table != NULL);

	struct table *table = sym_tbl->table;
	uint32_t hash = murmur3_hash(key, strlen(key));
	int idx = hash % table->size;

  next_bucket:
	switch(table->buckets[idx].state) {
	case SYMBOL_TABLE_STATE_EMPTY:
		return NULL;
	case SYMBOL_TABLE_STATE_FILLED:
		if (table->buckets[idx].hash == hash) {
			return table->buckets[idx].symbol.type;
		}
		/* fall-through */
	case SYMBOL_TABLE_STATE_ZOMBIE:
		idx = (idx + 1) % table->size;
	}
	goto next_bucket;
}

void symbol_table_set(struct symbol_table *sym_tbl, const char *key, struct type *type)
{
	assert(sym_tbl != NULL && sym_tbl->table != NULL);

	struct table *table = sym_tbl->table;
	uint32_t hash = murmur3_hash(key, strlen(key));
	uint32_t idx = hash % table->size;

  next_bucket:
	switch(table->buckets[idx].state) {
	case SYMBOL_TABLE_STATE_ZOMBIE:
	case SYMBOL_TABLE_STATE_EMPTY:
		if (table->count >= table->size * SYMBOL_TABLE_MAX_LOAD) {
			table = symbol_table_increase_size(table);
			sym_tbl->table = table;
			goto next_bucket;
		}
		table->buckets[idx].hash = hash;
		table->buckets[idx].state = SYMBOL_TABLE_STATE_FILLED;
		table->buckets[idx].symbol.name = strdup(key);
		table->buckets[idx].symbol.type = type;
		table->count++;
		return;
	case SYMBOL_TABLE_STATE_FILLED:
		if (table->buckets[idx].hash == hash) {
			table->buckets[idx].symbol.type = type;
			return;
		}
	}
	idx = (idx + 1) % table->size;
	goto next_bucket;
}

void symbol_table_delete(struct symbol_table *sym_tbl, const char *key)
{
	assert(sym_tbl != NULL);

	struct table *table = sym_tbl->table;
	uint32_t hash = murmur3_hash(key, strlen(key));
	int idx = hash % table->size;

  next_bucket:
	switch(table->buckets[idx].state) {
	case SYMBOL_TABLE_STATE_EMPTY:
		return;
	case SYMBOL_TABLE_STATE_FILLED:
		if (table->buckets[idx].hash == hash) {
			table->buckets[idx].state = SYMBOL_TABLE_STATE_ZOMBIE;
			free(table->buckets[idx].symbol.name);
			return;
		}
		/* fall-through */
	case SYMBOL_TABLE_STATE_ZOMBIE:
		idx = (idx + 1) % table->size;
	}
	goto next_bucket;
}

struct table *symbol_table_increase_size(struct table *tbl)
{
	struct table *big_tbl = table_init(tbl->size * 2);
	for (size_t i = 0; i < tbl->size; i++) {
		if (tbl->buckets[i].state != SYMBOL_TABLE_STATE_FILLED) {
			continue;
		}

		int big_idx = tbl->buckets[i].hash % big_tbl->size;
		while (big_tbl->buckets[big_idx].state != SYMBOL_TABLE_STATE_EMPTY) {
			big_idx = (big_idx + 1) % big_tbl->size;
		}
		big_tbl->buckets[big_idx].hash = tbl->buckets[i].hash;
		big_tbl->buckets[big_idx].state = SYMBOL_TABLE_STATE_FILLED;
		big_tbl->buckets[big_idx].symbol.name = strdup(tbl->buckets[i].symbol.name);
		big_tbl->buckets[big_idx].symbol.type = tbl->buckets[i].symbol.type;
		big_tbl->count++;
	}

	struct symbol_table tmp;
	tmp.table = tbl;
	symbol_table_free(&tmp);
	return big_tbl;
}

uint32_t murmur3_hash(const char *key, size_t len)
{
	uint32_t hash = 0;
	uint32_t k;
	uint32_t idx = 0;
	while (len - idx >= 4) {
		memcpy(&k, key + idx, sizeof(uint32_t));
		hash ^= murmur3_scramble(k);
		hash = (hash << 5) | (hash >> (32 - 5));
		hash = hash * 5 + 0xe6546b64;
		idx += 4;
	}

	k = 0;
	while (idx < len) {
		k <<= 8;
		k |= key[idx];
		idx += 1;
	}

	hash ^= murmur3_scramble(k);
	hash ^= len;
	hash ^= hash >> 16;
	hash *= 0x85ebca6b;
	hash ^= hash >> 13;
	hash *= 0xc2b2ae35;
	hash ^= hash >> 16;
	return hash;
}

uint32_t murmur3_scramble(uint32_t k)
{
	k *= 0xcc9e2d51;
	k = (k << 15) | (k >> 17);
	return k * 0x1b873593;
}
