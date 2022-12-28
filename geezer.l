%{
	#define yyterminate() return END
	#include "include/gzr_operator.h"
	#include "geezer.tab.h"
	
	int linenum = 0;
%}

%%

[0-9]+\.[0-9]+ { yylval.val = gwrap_double(atof(yytext)); return DECIMAL; }
[0-9]+ { yylval.val = gwrap_int(atoi(yytext)); return INTEGER; }
\".+\" { yylval.val = gwrap_string(strdup(yytext)); return STRING; }
	/*this comment is here for my sanity since I use C syntax highlighting"*/

"true" { yylval.val = gwrap_bool(1); return BOOL; }
"false" { yylval.val = gwrap_bool(0); return BOOL; }
"null" { yylval.val = gwrap_null(); return NULL_T; }

"class" { return CLASS_DEF; }
"def" { return FUN_DEF; }
"if" { return IF; }
"else" { return ELSE; }
"elif" { return ELIF; }
"for" { return FOR; }
"while" { return WHILE; }
"break" { return BREAK; }
"continue" { return CONT; }
"pass" { return NOP; }

"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
"{" { return LCB; }
"}" { return RCB; }
"," { return COMMA; }
";" { return SEMICOLON; }
":" { return COLON; }

"IN" { yylval.val = gwrap_int(OP_IN); return IN; }
"not" { yylval.val = gwrap_int(OP_NOT); return NOT; }
"and" { yylval.val = gwrap_int(OP_AND); return AND; }
"or" { yylval.val = gwrap_int(OP_OR); return OR; }
"--" { yylval.val = gwrap_int(OP_DECREMENT); return DECREMENT; }
"++" { yylval.val = gwrap_int(OP_INCREMENT); return INCREMENT; }
"+" { yylval.val = gwrap_int(OP_PLUS); return PLUS; }
"-" { yylval.val = gwrap_int(OP_MINUS); return MINUS; }
"*" { yylval.val = gwrap_int(OP_MULT); return MULT; }
"/" { yylval.val = gwrap_int(OP_DIV); return DIV; }
"%" { yylval.val = gwrap_int(OP_MOD); return MOD; }
"." { yylval.val = gwrap_int(OP_DOT); return DOT; }
"==" { yylval.val = gwrap_int(OP_EQUAL); return EQUAL; }
"!=" { yylval.val = gwrap_int(OP_NOTEQUAL); return NOT_EQUAL; }
">=" { yylval.val = gwrap_int(OP_GTE); return GTE; }
"<=" { yylval.val = gwrap_int(OP_LTE); return LTE; }
">" { yylval.val = gwrap_int(OP_GT); return GT; }
"<" { yylval.val = gwrap_int(OP_LT); return LT; }
"=" { yylval.val = gwrap_int(OP_ASSIGN); return ASSIGN; }

[a-zA-Z_]+[a-zA-Z_0-9]* {
	char *str = strdup(yytext);
	yylval.val = gwrap_object(str, strlen(str) + 1);
	return ID;
}

[\n] { linenum++; }

%%
