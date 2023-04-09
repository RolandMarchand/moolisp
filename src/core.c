#include "core.h"

static void print_list(const struct var *list);
static bool compare_cons(const struct var *c1, const struct var *c2);

struct var *print(const struct var *v)
{
	assert(v);
	switch (v->type) {
	case VAR_SYMBOL:
		printf("%s", v->as.symbol);
		break;
	case VAR_NUMBER:
		printf("%g", v->as.number);
		break;
	case VAR_STRING:
		printf("\"%s\"", v->as.string);
		break;
	case VAR_CONS:
		if (_quotep(v)) {
			printf("'");
			print(car(cdr(v)));
			break;
		}
		print_list(v);
		break;
	case VAR_C_FUNCTION:
		printf("<SYSTEM-FUNCTION %p>", v->as.c_function);
		break;
	case VAR_CLOSURE:
		printf("<FUNCTION ");
		print(v->as.closure->params);
		printf(" ");
		print(v->as.closure->body);
		printf(">");
		break;
	default:
		fprintf(stderr, "error: unknown variant type '%d'\n", v->type);
		exit(EXIT_FAILURE);
	}
	return (struct var *)v;
}

void print_list(const struct var *list)
{
	assert(_var2bool(listp(list)));
	printf("(");
	while (_var2bool(list)) {
		print(car(list));
		list = cdr(list);
		if (!_var2bool(list)) {
			break;
		} else {
			printf(" ");
			continue;
		}
	}
	printf(")");
}

struct var *add(struct var *list)
{
	double total = 0;
	assert(listp(list));
	while (_var2bool(list)) {
		assert(_var2bool(numberp(car(list))));
		total += car(list)->as.number;
		list = cdr(list);
	}
	return number(total);
}

struct var *eq(const struct var *list)
{
	assert(list);
	while (_var2bool(cdr(list))) {
		if (car(list) != car(cdr(list))) {
			return nil();
		}
		list = cdr(list);
	}
	return t();
}

bool compare_cons(const struct var *c1, const struct var *c2)
{
	bool c1_nil = !_var2bool(c1);
	bool c2_nil = !_var2bool(c2);
	if ((c1_nil ^ c2_nil) != 0) {
		return false;
	}
	if (c1_nil) {
		return true;
	}
	return _var2bool(equal(cons(c1->as.cons->x, cons(c2->as.cons->x, nil()))))
		&& _var2bool(equal(cons(c1->as.cons->y, cons(c2->as.cons->y, nil()))));
}

struct var *equal(const struct var *list)
{
	assert(list);
	while (_var2bool(cdr(list))) {
		struct var *a = car(list);
		struct var *b = car(cdr(list));
		if (a->type != b->type) {
			return nil();
		}
		if (a == b) {
			list = cdr(list);
			continue;
		}
		switch (a->type) {
		case VAR_NUMBER:
			if (a->as.number != b->as.number) {
				return nil();
			}
			break;
		case VAR_STRING:
			if (strcmp(a->as.string, b->as.string) != 0) {
				return nil();
			}
			break;
		case VAR_C_FUNCTION:
			if (a->as.c_function != b->as.c_function) {
				return nil();
			}
			break;
		case VAR_CONS:
			if (!compare_cons(a, b)) {
				return nil();
			}
			break;
			/* Identity comparison */
		case VAR_SYMBOL:
		case VAR_CLOSURE:
			return nil();
			break;
		default:
			fprintf(stderr, "error: unknown type to compare equality\n");
			exit(EXIT_FAILURE);
		}
	}
	return t();
}

struct var *length(struct var *list)
{
	assert(list);
	assert(_var2bool(listp(list)));
	assert(!_var2bool(cdr(list)));
	list = car(list);

	size_t len = 0;
	while (_var2bool(list)) {
		len++;
		list = cdr(list);
	}
	return number(len);
}
