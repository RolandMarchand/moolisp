#include "linked_list.h"

extern tgc_t gc;

struct node *list_make(void *data_ptr)
{
	return list_push(NULL, data_ptr);
}

struct node *list_push(struct node *list, void *data_ptr)
{
	struct node *new_head = tgc_alloc(&gc, sizeof(struct node));
	new_head->data = data_ptr;
	new_head->next = list;
	return new_head;
}

void *list_first(struct node *list)
{
	return (list ? list->data : NULL);
}

struct node *list_pop(struct node *list)
{
	return (list ? list->next : NULL);
}

int64_t list_count(struct node *list)
{
	if (!list) {
		return 0;
	}

	int64_t counter = 1;
	while(list->next) {
		counter++;
		list = list->next;
	}
	return counter;
}

struct node *list_reverse(struct node *list)
{
	if (!list || !list->next) {
		return list;
	}

	struct node *prev = NULL, *next = NULL, *current = list;
	while (current) {
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	list = prev;
	return list;
}

struct node *list_concatenate(struct node *list1, struct node *list2)
{
	if (!list1 || !list2) {
		return list1 ? list1 : list2;
	}
	struct node *copy1 = list_copy(list1);
	struct node *copy2 = list_copy(list2);
	struct node *tail = copy1;
	while(tail->next) {
		tail = tail->next;
	}
	tail->next = copy2;
	return copy1;
}

void *list_nth(struct node *list, int n)
{
	for (int idx = 0; list; list = list->next) {
		if (n == idx++) {
			return list->data;
		}
	}
	return NULL;
}

struct node *list_rest(struct node *list)
{
	return list ? list->next : NULL;
}

struct node *list_copy(struct node *list)
{
	struct node *new_list = NULL;
	while(list) {
		new_list = list_push(new_list, list->data);
		list = list->next;
	}
	return new_list;
}

int64_t list_findf(struct node *list, char* keystring, char*(*fn)(void*))
{
	struct node *current = list;
	for (int64_t idx = 0; current; idx++) {
		char* item = fn(current->data);
		if (strcmp(keystring, item) == 0) {
			return idx;
		}
		current = current->next;
	}
	return -1;
}
