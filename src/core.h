#include "common.h"
#include "var.h"

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
struct var *eq(const struct var *list);
struct var *equal(const struct var *list);
struct var *length(const struct var *list);
struct var *apply(const struct var *list);
