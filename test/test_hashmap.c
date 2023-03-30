#include "common.h"
#include "hashmap.h"

tgc_t gc;

void _main(void);

int main(void)
{
	volatile void *dummy;
	tgc_start(&gc, &dummy);
	_main();
	tgc_stop(&gc);

}

void _main(void)
{
	struct hashmap hm = hashmap_init(4);
	struct var a, b, c, d, e, f, s;

	assert(hashmap_get(&hm, "false") == NULL);

	hashmap_set(&hm, "set", &s);
	assert(hashmap_get(&hm, "set") == &s);

	hashmap_set(&hm, "0", &a);
	hashmap_set(&hm, "1", &b);
	hashmap_set(&hm, "2", &c);
	hashmap_set(&hm, "3", &d); /* size increase */
	assert(hashmap_get(&hm, "set") == &s);

	hashmap_delete(&hm, "set");
	hashmap_set(&hm, "4", &e);
	hashmap_set(&hm, "5", &f);
	assert(hashmap_get(&hm, "set") == NULL);

	hashmap_delete(&hm, "0");
	hashmap_delete(&hm, "2");
	hashmap_delete(&hm, "4");
	hashmap_delete(&hm, "5");
	assert(hashmap_get(&hm, "0") == NULL);
	assert(hashmap_get(&hm, "1") == &b);
	assert(hashmap_get(&hm, "2") == NULL);
	assert(hashmap_get(&hm, "3") == &d);
	assert(hashmap_get(&hm, "4") == NULL);
	assert(hashmap_get(&hm, "5") == NULL);
}
