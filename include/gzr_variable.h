#ifndef GVALUE_H
#define GVALUE_H
#include "gzr_value.h"
#endif

struct gzr_variable {
	char *id;
	gvalue *contents;
} typedef gvar;

gvar *gvar_create(char *id);
void gvar_set(gvar *src, gvalue *new_value);
void gvar_destroy(gvar *src);