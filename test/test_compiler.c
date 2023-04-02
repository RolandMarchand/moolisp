#include "common.h"
#include "parser.h"

tgc_t gc;

void _main(void);
void test1();
void test2();
void test3();

int main(void)
{
	volatile void *dummy;
	tgc_start(&gc, &dummy);
	_main();
	tgc_stop(&gc);
}

void _main(void)
{
	test1();
	test2();
	test3();
}

void test1()
{
	struct var p = parse("(+ 1 2 3)");
	struct var *addition = car(&p);
	assert(addition);
	assert(addition->type == VAR_CONS);

	/* TODO: wire the symbols to the environment */
	struct var *plus = car(addition);
	assert(plus);
	assert(plus->type == VAR_SYMBOL);
	assert(strcmp(plus->as.symbol, "+") == 0);

	struct var *one = car(cdr(addition));
	assert(one);
	assert(one->type == VAR_NUMBER);
	assert(one->as.number == 1.0);

	struct var *two = car(cdr(cdr(addition)));
	assert(two);
	assert(two->type == VAR_NUMBER);
	assert(two->as.number == 2.0);

	struct var *three = car(cdr(cdr(cdr(addition))));
	assert(three);
	assert(three->type == VAR_NUMBER);
	assert(three->as.number == 3.0);

	struct var *_nil = cdr(cdr(cdr(cdr(addition))));
	assert(_nil);
	assert(_nil->type == VAR_NIL);
}

void test2()
{
	struct var p = parse("'(apply 'print '(\"hello\" \"yo\"))");
	struct var *printing = car(&p);
	assert(printing);
	assert(printing->type == VAR_CONS);

	struct var *quoted_list = car(printing);
	assert(quoted_list);
	assert(quoted_list->type == VAR_QUOTE);

	struct var *list = cdr(printing);
	assert(list);
	assert(list->type == VAR_CONS);

	struct var *apply = car(list);
	assert(apply);
	assert(apply->type == VAR_SYMBOL);
	assert(strcmp(apply->as.symbol, "apply") == 0);

	struct var *quoted_print = car(cdr(list));
	assert(quoted_print);
	assert(quoted_print->type == VAR_CONS);
	assert(car(quoted_print)->type == VAR_QUOTE);
	assert(cdr(quoted_print)->type == VAR_SYMBOL);
	assert(strcmp(cdr(quoted_print)->as.symbol, "print") == 0);

	struct var *string_list = car(cdr(cdr(list)));
	assert(string_list);
	assert(string_list->type == VAR_CONS);
	assert(car(string_list)->type == VAR_QUOTE);
	assert(cdr(string_list)->type == VAR_CONS);
	string_list = cdr(string_list);

	struct var *hello = car(string_list);
	assert(hello->type == VAR_STRING);
	assert(strcmp(hello->as.string, "hello") == 0);

	struct var *yo = car(cdr(string_list));
	assert(yo->type == VAR_STRING);
	assert(strcmp(yo->as.string, "yo") == 0);
}

void test3()
{
	struct var p = parse("()'()");
	assert(nilp(car(&p)));
	assert(!nilp(car(cdr(&p))));
}
