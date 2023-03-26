#pragma once

#include "common.h"

struct node {
	void *data;
	struct node *next;
};

struct node *list_make(void *data_ptr);
struct node *list_push(struct node *lst, void *data_ptr);
void *list_first(struct node *lst);
void *list_nth(struct node *lst, int n);
struct node *list_rest(struct node *lst);
struct node *list_pop(struct node *lst);
struct node *list_reverse(struct node *lst);
int64_t list_count(struct node *lst);
struct node *list_concatenate(struct node *lst1, struct node *lst2);
struct node *list_copy(struct node *lst);
int64_t list_findf(struct node *lst, char* keystring, char*(*fn)(void*));
