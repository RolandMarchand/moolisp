#include "common.h"
#include "var.h"
#include "env.h"

tgc_t gc;
struct env *dummy_env;

void _main(void);
void test1();
void test2();
struct var *dummy_func(struct var *v) {return v;}
struct var *dummy_func2(struct var *v) {return v;}

int main(void)
{
	volatile void *dummy_var;
	tgc_start(&gc, &dummy_var);
	_main();
	tgc_stop(&gc);
}

void _main(void)
{
	dummy_env = env_make(NULL, nil(), nil());
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

	struct var *params = cons(symbol("x"), cons(symbol("y"), nil()));
	struct var *body = cons(symbol("+"),
				cons(symbol("x"),
				     cons(symbol("y"), nil())));
	struct var *cl = closure(dummy_env, params, body);
	assert(cl->type == VAR_CLOSURE);
	assert(cl->as.closure->params == params);
	assert(cl->as.closure->body == body);

	struct var *func = c_function(dummy_func);
	assert(func->type == VAR_C_FUNCTION);
	assert(func->as.c_function == dummy_func);
}

void test2()
{
	struct var *num = number(0);
	struct var *str = string("string");
	struct var *sym = symbol("symbol");
	struct var *q = quote(sym);
	struct var *_nil = nil();
	struct var *c = cons(num, cons(str, _nil));
	struct var *cl = closure(dummy_env, nil(), nil());
	struct var *func = c_function(dummy_func);

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
	assert(_var2bool(atom(cl)));
	assert(_var2bool(atom(func)));

	assert(_var2bool(numberp(num)));
	assert(!_var2bool(numberp(str)));
	assert(!_var2bool(numberp(sym)));
	assert(!_var2bool(numberp(q)));
	assert(!_var2bool(numberp(_nil)));
	assert(!_var2bool(numberp(c)));
	assert(!_var2bool(numberp(cl)));
	assert(!_var2bool(numberp(func)));

	assert(!_var2bool(stringp(num)));
	assert(_var2bool(stringp(str)));
	assert(!_var2bool(stringp(sym)));
	assert(!_var2bool(stringp(q)));
	assert(!_var2bool(stringp(_nil)));
	assert(!_var2bool(stringp(c)));
	assert(!_var2bool(stringp(cl)));
	assert(!_var2bool(stringp(func)));

	assert(!_var2bool(symbolp(num)));
	assert(!_var2bool(symbolp(str)));
	assert(_var2bool(symbolp(sym)));
	assert(!_var2bool(symbolp(q)));
	assert(!_var2bool(symbolp(_nil)));
	assert(!_var2bool(symbolp(c)));
	assert(!_var2bool(symbolp(cl)));
	assert(!_var2bool(symbolp(func)));

	assert(!_var2bool(functionp(num)));
	assert(!_var2bool(functionp(str)));
	assert(!_var2bool(functionp(sym)));
	assert(!_var2bool(functionp(q)));
	assert(!_var2bool(functionp(_nil)));
	assert(!_var2bool(functionp(c)));
	assert(_var2bool(functionp(cl)));
	assert(_var2bool(functionp(func)));

	assert(!_var2bool(nilp(num)));
	assert(!_var2bool(nilp(str)));
	assert(!_var2bool(nilp(sym)));
	assert(!_var2bool(nilp(q)));
	assert(_var2bool(nilp(_nil)));
	assert(!_var2bool(nilp(c)));
	assert(!_var2bool(nilp(cl)));
	assert(!_var2bool(nilp(func)));

	assert(_var2bool(not(nilp(num))));
	assert(_var2bool(not(nilp(str))));
	assert(_var2bool(not(nilp(sym))));
	assert(_var2bool(not(nilp(q))));
	assert(!_var2bool(not(nilp(_nil))));
	assert(_var2bool(not(nilp(c))));
	assert(_var2bool(not(nilp(cl))));
	assert(_var2bool(not(nilp(func))));

	assert(_var2bool(eq(num, num)));
	assert(!_var2bool(eq(str, num)));
	assert(!_var2bool(eq(sym, num)));
	assert(_var2bool(eq(q, q)));
	assert(!_var2bool(eq(_nil, c)));
	assert(!_var2bool(eq(c, _nil)));
	assert(_var2bool(eq(cl, cl)));
	assert(_var2bool(eq(func, func)));

	assert(!_var2bool(listp(num)));
	assert(!_var2bool(listp(str)));
	assert(!_var2bool(listp(sym)));
	assert(_var2bool(listp(q)));
	assert(_var2bool(listp(_nil)));
	assert(_var2bool(listp(c)));
	assert(!_var2bool(listp(cl)));
	assert(!_var2bool(listp(func)));

	assert(_var2bool(equal(num, number(0))));
	assert(_var2bool(equal(str, string("string"))));
	assert(!_var2bool(equal(sym, symbol("symbol"))));
	assert(!_var2bool(equal(q, quote(symbol("symbol")))));
	assert(_var2bool(equal(_nil, nil())));
	assert(_var2bool(equal(c, cons(number(0),
				       cons(string("string"), nil())))));
	assert(!_var2bool(equal(cl, closure(dummy_env, nil(), nil()))));
	assert(_var2bool(equal(func, c_function(dummy_func))));

	assert(_var2bool(equal(num, num)));
	assert(_var2bool(equal(str, str)));
	assert(_var2bool(equal(sym, sym)));
	assert(_var2bool(equal(q, q)));
	assert(_var2bool(equal(_nil, _nil)));
	assert(_var2bool(equal(c, c)));
	assert(_var2bool(equal(cl, cl)));
	assert(_var2bool(equal(func, func)));

	assert(!_var2bool(equal(num, string("not equal"))));
	assert(!_var2bool(equal(str, number(8))));
	assert(!_var2bool(equal(sym, nil())));
	assert(!_var2bool(equal(q, num)));
	assert(!_var2bool(equal(_nil, func)));
	assert(!_var2bool(equal(c, cons(number(0), _nil))));
	assert(!_var2bool(equal(cl, closure(dummy_env, t(), t()))));
	assert(!_var2bool(equal(func, c_function(dummy_func2))));

	assert(!_var2bool(equal(num, number(1))));
	assert(!_var2bool(equal(str, string("different string"))));
	assert(!_var2bool(equal(sym, symbol("different symbol"))));
	assert(!_var2bool(equal(q, quote(str))));
	assert(!_var2bool(equal(_nil, number(1))));
	assert(!_var2bool(equal(c, cons(number(0), cons(sym, nil())))));
	assert(!_var2bool(equal(cl, t())));
	assert(!_var2bool(equal(func, nil())));
}
