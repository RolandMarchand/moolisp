#pragma once

#include "common.h"
#include "var.h"

struct var *eval(struct env *env, struct var *ast);
