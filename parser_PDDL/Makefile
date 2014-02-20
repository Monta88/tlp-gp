# Makefile

OBJS	= bison.o lex.o main.o

CC	= g++
CFLAGS	= -g -Wall -ansi

pddl_3-1:	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o pddl_3-1 -lfl

lex.o:		lex.c
		$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:		pddl_3-1.lex 
		flex pddl_3-1.lex
		cp lex.yy.c lex.c

bison.o:	bison.c
		$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:	pddl_3-1.y
		bison -d -v pddl_3-1.y
		cp pddl_3-1.tab.c bison.c
		cmp -s pddl_3-1.tab.h tok.h || cp pddl_3-1.tab.h tok.h

main.o:		main.cc
		$(CC) $(CFLAGS) -c main.cc -o main.o

lex.o yac.o main.o	: heading.h
lex.o main.o		: tok.h

clean:
	rm -f *.o *~ lex.c lex.yy.c bison.c tok.h pddl_3-1.tab.c pddl_3-1.tab.h pddl_3-1.output pddl_3-1

