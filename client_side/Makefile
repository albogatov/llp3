all: include/ast.h src/ast.c include/output.h src/output.c include/string_utils.h src/string_utils.c parser.y tokenizer.l
	make clean
	make aql_parser
	make run

aql_parser: src/ast.c src/output.c src/string_utils.c
	flex tokenizer.l
	bison -d parser.y
	gcc -g -o $@ parser.tab.c lex.yy.c $^

run:
	./aql_parser

clean:
	rm -f aql_parser lex.yy.c parser.tab.c parser.tab.h

