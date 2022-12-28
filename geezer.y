%{
    #include "include/gzr_interpreter.h"
    int yylex();
    int yyerror();
    
    extern int linenum;
    
    exec_node *global_entry;
    exec_node *current_node;
%}

%union {
    gvalue *val;
    struct gzr_exec_node *enode;
}
%start program
%token CLASS_DEF FUN_DEF IF ELSE ELIF FOR WHILE
%token LP RP LB RB LCB RCB COMMA SEMICOLON COLON
%token <val> IN NOT AND OR EQUAL NOT_EQUAL GTE LTE GT LT
%token <val> PLUS MINUS MULT DIV MOD DOT ASSIGN DECREMENT INCREMENT
%left COMMA
%left IN
%left ASSIGN
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left GT GTE
%left LT LTE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT
%right INCREMENT DECREMENT
%left DOT
%left LB RB
%left LP RP
%token <val> NULL_T INTEGER DECIMAL BOOL ID STRING
%token <enode> BREAK CONT NOP
%type <enode> statement nondefining_statement nondef_statement_list
%type <enode> class_definition class_body class_statement
%type <enode> function_definition identifier_list param_list
%type <enode> if_statement elif_list else_clause
%type <enode> while_loop for_loop code_block expression
%token END 0 "end of file"
%expect 4
%%

program: statement_list END
       | END
       ;

statement_list: statement_list statement {
                current_node = new_sibling(current_node, $2);
              }
              | statement {
                add_child(global_entry, $1);
                current_node = $1;
              }
              ;

statement: nondefining_statement
         | class_definition
         | function_definition
         ;

nondef_statement_list: nondef_statement_list nondefining_statement {
                        new_sibling($1, $2);
                     }
                     | nondefining_statement {
                        $$ = $1;
                     }
                     ;

nondefining_statement: if_statement
                     | for_loop
                     | while_loop
                     | expression SEMICOLON
                     | BREAK SEMICOLON {
                        $$ = new_exec_node(BRK, NULL, linenum);
                     }
                     | CONT SEMICOLON {
                        $$ = new_exec_node(CNT, NULL, linenum);
                     }
                     | NOP SEMICOLON {
                        $$ = new_exec_node(NOOP, NULL, linenum);
                     }
                     ;

class_definition: CLASS_DEF ID LCB class_body RCB
                ;

class_body: class_body class_statement
          | class_statement
          ;

class_statement: function_definition
               | nondefining_statement
               ;

function_definition: FUN_DEF ID LP identifier_list RP code_block
                   ;

identifier_list: identifier_list COMMA ID
               | ID
               |
               ;

code_block: LCB nondef_statement_list RCB {
            $$ = new_exec_node(BLOCK, NULL, linenum);
            add_child($$, $2);
          }
          | COLON nondefining_statement {
            $$ = new_exec_node(BLOCK, NULL, linenum);
            add_child($$, $2);
          }
          ;

if_statement: IF expression code_block else_clause {
                $$ = new_exec_node(IFSTMT, NULL, linenum);
                add_child($$, $2);
                add_child($$, $3);
                add_child($$, $4);
            }
            ;

else_clause: elif_list ELSE code_block { new_sibling($1, $3); $$ = $1; }
           | ELSE code_block { $$ = $2; }
           | elif_list
           | { $$ = new_exec_node(NOOP, NULL, linenum); }
           ;

elif_list: elif_list ELIF expression code_block {
            exec_node *new_node = new_sibling($$,
                new_exec_node(ELIFSTMT, NULL, linenum));
            add_child(new_node, $3);
            add_child(new_node, $4);
         }
         | ELIF expression code_block {
            $$ = new_exec_node(ELIFSTMT, NULL, linenum);
            add_child($$, $2);
            add_child($$, $3);
         }
         ;

for_loop: FOR ID IN expression code_block {
            $$ = new_exec_node(FORSTMT, NULL, linenum);
            add_child($$, new_exec_node(IDENT, $<val>2, linenum));
            add_child($$, $4);
            add_child($$, $5);
        }
        ;

while_loop: WHILE expression code_block {
            $$ = new_exec_node(WHILESTMT, NULL, linenum);
            add_child($$, $2);
            add_child($$, $3);
          }
          ;
    
expression: ID { $$ = new_exec_node(IDENT, $<val>1, linenum); }
          | expression LB expression RB
          { $$ = new_exec_node(IDENT, NULL, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | INTEGER { $$ = new_exec_node(LITERAL, $<val>1, linenum); }
          | DECIMAL { $$ = new_exec_node(LITERAL, $<val>1, linenum); }
          | BOOL { $$ = new_exec_node(LITERAL, $<val>1, linenum); }
          | NULL_T { $$ = new_exec_node(LITERAL, $<val>1, linenum); }
          | STRING { $$ = new_exec_node(LITERAL, $<val>1, linenum); }
          | expression LP param_list RP
          {
            $$ = new_exec_node(FUNCALL, NULL, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | LP expression RP { $$ = $2; }
          | expression INCREMENT
          {
            $$ = new_exec_node(POSTOP, $2, linenum);
            add_child($$, $1);
          }
          | expression DECREMENT
          {
            $$ = new_exec_node(POSTOP, $2, linenum);
            add_child($$, $1);
          }
          | NOT expression
          {
            $$ = new_exec_node(PREOP, $1, linenum);
            add_child($$, $2);
          }
          | MINUS expression
          {
            $$ = new_exec_node(PREOP, $1, linenum);
            add_child($$, $2);
          }
          | PLUS expression
          {
            $$ = new_exec_node(PREOP, $1, linenum);
            add_child($$, $2);
          }
          | INCREMENT expression
          {
            $$ = new_exec_node(PREOP, $1, linenum);
            add_child($$, $2);
          }
          | DECREMENT expression
          {
            $$ = new_exec_node(PREOP, $1, linenum);
            add_child($$, $2);
          }
          | expression PLUS expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression MINUS expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression MULT expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression DIV expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression MOD expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression DOT expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression ASSIGN expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression EQUAL expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression NOT_EQUAL expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression GT expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression LT expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression GTE expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression LTE expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression AND expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression OR expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          | expression IN expression
          {
            $$ = new_exec_node(BINOP, $2, linenum);
            add_child($$, $1);
            add_child($$, $3);
          }
          ;

param_list: param_list COMMA expression {
            add_child($$, $3);
          }
          | expression {
            $$ = new_exec_node(PARAMLIST, NULL, linenum);
            add_child($$, $1);
          }
          | { $$ = new_exec_node(NOOP, NULL, linenum); }
          ;

%%

int yyerror(char *msg) {
    fprintf(stderr, "%s\n", msg);
    return 1;
}

int main() {
    
    global_entry = new_AST(0);
    current_node = global_entry;
  
    yyparse();
    
    // debug: print AST data structure
    //print_AST(global_entry);
    
    // run the program
    gscope *global_scope = init_call_stack();
    gzr_eval(global_scope, global_entry);
    
    //clean up
    destroy_call_stack(global_scope);
    destroy_AST(global_entry);
    return 0;
}