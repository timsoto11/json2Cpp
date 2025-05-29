CC = gcc
CPP = g++
CFLAGS = -I./ast/

%.o: %.c 
	$(CC) -c -std=gnu11 -o $@ $< $(CFLAGS)

%.o: %.cpp 
	$(CPP) -c -std=c++17 -o $@ $< $(CFLAGS)

all: jsonSchema

parser.o: ast/scanner.c ast/parser.c

ast/scanner.c: ast/scanner.l
	flex --outfile=ast/scanner.c ast/scanner.l

ast/parser.c: ast/parser.y
	bison -d --defines=ast/parser.h --output=ast/parser.c ast/parser.y

jsonSchema: ast/parser.o ast/scanner.o ast/ast.o cppGen.o main.o
	$(CPP) $^ -o $@

clean:
	rm -f *.o **/*.o ast/parser.c ast/parser.h scanner.c jsonSchema 