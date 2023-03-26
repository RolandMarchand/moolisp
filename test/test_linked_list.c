#include "common.h"
#include "linked_list.h"

tgc_t gc;

char *str_key(void *data) {
	return (char *)data;
}

int main() {
	volatile void *dummy;
	tgc_start(&gc, &dummy);

	char *apple = "Apple";
	struct node *list1 = list_make(apple);
	assert(list_first(list1) == apple);
		
	char *banana = "Banana", *cherry = "Cherry";
	list1 = list_push(list1, banana);
	list1 = list_push(list1, cherry);
	assert(list_nth(list1, 0) == cherry);
	assert(list_nth(list1, 1) == banana);
	assert(list_nth(list1, 2) == apple);

	assert(list_first(list_rest(list1)) == banana);

	list1 = list_pop(list1);
	assert(list_nth(list1, 0) == banana);

	list1 = list_push(list1, cherry);
	assert(list_first(list1) == cherry);

	list1 = list_reverse(list1);
	assert(list_nth(list1, 0) == apple);
	assert(list_nth(list1, 1) == banana);
	assert(list_nth(list1, 2) == cherry);

	assert(list_count(list1) == 3);

	struct node *list_cpy = list_copy(list1);
	assert(list_nth(list_cpy, 0) == cherry);
	assert(list_nth(list_cpy, 1) == banana);
	assert(list_nth(list_cpy, 2) == apple);
	assert(list_count(list_cpy) == 3);

	char *orange = "Orange", *peach = "Peach";
	struct node *list2 = list_make(orange);
	list2 = list_push(list2, peach);
	list1 = list_concatenate(list1, list2);
	assert(list_nth(list1, 0) == cherry);
	assert(list_nth(list1, 1) == banana);
	assert(list_nth(list1, 2) == apple);
	assert(list_nth(list1, 3) == orange);
	assert(list_nth(list1, 4) == peach);

	int64_t index = list_findf(list1, "Peach", str_key);
	assert(index == 4);

	tgc_stop(&gc);
}
