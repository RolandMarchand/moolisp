#include "var.h"
#define t() number(1)

extern tgc_t gc;

static struct var *create_var(const struct var *v);
static bool compare_functions(const struct var *f1, const struct var *f2);
static bool compare_cons(const struct var *c1, const struct var *c2);

bool _var2bool(const struct var *v)
{
	assert(v);
	return memcmp(v, &(struct var){}, sizeof(struct var)) != 0;
}

struct var *create_var(const struct var *v)
{
	assert(v);
	struct var *ret = tgc_alloc(&gc, sizeof(struct var));
	*ret = *v;
	return ret;
}

struct var *lambda(struct var *param_cnt, struct var *param_names,
		   struct var *body)
{
	assert(param_cnt);
	assert(param_names);
	assert(body);
	struct function *f = tgc_alloc(&gc, sizeof(struct function));
	f->param_cnt = param_cnt;
	f->param_names = param_names;
	f->body = body;
	return create_var(&(struct var){
			.type = VAR_FUNCTION,
			.as.function = f
		});
}

struct var *cons(struct var *x, struct var *y)
{
	assert(x);
	assert(y);
	struct cons *c = tgc_calloc(&gc, 1, sizeof(struct cons));
	c->x = x, c->y = y;
	return create_var(&(struct var){
			.type = VAR_CONS,
			.as.cons = c
		});
}

struct var *string(const char *string)
{
	assert(string);
	char *string_cpy = tgc_alloc(&gc, strlen(string) + 1);
	strcpy(string_cpy, string);
	return create_var(&(struct var){
			.type = VAR_STRING,
			.as.string = string_cpy
		});
}

struct var *symbol(const char *symbol)
{
	assert(symbol);
	char *symbol_cpy = tgc_alloc(&gc, strlen(symbol) + 1);
	strcpy(symbol_cpy, symbol);
	return create_var(&(struct var){
			.type = VAR_SYMBOL,
			.as.symbol = symbol_cpy
		});
}

struct var *car(const struct var *list)
{
	assert(list);
	if (!_var2bool(list)) {
		return (struct var *)list;
	}
	assert(list->type == VAR_CONS);
	return list->as.cons->x;
}

struct var *cdr(const struct var *list)
{
	assert(list);
	if (!_var2bool(list)) {
		return (struct var *)list;
	}
	assert(list->type == VAR_CONS);
	return list->as.cons->y;
}

struct var *number(double number)
{
	return create_var(&(struct var){
			.type = VAR_NUMBER,
			.as.number = number
		});
}

struct var *quote(struct var *expr)
{
	assert(expr);
	return cons(symbol("quote"), cons(expr, nil()));
}

struct var *nil()
{
	return create_var(&(struct var){});
}

struct var *nilp(const struct var *v)
{
	assert(v);
	if (_var2bool(v)) {
		return nil();
	}
	return t();
}

struct var *listp(const struct var *v)
{
	assert(v);
	if (v->type == VAR_CONS) {
		return t();
	}
	return nil();
}

struct var *not(const struct var *v)
{
	assert(v);
	if (_var2bool(nilp(v))) {
		return t();
	}
	return nil();
}

struct var *eq(const struct var *a, const struct var *b)
{
	assert(a);
	assert(b);
	if (a == b) {
		return t();
	}
	return nil();
}

bool compare_functions(const struct var *_f1, const struct var *_f2)
{
	assert(_f1);
	assert(_f2);
	assert(_var2bool(functionp(_f1)));
	assert(_var2bool(functionp(_f2)));
	struct function *f1 = _f1->as.function;
	struct function *f2 = _f2->as.function;

	return _var2bool(equal(f1->param_cnt, f2->param_cnt))
		&& _var2bool(equal(f1->param_names, f2->param_names))
		&& _var2bool(equal(f1->body, f2->body));
}

bool compare_cons(const struct var *c1, const struct var *c2)
{
	bool c1_nil = _var2bool(nilp(c1));
	bool c2_nil = _var2bool(nilp(c2));
	if ((c1_nil ^ c2_nil) != 0) {
		return _var2bool(nil());
	}
	if (c1_nil) {
		return _var2bool(t());
	}
	return _var2bool(equal(c1->as.cons->x, c2->as.cons->x))
		&& _var2bool(equal(c1->as.cons->y, c2->as.cons->y));
}

struct var *equal(const struct var *a, const struct var *b)
{
	if (a->type != b->type) {
		return nil();
	}
	if (_var2bool(eq(a, b))) {
		return t();
	}
	bool _equal = false;
	switch (a->type) {
	case VAR_NUMBER:
		_equal = a->as.number == b->as.number;
		break;
	case VAR_SYMBOL:
		_equal = strcmp(a->as.symbol, b->as.symbol) == 0;
		break;
	case VAR_STRING:
		_equal = strcmp(a->as.string, b->as.string) == 0;
		break;
	case VAR_FUNCTION:
		_equal = compare_functions(a, b);
		break;
	case VAR_CONS:
		_equal = compare_cons(a, b);
		break;
	default:
		fprintf(stderr, "error: unknown type to compare equality\n");
		exit(EXIT_FAILURE);
	}
	if (_equal) {
		return t();
	}
	return nil();
}

struct var *atom(const struct var *v)
{
	if (v->type != VAR_CONS) {
		return t();
	}
	return nilp(v);
}

struct var *functionp(const struct var *f)
{
	if (f->type == VAR_FUNCTION) {
		return t();
	}
	return nil();	
}

/* struct var *dolist(const struct var *list, struct var *(f)(const struct var *)) */
/* { */
/* 	if (!_var2bool(list)) { */
/* 		/\* TODO: error handling *\/ */
/* 		return (struct var *)list; */
/* 	} */

/* 	assert(_var2bool(listp(list))); */
/* 	struct var *first = car(list); */
/* 	struct var *rest = cdr(list); */
/* 	struct var *last_ret = NULL; */
/* 	do { */
/* 		last_ret = f(first); */
/* 		first = car(rest); */
/* 		rest = cdr(rest); */
/* 	} while (_var2bool(first) || _var2bool(rest)); */

/* 	assert(last_ret); */
/* 	return last_ret; */
/* } */

struct var *print_list(struct var *v)
{
	assert(_var2bool(listp(v)));
	return v;
}

struct var *print(const struct var *v)
{
	switch (v->type) {
	case VAR_SYMBOL:
		printf("%s", v->as.symbol);
		break;
	case VAR_NUMBER:
		printf("%g", v->as.number);
		break;
	case VAR_STRING:
		printf("%s", v->as.string);
		break;
	case VAR_CONS:
		if (_var2bool(equal(car(v), symbol("quote")))
		    && _var2bool(nilp(cdr(cdr(v))))) {
			printf("'");
			print(car(cdr(v)));
			break;
		}
		printf("(");
		/* TODO: implement functions and evals */
		/* dolist(v, print); */
		printf(")");
		break;
	default:
		fprintf(stderr, "error: unknown variant type '%d'\n", v->type);
		exit(EXIT_FAILURE);
	}
	return (struct var *)v;
}
