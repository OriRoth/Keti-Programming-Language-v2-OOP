#ifndef NATIVE_H_
#define NATIVE_H_

#include "memory.h"

memptr func_plus(memptr func_expr);
memptr func_minus(memptr func_expr);
memptr func_mult(memptr func_expr);
memptr func_div(memptr func_expr);
memptr func_equals(memptr func_expr);
memptr func_smaller(memptr func_expr);
memptr func_greater(memptr func_expr);
memptr func_cons(memptr func_expr);
memptr func_car(memptr func_expr);
memptr func_cdr(memptr func_expr);
memptr func_nil(memptr func_expr);
memptr func_true(memptr func_expr);
memptr func_cond(memptr func_expr);
memptr func_setq(memptr func_expr);
memptr func_defun(memptr func_expr);
memptr func_or(memptr func_expr);
memptr func_and(memptr func_expr);
memptr func_xor(memptr func_expr);
memptr func_list(memptr func_expr);
memptr func_value(memptr func_expr);
memptr func_assign(memptr func_expr);
memptr func_supps(memptr func_expr);

#endif /* NATIVE_H_ */
