#include "lexer.h"
#include "repl.h"
#include "common.h"
#include "var.h"
#include "context.h"

tgc_t gc;

struct var eval(struct var expr);
struct var progn(struct var list);
struct var unless(struct var test, struct var then, struct var otherwise);

int main(int argc, char **argv)
{

}

struct var eval(struct var expr)
{

}

struct var progn(struct var list)
{
	assert(list.type == VAR_CONS);
	struct var ret;
	do {
		ret = eval(list.cons->x);
		list = list.cons->y;
	} while (!nil(list));
	return ret;
}

struct var unless(struct var test, struct var then, struct var otherwise)
{
	if (nil(test)) {
		return progn(then);
	} else {
		return progn(otherwise);
	}
}
