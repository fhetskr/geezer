#ifndef GVALUE_H
#define GVALUE_H
#include "gzr_value.h"
#endif

enum gzr_operator {OP_NOT, OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_MOD,
	OP_DOT, OP_ASSIGN, OP_EQUAL, OP_NOTEQUAL, OP_GT, OP_LT, OP_GTE, OP_LTE,
	OP_AND, OP_OR, OP_IN, OP_INCREMENT, OP_DECREMENT} typedef gzr_op;


gvalue *gzr_binary_op(gzr_op op_code, gvalue *a, gvalue* b);
gvalue *gzr_unary_op(gzr_op op_code, int is_postop, gvalue *a);

gvalue *gzr_add(gvalue *a, gvalue *b);