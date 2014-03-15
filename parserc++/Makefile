# Makefile

OBJS	= lex.o parse.o main.o
CC		= g++
CFLAGS	= -std=c++11

all: pddl_3-1-te

pddl_3-1-te:	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o pddl_3-1-te

lex.o:		scanner/lex.cc
		$(CC) $(CFLAGS) -c scanner/lex.cc -o lex.o

parse.o:	parser/parse.cc
		$(CC) $(CFLAGS) -c parser/parse.cc -o parse.o

main.o:		main.cc
		$(CC) $(CFLAGS) -c main.cc -o main.o

clean:
	rm -f *.o *~ parser/*.o parser/*~ scanner/*.o scanner/*~

mrproper:	clean
	rm -f pddl_3-1-te

#do not remove Scanner.h !
cleanall:	mrproper
	rm -f parser/parse.cc parser/Parserbase.h  scanner/lex.cc scanner/Scannerbase.h

bison:
	cd parser && bisonc++ -H'../headers.h' grammar && cd ..

flex:
	cd scanner && flexc++ lexer && cd ..

addFlexLib:
	echo '#include "../parser/Parserbase.h"' >> scanner/Scanner.ih

reset: cleanall bison flex addFlexLib pddl_3-1-te
