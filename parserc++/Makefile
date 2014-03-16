# Makefile

OBJS	= lex.o parse.o Data.o action.o atom.o attribute.o domain.o durative_action.o fluent.o interval.o lexer.o planningData.o problem.o type.o variable.o main.o
CC		= g++
CFLAGS	= -std=c++11

all: pddl_3-1-te

pddl_3-1-te:		$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o pddl_3-1-te

lex.o:				scanner/lex.cc
		$(CC) $(CFLAGS) -c scanner/lex.cc -o lex.o

parse.o:			parser/parse.cc
		$(CC) $(CFLAGS) -c parser/parse.cc -o parse.o

Data.o:				Data.cpp
		$(CC) $(CFLAGS) -c Data.cpp -o Data.o

action.o:			src/action.cpp
		$(CC) $(CFLAGS) -c src/action.cpp -o action.o

atom.o:				src/atom.cpp
		$(CC) $(CFLAGS) -c src/atom.cpp -o atom.o

attribute.o:		src/attribute.cpp
		$(CC) $(CFLAGS) -c src/attribute.cpp -o attribute.o

domain.o:			src/domain.cpp
		$(CC) $(CFLAGS) -c src/domain.cpp -o domain.o

durative_action.o:	src/durative_action.cpp
		$(CC) $(CFLAGS) -c src/durative_action.cpp -o durative_action.o

fluent.o:			src/fluent.cpp
		$(CC) $(CFLAGS) -c src/fluent.cpp -o fluent.o

interval.o:			src/interval.cpp
		$(CC) $(CFLAGS) -c src/interval.cpp -o interval.o

lexer.o:			src/lexer.cpp
		$(CC) $(CFLAGS) -c src/lexer.cpp -o lexer.o

planningData.o:		src/planningData.cpp
		$(CC) $(CFLAGS) -c src/planningData.cpp -o planningData.o

problem.o:	src/problem.cpp
		$(CC) $(CFLAGS) -c src/problem.cpp -o problem.o

type.o:	src/type.cpp
		$(CC) $(CFLAGS) -c src/type.cpp -o type.o

variable.o:	src/variable.cpp
		$(CC) $(CFLAGS) -c src/variable.cpp -o variable.o

main.o:		main.cc
		$(CC) $(CFLAGS) -c main.cc -o main.o

clean:
	rm -f *.o *~ parser/*.o parser/*~ scanner/*.o scanner/*~ src/*.o src/*~

mrproper:	clean
	rm -f pddl_3-1-te

.PHONY: mrproper

#do not remove Scanner.h !
cleanall:	mrproper
	rm -f parser/parse.cc parser/Parserbase.h  scanner/lex.cc scanner/Scannerbase.h

bison:
	cd parser && bisonc++ -H'../headers.h' grammar && cd ..

flex:
	cd scanner && flexc++ lexer && cd ..

reset: cleanall bison flex pddl_3-1-te
