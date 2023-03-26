#include "hashmap.h"

#define HASHMAP_MAX_LOAD 0.75	/* TODO: test the optimal max load */

extern tgc_t gc;

static struct table *table_init(size_t size);
static struct table *hashmap_increase_size(struct table *tbl);
static uint32_t murmur3_hash(const char *key, size_t len);
static uint32_t murmur3_scramble(uint32_t k);

struct table *table_init(size_t size)
{
	struct table *tbl = tgc_alloc(&gc, sizeof(struct hashmap));
	tbl->buckets = tgc_calloc(&gc, size, sizeof(struct bucket));
	tbl->count = 0;
	tbl->size = size;
	return tbl;
}

struct hashmap hashmap_init(size_t size)
{
	return (struct hashmap){.table = table_init(size)};
}

struct var *hashmap_get(struct hashmap *hashmap, const char *key)
{
	assert(hashmap != NULL && hashmap->table != NULL);

	struct table *table = hashmap->table;
	uint32_t hash = murmur3_hash(key, strlen(key));
	int idx = hash % table->size;

  next_bucket:
	switch(table->buckets[idx].state) {
	case HASHMAP_STATE_EMPTY:
		return NULL;
	case HASHMAP_STATE_FILLED:
		if (table->buckets[idx].hash == hash) {
			return table->buckets[idx].symbol;
		}
		/* fall-through */
	case HASHMAP_STATE_ZOMBIE:
		idx = (idx + 1) % table->size;
	}
	goto next_bucket;
}

void hashmap_set(struct hashmap *hashmap, const char *key, struct var *symbol)
{
	assert(hashmap != NULL && hashmap->table != NULL);

	struct table *table = hashmap->table;
	uint32_t hash = murmur3_hash(key, strlen(key));
	uint32_t idx = hash % table->size;

  next_bucket:
	switch(table->buckets[idx].state) {
	case HASHMAP_STATE_ZOMBIE:
	case HASHMAP_STATE_EMPTY:
		if (table->count >= table->size * HASHMAP_MAX_LOAD) {
			table = hashmap_increase_size(table);
			hashmap->table = table;
			goto next_bucket;
		}
		table->buckets[idx].hash = hash;
		table->buckets[idx].state = HASHMAP_STATE_FILLED;
		table->buckets[idx].symbol = symbol;
		table->count++;
		return;
	case HASHMAP_STATE_FILLED:
		if (table->buckets[idx].hash == hash) {
			table->buckets[idx].symbol = symbol;
			return;
		}
	}
	idx = (idx + 1) % table->size;
	goto next_bucket;
}

void hashmap_delete(struct hashmap *hashmap, const char *key)
{
	assert(hashmap != NULL && hashmap->table != NULL);

	struct table *table = hashmap->table;
	uint32_t hash = murmur3_hash(key, strlen(key));
	int idx = hash % table->size;

  next_bucket:
	switch(table->buckets[idx].state) {
	case HASHMAP_STATE_EMPTY:
		return;
	case HASHMAP_STATE_FILLED:
		if (table->buckets[idx].hash == hash) {
			table->buckets[idx].state = HASHMAP_STATE_ZOMBIE;
			return;
		}
		/* fall-through */
	case HASHMAP_STATE_ZOMBIE:
		idx = (idx + 1) % table->size;
	}
	goto next_bucket;
}

struct table *hashmap_increase_size(struct table *tbl)
{
	struct table *big_tbl = table_init(tbl->size * 2);
	for (size_t i = 0; i < tbl->size; i++) {
		if (tbl->buckets[i].state != HASHMAP_STATE_FILLED) {
			continue;
		}

		int big_idx = tbl->buckets[i].hash % big_tbl->size;
		while (big_tbl->buckets[big_idx].state != HASHMAP_STATE_EMPTY) {
			big_idx = (big_idx + 1) % big_tbl->size;
		}
		big_tbl->buckets[big_idx].hash = tbl->buckets[i].hash;
		big_tbl->buckets[big_idx].state = HASHMAP_STATE_FILLED;
		big_tbl->buckets[big_idx].symbol = tbl->buckets[i].symbol;
		big_tbl->count++;
	}
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
