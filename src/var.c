#include "var.h"

extern tgc_t gc;

static struct var *create_var(const struct var *v);
static bool compare_cons(const struct var *c1, const struct var *c2);
static void print_list(const struct var *list);

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

struct var *c_function(struct var *(*f)(struct var *))
{
	assert(f);
	return create_var(&(struct var){
			.type = VAR_C_FUNCTION,
			.as.c_function = f
		});
}

struct var *closure(struct env *env, struct var *params, struct var *body)
{
	assert(env);
	assert(params);
	assert(body);
	struct closure *c = tgc_alloc(&gc, sizeof(struct closure));
	c->env = env;
	c->params = params;
	c->body = body;
	return create_var(&(struct var){
			.type = VAR_CLOSURE,
			.as.closure = c
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
	bool _equal;
	switch (a->type) {
	case VAR_NUMBER:
		_equal = a->as.number == b->as.number;
		break;
	case VAR_STRING:
		_equal = strcmp(a->as.string, b->as.string) == 0;
		break;
	case VAR_C_FUNCTION:
		_equal = a->as.c_function == b->as.c_function;
		break;
	case VAR_CONS:
		_equal = compare_cons(a, b);
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
	if (_equal) {
		return t();
	}
	return nil();
}

struct var *atom(const struct var *a)
{
	if (a->type != VAR_CONS) {
		return t();
	}
	return nilp(a);
}

struct var *stringp(const struct var *s)
{
	if (s->type == VAR_STRING) {
		return t();
	}
	return nil();
}

struct var *numberp(const struct var *n)
{
	assert(n);
	if (n->type == VAR_NUMBER) {
		return t();
	}
	return nil();
}

struct var *symbolp(const struct var *s)
{
	assert(s);
	if (s->type == VAR_SYMBOL) {
		return t();
	}
	return nil();
}

struct var *c_functionp(const struct var *f)
{
	assert(f);
	if (f->type == VAR_C_FUNCTION) {
		return t();
	}
	return nil();
}

struct var *closurep(const struct var *c)
{
	assert(c);
	if (c->type == VAR_CLOSURE) {
		return t();
	}
	return nil();
}

struct var *functionp(const struct var *f)
{
	assert(f);
	if (f->type == VAR_C_FUNCTION || f->type == VAR_CLOSURE) {
		return t();
	}
	return nil();
}

void print_list(const struct var *list)
{
	assert(_var2bool(listp(list)));
	printf("(");
	while (!_var2bool(nilp(list))) {
		print(car(list));
		list = cdr(list);
		if (_var2bool(nilp(list))) {
			break;
		} else {
			printf(" ");
			continue;
		}
	}
	printf(")");
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
		if (strcmp(car(v)->as.symbol, "quote") == 0) {
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
