#include "core.h"

struct var *add(struct var *list)
{
	double total = 0;
	assert(listp(list));
	while (!_var2bool(nilp(list))) {
		assert(_var2bool(numberp(car(list))));
		total += car(list)->as.number;
		list = cdr(list);
	}
	return number(total);
}
