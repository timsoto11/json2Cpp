CC = gcc
CPP = g++
CFLAGS =

%.o: %.c 
	$(CC) -c -std=gnu11 -o $@ $< $(CFLAGS)

%.o: %.cpp 
	$(CPP) -c -std=c++17 -o $@ $< $(CFLAGS)

all: jsonSchema

parser.o: scanner.c

scanner.c: scanner.l
	flex --outfile=scanner.c scanner.l

parser.c: parser.y
	bison -d --defines=parser.h --output=parser.c parser.y

jsonSchema: parser.o scanner.o ast.o cppGen.o main.o
	$(CPP) $^ -o $@

clean:
	rm -f *.o parser.c parser.h scanner.c jsonSchema 