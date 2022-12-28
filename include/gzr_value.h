#include "cstd.h"

enum gzr_type {GZR_VOID, GZR_NULL, GZR_INTEGER, GZR_DOUBLE, GZR_BYTE, GZR_BOOL, GZR_REF};

union Value {
		int i;
		unsigned char b;
		double d;
		void *ref;
};

struct gzr_value {
	union Value value;
	size_t size;
	enum gzr_type type;
	int ref_count;
} typedef gvalue;


gvalue *new_gvalue();
gvalue *gwrap_null();
gvalue *gwrap_void();
gvalue *gwrap_int(int value);
gvalue *gwrap_double(double value);
gvalue *gwrap_byte(unsigned char value);
gvalue *gwrap_bool(unsigned char value);
gvalue *gwrap_object(void *ref, size_t size);
gvalue *gwrap_string(char *str);
gvalue *copy_gval(gvalue *gval);
gvalue *deepcopy_gval(gvalue *gval);
void deref(gvalue *gval); // NOT THREAD SAFE!!
void destroy_gvalue(gvalue *gval);


void print_gvalue(gvalue *gval);