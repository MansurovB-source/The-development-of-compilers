NAME=compiler.out
CC=gcc
CC_FLAGS=

all: build

build: parser.tab.c lexer.yy.c util.o
	$(CC) $(CC_FLAGS) -o $(NAME) parser.tab.c lex.yy.c util.o -lfl

parser.tab.c:
	bison -d -o parser.tab.c parser.y

lexer.yy.c:
	flex -o lex.yy.c lexer.flex

util.o:
	$(CC) $(CC_FLAFS)-c -o util.o ../src/util.c

clean:
	rm -f *.yy.c
	rm -f *.tab.*
	rm -f *.o
	rm -f compiler.out