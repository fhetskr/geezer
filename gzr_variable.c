#include "include/gzr_variable.h"

gvar *gvar_create(char *id) {
	gvar *new_var = malloc(sizeof(gvar));
	// todo: error handling
	if (new_var == NULL) {
		fprintf(stderr,
			"Fatal error: could not allocate memory for variable '%s'\n", id);
		exit(1);
	}
	new_var->id = malloc(strlen(id) + 1);
	if (new_var->id == NULL) {
		fprintf(stderr,
			"Fatal error: could not allocate memory for variable '%s'\n", id);
		exit(1);
	}
	new_var->contents = NULL;
	
	return new_var;
}

void gvar_set(gvar *src, gvalue *new_value) {
	deref(src->contents);
	
	// set to new value
	src->contents = new_value;
	// increase ref count if necessary
	if (new_value && new_value->type == GZR_REF)
		new_value->ref_count++;
}

void gvar_destroy(gvar *src) {
	gvar_set(src, NULL);
	free(src->id);
	free(src);
}