# Makefile

OBJS	= lex.o parse.o main.o
CC		= g++
CFLAGS	= -std=c++11 -Wall

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

cleanall:	mrproper
	rm -f parser/parse.cc parser/Parser.h parser/Parser.ih parser/Parserbase.h scanner/lex.cc scanner/Scanner.h scanner/Scanner.ih scanner/Scannerbase.h

bison:
	cd parser && bisonc++ grammar && cd ..

flex:
	cd scanner && flexc++ --print lexer && cd ..

