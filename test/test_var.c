#include "common.h"
#include "var.h"

tgc_t gc;

void _main(void);
void test1();
void test2();

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
}

void test1()
{
	struct var *num = number(3);
	assert(num->type == VAR_NUMBER);
	assert(num->as.number == 3.0);

	struct var *str = string("string");
	assert(str->type == VAR_STRING);
	assert(strcmp(str->as.string, "string") == 0);

	/* TODO: wire the symbols to the environment */
	struct var *sym = symbol("symbol");
	assert(sym->type == VAR_SYMBOL);
	assert(strcmp(sym->as.string, "symbol") == 0);

	struct var *n = nil();
	assert(n->type == VAR_CONS);
	assert(n->as.cons == NULL);

	struct var *c = cons(sym, cons(str, n));
	assert(c->type == VAR_CONS);
	assert(c->as.cons->x == sym);
	c = c->as.cons->y;
	assert(c->type == VAR_CONS);
	assert(c->as.cons->x == str);
	assert(c->as.cons->y == n);

	struct var *q = quote(n);
	assert(q->type == VAR_CONS);
	assert(q->as.cons->x->type == VAR_SYMBOL);
	assert(strcmp(q->as.cons->x->as.symbol, "quote") == 0);
	q = q->as.cons->y;
	assert(q->type == VAR_CONS);
	assert(q->as.cons->x == n);
	q = q->as.cons->y;
	assert(q->type == VAR_CONS);
	assert(q->as.cons == NULL);

	struct var *param_cnt = number(2);
	struct var *param_names = cons(symbol("x"), cons(symbol("y"), nil()));
	struct var *body = cons(symbol("+"),
				cons(symbol("x"),
				     cons(symbol("y"), nil())));
	struct var *func = lambda(param_cnt, param_names, body);
	assert(func->type == VAR_FUNCTION);
	assert(func->as.function->param_cnt == param_cnt);
	assert(func->as.function->param_names == param_names);
	assert(func->as.function->body == body);

	num = number(0);
	struct var *n1 = nil();
	struct var *n2 = nil();
	func = lambda(num, n1, n2);
	assert(func->type == VAR_FUNCTION);
	assert(func->as.function->param_cnt == num);
	assert(func->as.function->param_names == n1);
	assert(func->as.function->body == n2);
}

void test2()
{
	struct var *num = number(0);
	struct var *str = string("string");
	struct var *sym = symbol("symbol");
	struct var *q = quote(sym);
	struct var *_nil = nil();
	struct var *c = cons(num, cons(str, _nil));
	struct var *func = lambda(number(0), nil(), nil());

	assert(car(c) == num);
	assert(car(cdr(c)) == str);
	assert(car(cdr(cdr(c))) == _nil);
	assert(cdr(cdr(cdr(c))) == _nil);

	assert(_var2bool(atom(num)));
	assert(_var2bool(atom(str)));
	assert(_var2bool(atom(sym)));
	assert(!_var2bool(atom(q)));
	assert(_var2bool(atom(_nil)));
	assert(!_var2bool(atom(c)));
	assert(_var2bool(atom(func)));

	assert(!_var2bool(functionp(num)));
	assert(!_var2bool(functionp(str)));
	assert(!_var2bool(functionp(sym)));
	assert(!_var2bool(functionp(q)));
	assert(!_var2bool(functionp(_nil)));
	assert(!_var2bool(functionp(c)));
	assert(_var2bool(functionp(func)));

	assert(!_var2bool(nilp(num)));
	assert(!_var2bool(nilp(str)));
	assert(!_var2bool(nilp(sym)));
	assert(!_var2bool(nilp(q)));
	assert(_var2bool(nilp(_nil)));
	assert(!_var2bool(nilp(c)));
	assert(!_var2bool(nilp(func)));

	assert(_var2bool(not(nilp(num))));
	assert(_var2bool(not(nilp(str))));
	assert(_var2bool(not(nilp(sym))));
	assert(_var2bool(not(nilp(q))));
	assert(!_var2bool(not(nilp(_nil))));
	assert(_var2bool(not(nilp(c))));
	assert(_var2bool(not(nilp(func))));

	assert(_var2bool(eq(num, num)));
	assert(!_var2bool(eq(str, num)));
	assert(!_var2bool(eq(sym, num)));
	assert(_var2bool(eq(q, q)));
	assert(!_var2bool(eq(_nil, c)));
	assert(!_var2bool(eq(c, _nil)));
	assert(_var2bool(eq(func, func)));

	assert(!_var2bool(listp(num)));
	assert(!_var2bool(listp(str)));
	assert(!_var2bool(listp(sym)));
	assert(_var2bool(listp(q)));
	assert(_var2bool(listp(_nil)));
	assert(_var2bool(listp(c)));
	assert(!_var2bool(listp(func)));

	assert(_var2bool(equal(num, number(0))));
	assert(_var2bool(equal(str, string("string"))));
	assert(_var2bool(equal(sym, symbol("symbol"))));
	assert(_var2bool(equal(q, quote(symbol("symbol")))));
	assert(_var2bool(equal(_nil, nil())));
	assert(_var2bool(equal(c, cons(number(0),
				       cons(string("string"), nil())))));
	assert(_var2bool(equal(func, lambda(number(0), nil(), nil()))));

	assert(_var2bool(equal(num, num)));
	assert(_var2bool(equal(str, str)));
	assert(_var2bool(equal(sym, sym)));
	assert(_var2bool(equal(q, q)));
	assert(_var2bool(equal(_nil, _nil)));
	assert(_var2bool(equal(c, c)));
	assert(_var2bool(equal(func, func)));

	assert(!_var2bool(equal(num, string("not equal"))));
	assert(!_var2bool(equal(str, number(8))));
	assert(!_var2bool(equal(sym, nil())));
	assert(!_var2bool(equal(q, num)));
	assert(!_var2bool(equal(_nil, func)));
	assert(!_var2bool(equal(c, cons(number(0), _nil))));
	assert(!_var2bool(equal(func, lambda(number(0), nil(), number(0)))));

	assert(!_var2bool(equal(num, number(1))));
	assert(!_var2bool(equal(str, string("different string"))));
	assert(!_var2bool(equal(sym, symbol("different symbol"))));
	assert(!_var2bool(equal(q, quote(str))));
	assert(!_var2bool(equal(_nil, number(1))));
	assert(!_var2bool(equal(c, cons(number(0), cons(sym, nil())))));
	assert(!_var2bool(equal(func, nil())));
}
