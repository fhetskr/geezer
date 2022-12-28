#include "include/gzr_interpreter.h"

void gzr_eval(gscope *symtable, exec_node *AST) {
	switch (AST->op_code) {
		case ENTRY:
			gzr_eval(symtable, AST->child);
			return;
		case NOOP:
			break;
		case LITERAL:
			deref(symtable->accumulator);
			symtable->accumulator = AST->data;
			break;
		case BINOP:
			gzr_eval(symtable, AST->child);
			gvalue *param1 = copy_gval(symtable->accumulator);
			gzr_eval(symtable, AST->child->sibling);
			gvalue *param2 = copy_gval(symtable->accumulator);
			
			symtable->accumulator = gzr_binary_op(AST->data->value.i, param1, param2);
			print_gvalue(symtable->accumulator);
			printf("\n");
			deref(param1);
			deref(param2);
			
	}
	// execute sibling node conditionally based on parent node type
	switch (AST->parent->op_code) {
		case ENTRY:
			deref(symtable->accumulator);
			gzr_eval(symtable, AST->sibling);
	}
}