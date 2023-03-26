#include "common.h"
#include "lisp.h"

#pragma once

typedef enum {
	SYMBOL_TABLE_STATE_EMPTY,
	SYMBOL_TABLE_STATE_FILLED,
	SYMBOL_TABLE_STATE_ZOMBIE
} SYMBOL_TABLE_STATE;


struct bucket {
	struct var *symbol;
	uint32_t hash;
	SYMBOL_TABLE_STATE state;
};

struct table {
	struct bucket *buckets;
	size_t count;
	size_t size;
};

struct symbol_table {
	struct table *table;
};

struct symbol_table symbol_table_init(size_t size);
void symbol_table_free(struct symbol_table *sym_tbl);
/* return the type of the symbol associated with the key, or NULL */
struct type *symbol_table_get(struct symbol_table *sym_tbl, const char *key);
void symbol_table_set(struct symbol_table *sym_tbl, const char *key, struct type *type);
void symbol_table_delete(struct symbol_table *sym_tbl, const char *key);
