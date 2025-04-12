
CFLAGS =-Wall -O3 -Wno-unused-variable -Wno-unused-parameter -Wall -Wextra -pedantic -fstack-protector-strong -Werror=format-security

%.o: %.c 
	$(CC) -c -std=gnu11 -o $@ $< $(DEFINES) $(CFLAGS)

%.o: %.cpp 
	$(CPP) -c -std=c++17 -o $@ $< $(DEFINES) $(CFLAGS)

scanner:
	flex --outfile=scanner.c scanner.l

parser: 
	bison -d --defines=parser.h --output=parser.c parser.y

jsonSchema:  scanner parser
	gcc parser.c scanner.c ast.c -lfl -o $@

clean:
	rm -f jsonSchema.tab.c jsonSchema.tab.h lex.yy.c jsonSchema testOutput.txt