
jsonSchema:  jsonSchema.l jsonSchema.y
	bison -d jsonSchema.y
	flex jsonSchema.l
	gcc jsonSchema.tab.c lex.yy.c ast.c -lfl -o $@

clean:
	rm -f jsonSchema.tab.c jsonSchema.tab.h lex.yy.c jsonSchema testOutput.txt