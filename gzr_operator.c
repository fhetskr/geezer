#include "include/gzr_operator.h"

gvalue *gzr_binary_op(gzr_op op_code, gvalue *a, gvalue* b) {
	if (a->type == GZR_BYTE) {
		if (b->type == GZR_BYTE) {
			return gwrap_byte(GZR_OPERATE_BINARY(op_code, a->value.b, b->value.b));
		} else if (b->type == GZR_INTEGER) {
			return gwrap_int(GZR_OPERATE_BINARY(op_code, (int) a->value.b + b->value.i));
		} else if (b->type == GZR_DOUBLE) {
			return gwrap_double((double) a->value.b + b->value.d);
		}
	} else if (a->type == GZR_INTEGER) {
		if (b->type == GZR_INTEGER) {
			return gwrap_int(a->value.i + b->value.i);
		} else if (b->type == GZR_BYTE) {
			return gwrap_int(a->value.i + (int) b->value.b);
		} else if (b->type == GZR_DOUBLE) {
			return gwrap_double((double) a->value.i + b->value.d);
		}
	} else if (a->type == GZR_DOUBLE) {
		if (b->type == GZR_DOUBLE) {
			return gwrap_double(a->value.d + b->value.d);
		} else if (b->type == GZR_BYTE) {
			return gwrap_double(a->value.d + (double) b->value.b);
		} else if (b->type == GZR_INTEGER) {
			return gwrap_double(a->value.d + (double) b->value.i);
		}
	}
	// fails if previous conditions not triggered
	return gwrap_void();
}