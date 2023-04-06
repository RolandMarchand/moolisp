#include "hashmap.h"

#define HASHMAP_MAX_LOAD 0.75	/* TODO: test the optimal max load */

extern tgc_t gc;

static struct hashmap *hashmap_increase_size(struct hashmap *tbl);
static uint32_t murmur3_hash(const char *key, size_t len);
static uint32_t murmur3_scramble(uint32_t k);

struct hashmap *hashmap_init(size_t size)
{
	struct hashmap *hashmap = tgc_alloc(&gc, sizeof(struct hashmap));
	hashmap->buckets = tgc_calloc(&gc, size, sizeof(struct bucket));
	hashmap->count = 0;
	hashmap->size = size;
	return hashmap;
}

struct var *hashmap_get(struct hashmap *hashmap, const char *key)
{
	assert(hashmap);
	assert(key);

	uint32_t hash = murmur3_hash(key, strlen(key));
	int idx = hash % hashmap->size;

  next_bucket:
	switch(hashmap->buckets[idx].state) {
	case HASHMAP_STATE_EMPTY:
		return NULL;
	case HASHMAP_STATE_FILLED:
		if (hashmap->buckets[idx].hash == hash) {
			return hashmap->buckets[idx].symbol;
		}
		__attribute__((fallthrough));
	case HASHMAP_STATE_ZOMBIE:
		idx = (idx + 1) % hashmap->size;
	}
	goto next_bucket;
}

void hashmap_set(struct hashmap **_hashmap, const char *key,
			    struct var *symbol)
{
	assert(_hashmap);
	assert(*_hashmap);
	assert(key);
	assert(symbol);

	struct hashmap *hashmap = *_hashmap;
	uint32_t hash = murmur3_hash(key, strlen(key));
	uint32_t idx = hash % hashmap->size;

  next_bucket:
	switch(hashmap->buckets[idx].state) {
	case HASHMAP_STATE_ZOMBIE:
	case HASHMAP_STATE_EMPTY:
		if (hashmap->count >= hashmap->size * HASHMAP_MAX_LOAD) {
			hashmap = hashmap_increase_size(hashmap);
			goto next_bucket;
		}
		hashmap->buckets[idx].hash = hash;
		hashmap->buckets[idx].state = HASHMAP_STATE_FILLED;
		hashmap->buckets[idx].symbol = symbol;
		hashmap->count++;
		*_hashmap = hashmap;
		return;
	case HASHMAP_STATE_FILLED:
		if (hashmap->buckets[idx].hash == hash) {
			hashmap->buckets[idx].symbol = symbol;
			*_hashmap = hashmap;
			return;
		}
	}
	idx = (idx + 1) % hashmap->size;
	goto next_bucket;
}

void hashmap_delete(struct hashmap *hashmap, const char *key)
{
	assert(hashmap);
	assert(key);

	uint32_t hash = murmur3_hash(key, strlen(key));
	int idx = hash % hashmap->size;

  next_bucket:
	switch(hashmap->buckets[idx].state) {
	case HASHMAP_STATE_EMPTY:
		return;
	case HASHMAP_STATE_FILLED:
		if (hashmap->buckets[idx].hash == hash) {
			hashmap->buckets[idx].state = HASHMAP_STATE_ZOMBIE;
			return;
		}
		__attribute__((fallthrough));
	case HASHMAP_STATE_ZOMBIE:
		idx = (idx + 1) % hashmap->size;
	}
	goto next_bucket;
}

struct hashmap *hashmap_increase_size(struct hashmap *hashmap)
{
	struct hashmap *big_hashmap = hashmap_init(hashmap->size * 2);
	for (size_t i = 0; i < hashmap->size; i++) {
		if (hashmap->buckets[i].state != HASHMAP_STATE_FILLED) {
			continue;
		}

		size_t big_idx = hashmap->buckets[i].hash % big_hashmap->size;
		while (big_hashmap->buckets[big_idx].state
		       != HASHMAP_STATE_EMPTY) {
			big_idx = (big_idx + 1) % big_hashmap->size;
		}
		big_hashmap->buckets[big_idx].hash = hashmap->buckets[i].hash;
		big_hashmap->buckets[big_idx].state = HASHMAP_STATE_FILLED;
		big_hashmap->buckets[big_idx].symbol = hashmap->buckets[i].symbol;
		big_hashmap->count++;
	}
	return big_hashmap;
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
