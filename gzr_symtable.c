#include "include/gzr_symtable.h"

gscope *init_call_stack() {
	gscope *new_stack = malloc(sizeof(gscope));
	if (new_stack == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory for an internal value\n");
		exit(1);
	}
	
	new_stack->name = strdup("global");
	new_stack->last = NULL;
	new_stack->id = 0;
	new_stack->variables = new_glist();
	
	return new_stack;
}

void push_call_stack(gscope *stack_top, char *name) {
	gscope *new_stack = malloc(sizeof(gscope));
	if (new_stack == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory for an internal value\n");
		exit(1);
	}
	
	new_stack->name = strdup(name);
	new_stack->last = stack_top;
	new_stack->id = stack_top->id + 1;
	new_stack->variables = new_glist();
}

gscope *pop_call_stack(gscope *stack_top) {
	gscope *new_top = stack_top->last;
	free(stack_top->name);
	destroy_list(stack_top->variables);
	free(stack_top);
	
	return new_top;
}

void print_call_stack(gscope *stack_top) {
	if (stack_top == NULL) return;
	print_call_stack(stack_top->last);
	printf("Scope %s (%d)\n", stack_top->name, stack_top->id);
}

void destroy_call_stack(gscope *stack_top) {
	while (stack_top)
		stack_top = pop_call_stack(stack_top);
}

gvar *search_scope(gscope *scope, char *id) {
	struct gzr_node *cur_node = scope->variables->head;
	while (cur_node) {
		if (!strcmp(((gvar *)cur_node->contents->value.ref)->id, id)) {
			return (gvar *)cur_node->contents->value.ref;
		}
		cur_node = cur_node->next;
	}

	return NULL;
}

void new_var(gscope *scope, char *id, gvalue *val) {
	gvar *new_var = gvar_create(id);
	gvar_set(new_var, val);
	gappend(scope->variables, gwrap_object(new_var, sizeof (gvar)));
}

void destroy_var(gscope *scope, char *id) {
	struct gzr_node *cur_node = scope->variables->head;
	while (cur_node) {
		if (!strcmp(((gvar *)cur_node->contents->value.ref)->id, id)) {
			destroy_node(cur_node);
			return;
		}
		cur_node = cur_node->next;
	}
}