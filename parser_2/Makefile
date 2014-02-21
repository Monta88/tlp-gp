my.tab.c my.tab.h: my.y
	bison -d my.y

lex.yy.c: my.l my.tab.h
	flex my.l

my: lex.yy.c my.tab.c my.tab.h
	g++ my.tab.c lex.yy.c -lfl -o my

all:
	bison -d my.y
	flex my.l
	g++ my.tab.c lex.yy.c -lfl -o my

clean:
	rm -rf *.o my *.c *.h
