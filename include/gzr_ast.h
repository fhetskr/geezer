#ifndef GLIST_H
#define GLIST_H
#include "gzr_list.h"
#endif

enum exec_type {ENTRY, NOOP, BRK, CNT, BLOCK, IFSTMT, ELIFSTMT,
	FORSTMT, WHILESTMT, CLASSDEF, FUNDEF, LISTHEAD, PARAMLIST,
	IDENT, LITERAL, PREOP, POSTOP, BINOP, INDEX, FUNCALL, LIBCALL};

char *exec_type_name(enum exec_type type);

struct gzr_exec_node {
	enum exec_type op_code;
	struct gzr_exec_node *parent;
	struct gzr_exec_node *sibling;
	struct gzr_exec_node *child;
	gvalue *data;
	int linenum;
} typedef exec_node;

exec_node *new_exec_node(enum exec_type op_code, gvalue *data, int linenum);
exec_node *new_AST(int linenum);

void add_child(exec_node *parent, exec_node *child);
exec_node *new_sibling(exec_node *old_node, exec_node *new_node);

void print_AST(exec_node *entry);

void destroy_exec_node(exec_node *node);
void destroy_AST(exec_node *entry);