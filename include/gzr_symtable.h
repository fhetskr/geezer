#ifndef GLIST_H
#define GLIST_H
#include "gzr_list.h"
#endif
#include "gzr_variable.h"


struct gzr_scope {
	struct gzr_scope *last;
	char *name;
	int id;
	gvalue *accumulator;
	glist *variables;
} typedef gscope;

gscope *init_call_stack();
void push_call_stack(gscope *stack_top, char *name);
gscope *pop_call_stack(gscope *stack_top);
void print_call_stack(gscope *stack_top);
void destroy_call_stack(gscope *stack_top);
gvar *search_scope(gscope *scope, char *id);
void new_var(gscope *stack_top, char *id, gvalue *val);
void destroy_var(gscope *stack_top, char *id);