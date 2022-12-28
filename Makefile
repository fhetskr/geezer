libs := gzr_value.c gzr_variable.c gzr_list.c gzr_ast.c gzr_symtable.c gzr_interpreter.c gzr_operator.c

geezer:		lex.yy.c geezer.tab.c geezer.tab.h
	gcc -g -o geezer geezer.tab.c lex.yy.c $(libs) -ly -ll
	rm -rf geezer.tab.* lex.yy.*
	
lex.yy.c:	geezer.l
	flex geezer.l

geezer.tab.c:	geezer.y
	bison -d geezer.y

libraries:	gzr_list.c gzr_object.c gzr_variable.c
	gcc -Wall -c $(libs)

clean:
	rm -rf geezer