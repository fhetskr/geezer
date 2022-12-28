#include "include/gzr_ast.h"

char *exec_type_name(enum exec_type type) {
	switch (type) {
		case ENTRY:
			return "entrypoint";
		case NOOP:
			return "no-op";
		case BRK:
			return "break";
		case CNT:
			return "continue";
		case BLOCK:
			return "code block";
		case IFSTMT:
			return "if";
		case ELIFSTMT:
			return "elif";
		case FORSTMT:
			return "for loop";
		case WHILESTMT:
			return "while loop";
		case CLASSDEF:
			return "class definition";
		case FUNDEF:
			return "function definition";
		case LISTHEAD:
			return "list container";
		case PARAMLIST:
			return "parameter list";
		case IDENT:
			return "identifier";
		case LITERAL:
			return "literal value";
		case PREOP:
			return "prefix operator";
		case POSTOP:
			return "postfix operator";
		case BINOP:
			return "binary operator";
		case INDEX:
			return "list index accessor";
		case FUNCALL:
			return "function call";
	}
}

exec_node *new_exec_node(enum exec_type op_code, gvalue *data, int linenum) {
	exec_node *new_node = malloc(sizeof(exec_node));
	if (new_node == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory for an internal value\n");
		exit(1);
	}
	
	new_node->op_code = op_code;
	new_node->sibling = NULL;
	new_node->child = NULL;
	if (data) {
		new_node->data = data;
	} else {
		new_node->data = gwrap_null();
	}
	new_node->linenum = linenum;
	
	return new_node;
}

exec_node *new_AST(int linenum) {
	return new_exec_node(ENTRY, NULL, linenum);
}

void add_child(exec_node *parent, exec_node *child) {
	if (parent->child) {
		exec_node* sibling = parent->child;
		while (sibling->sibling)
			sibling = sibling->sibling;
		sibling->sibling = child;
	} else {
		parent->child = child;
	}
	child->parent = parent;
}

exec_node *new_sibling(exec_node *old_node, exec_node *new_node) {
	while (old_node->sibling)
		old_node = old_node->sibling;
	old_node->sibling = new_node;
	new_node->parent = old_node->parent;
	return new_node;
}

void print_AST(exec_node *entry) {
	if (entry == NULL) return;
	if (entry->parent)
		printf("child of %s:\n", exec_type_name(entry->parent->op_code));
	else
		printf("global context:\n");
	printf("OPCODE %s on line %d: ",
		exec_type_name(entry->op_code), entry->linenum);
	print_gvalue(entry->data);
	printf("\n\n");
	print_AST(entry->child);
	print_AST(entry->sibling);
}

void destroy_exec_node(exec_node *node) {
	destroy_gvalue(node->data);
	free(node);
}

void destroy_AST(exec_node *entry) {
	if (entry->child) destroy_AST(entry->child);
	if (entry->sibling) destroy_AST(entry->sibling);
	destroy_exec_node(entry);
}