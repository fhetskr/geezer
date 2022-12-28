#include "include/gzr_value.h"

gvalue *new_gvalue() {
	gvalue *new_value = malloc(sizeof(gvalue));
	if (new_value == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory for an internal value\n");
		exit(1);
	}
	
	new_value->ref_count = 0;
	new_value->type = GZR_VOID;
	
	return new_value;
}

gvalue *gwrap_null() {
	gvalue *new_value = new_gvalue();
	new_value->value.ref = NULL;
	new_value->size = sizeof(void *);
	new_value->type = GZR_NULL;
	return new_value;
}

gvalue *gwrap_void() {
	gvalue *new_value = gwrap_null();
	new_value->type = GZR_VOID;
	return new_value;
}

gvalue *gwrap_int(int value) {
	gvalue *new_value = new_gvalue();
	new_value->value.i = value;
	new_value->size = sizeof(int);
	new_value->type = GZR_INTEGER;
	return new_value;
}

gvalue *gwrap_double(double value) {
	gvalue *new_value = new_gvalue();
	new_value->value.d = value;
	new_value->size = sizeof(double);
	new_value->type = GZR_DOUBLE;
	return new_value;
}

gvalue *gwrap_byte(unsigned char value) {
	gvalue *new_value = new_gvalue();
	new_value->value.b = value;
	new_value->size = sizeof(unsigned char);
	new_value->type = GZR_BYTE;
	return new_value;
}

gvalue *gwrap_bool(unsigned char value) {
	gvalue *new_value = new_gvalue();
	new_value->value.b = (value) ? 1 : 0;
	new_value->size = sizeof(unsigned char);
	new_value->type = GZR_BOOL;
	return new_value;
}

gvalue *gwrap_object(void *ref, size_t size) {
	gvalue *new_value = new_gvalue();
	new_value->value.ref = ref;
	new_value->size = size;
	new_value->type = GZR_REF;
	return new_value;
}

gvalue *gwrap_string(char *str) {
	return gwrap_object(strdup(str), strlen(str) + 1);
}

gvalue *copy_gval(gvalue *gval) {
	gvalue *new_gval = new_gvalue();
	new_gval->value = gval->value;
	new_gval->size = gval->size;
	new_gval->type = gval->type;
	new_gval->ref_count = ++(gval->ref_count);
	
	return new_gval;
}

gvalue *deepcopy_gval(gvalue *gval) {
	gvalue *new_gval = new_gvalue();
	new_gval->value.ref = malloc(gval->size);
	if (new_gval->value.ref == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory for an internal value\n");
		exit(1);
	}
	memcpy(new_gval->value.ref, gval->value.ref, gval->size);
	
	new_gval->size = gval->size;
	new_gval->type = gval->type;
	new_gval->ref_count = 1;
	
	return new_gval;
}

void deref(gvalue *gval) {
	if (gval == NULL) return;
	gval->ref_count--;
	// should never go below zero, but just in case something
	// ungodly happens, we use <=
	if (gval->ref_count <= 0) {
		destroy_gvalue(gval);
	}
}

void destroy_gvalue(gvalue *gval) {
	if (gval == NULL) return;
	if (gval->type == GZR_REF)
		free(gval->value.ref);
	free(gval);
}

void print_gvalue(gvalue *gval) {
	switch (gval->type) {
		case GZR_VOID:
			printf("VOID");
		case GZR_NULL:
			printf("NULL");
			break;
		case GZR_INTEGER:
			printf("%d", gval->value.i);
			break;
		case GZR_DOUBLE:
			printf("%f", gval->value.d);
			break;
		case GZR_BOOL:
			printf("%s", (gval->value.i) ? "true" : "false");
			break;
		case GZR_REF:
			printf("<object at 0x%x>", gval->value.ref);
			break;
	}
}