#include "operators.h"

#include "mathlib.h"
#include "stack.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

double operator_add(double lhs, double rhs) {
    return lhs + rhs;
}

double operator_sub(double lhs, double rhs) {
    return lhs - rhs;
}

double operator_mul(double lhs, double rhs) {
    return lhs * rhs;
}

double operator_div(double lhs, double rhs) {
    if (rhs == 0) {
        return nan("nan");
    }
    return lhs / rhs;
}

// return false if there are fewer than 2 items on the stack. otherwise, pop
// rhs, then pop lhs, then pass them into op, then push the result. and return
// true
bool apply_binary_operator(binary_operator_fn op) {
    if (stack_size < 2) {
        return false;
    }

    double rhs;
    double lhs;
    assert(stack_pop(&rhs));
    assert(stack_pop(&lhs));
    double result = op(lhs, rhs);
    assert(stack_push(result));
    return true;
}

bool apply_unary_operator(unary_operator_fn op) {
    if (stack_size < 1) {
        return false;
    }

    double x;
    assert(stack_pop(&x));
    double result = op(x);
    assert(stack_push(result));
    return true;
}

bool parse_double(const char *s, double *d) {
    char *endptr;
    double result = strtod(s, &endptr);
    if (endptr != s) {
        *d = result;
        return true;
    } else {
        return false;
    }
}
