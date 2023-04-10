#include "common.h"
#include "var.h"

struct var *begin(struct env *env, const struct var *list);
struct var *curried_cons(const struct var *list);
struct var *curried_car(const struct var *list);
struct var *curried_cdr(const struct var *list);
struct var *add(const struct var *list);
struct var *substract(const struct var *list);
struct var *multiply(const struct var *list);
struct var *divide(const struct var *list);
struct var *number_equal(const struct var *list);
struct var *lesser_than(const struct var *list);
struct var *greater_than(const struct var *list);
struct var *lesser_equal(const struct var *list);
struct var *greater_equal(const struct var *list);
struct var *print(const struct var *v);
struct var *curried_print(const struct var *list);
struct var *eq(const struct var *list);
struct var *equal(const struct var *list);
struct var *length(const struct var *list);
struct var *apply(const struct var *list);
struct var *curried_apply(const struct var *list);
struct var *curried_length(const struct var *list);
