# Makefile

OBJS	= lex.o parse.o Data.o action.o attribute.o constant.o domain.o durative_action.o fluent.o function.o interval.o lexer.o member.o object.o planningData.o predicate.o problem.o type.o typedList.o variable.o tools.o lObjType.o edge.o vertex.o graph.o constraint.o sat.o main.o

CC		= g++
CFLAGS	= -std=c++11 -Wall -g

pddl_3-1-te:		$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o pddl_3-1-te

all:				bison flex pddl_3-1-te

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

attribute.o:			src/attribute.cpp
		$(CC) $(CFLAGS) -c src/attribute.cpp -o attribute.o

constant.o:			src/constant.cpp
		$(CC) $(CFLAGS) -c src/constant.cpp -o constant.o

domain.o:			src/domain.cpp
		$(CC) $(CFLAGS) -c src/domain.cpp -o domain.o

durative_action.o:	src/durative_action.cpp
		$(CC) $(CFLAGS) -c src/durative_action.cpp -o durative_action.o

fluent.o:			src/fluent.cpp
		$(CC) $(CFLAGS) -c src/fluent.cpp -o fluent.o

function.o:			src/function.cpp
		$(CC) $(CFLAGS) -c src/function.cpp -o function.o

interval.o:			src/interval.cpp
		$(CC) $(CFLAGS) -c src/interval.cpp -o interval.o

lexer.o:			src/lexer.cpp
		$(CC) $(CFLAGS) -c src/lexer.cpp -o lexer.o

member.o:			src/member.cpp
		$(CC) $(CFLAGS) -c src/member.cpp -o member.o

object.o:			src/object.cpp
		$(CC) $(CFLAGS) -c src/object.cpp -o object.o

planningData.o:		src/planningData.cpp
		$(CC) $(CFLAGS) -c src/planningData.cpp -o planningData.o

predicate.o:		src/predicate.cpp
		$(CC) $(CFLAGS) -c src/predicate.cpp -o predicate.o

problem.o:			src/problem.cpp
		$(CC) $(CFLAGS) -c src/problem.cpp -o problem.o

type.o:				src/type.cpp
		$(CC) $(CFLAGS) -c src/type.cpp -o type.o

typedList.o:		src/typedList.cpp
		$(CC) $(CFLAGS) -c src/typedList.cpp -o typedList.o

variable.o:			src/variable.cpp
		$(CC) $(CFLAGS) -c src/variable.cpp -o variable.o
		
graph.o: 			src/graph.cpp
		$(CC) $(CFLAGS) -c src/graph.cpp -o graph.o
		
constraint.o:		src/constraint.cpp
		$(CC) $(CFLAGS) -c src/constraint.cpp -o constraint.o
		
sat.o:		src/sat.cpp
		$(CC) $(CFLAGS) -c src/sat.cpp -o sat.o

tools.o: 			src/tools.cpp
		$(CC) $(CFLAGS) -c src/tools.cpp -o tools.o

edge.o: 			src/edge.cpp
		$(CC) $(CFLAGS) -c src/edge.cpp -o edge.o

vertex.o: 			src/vertex.cpp
		$(CC) $(CFLAGS) -c src/vertex.cpp -o vertex.o

lObjType.o: 			src/lObjType.cpp
		$(CC) $(CFLAGS) -c src/lObjType.cpp -o lObjType.o

main.o:				main.cc
		$(CC) $(CFLAGS) -c main.cc -o main.o

clean:
	rm -f *.o *~ parser/*.o parser/*~ scanner/*.o scanner/*~ src/*.o src/*~

mrproper:			clean
	rm -f pddl_3-1-te

.PHONY:				mrproper

cleanall:			mrproper
	rm -f parser/parse.cc parser/Parserbase.h  scanner/lex.cc scanner/Scannerbase.h
	
cleanbisonflex:			
	rm -f parser/parse.cc parser/Parserbase.h  scanner/lex.cc scanner/Scannerbase.h	

bison:				parser/grammar
	cd parser && bisonc++ -H'../headers.h' grammar && cd ..

flex:				scanner/lexer
	cd scanner && flexc++ lexer && cd ..

flex_debug:				scanner/lexer
	cd scanner && flexc++ --print lexer && cd ..

debug:				bison flex_debug pddl_3-1-te

reset: 				cleanall bison flex pddl_3-1-te

parser:				cleanbisonflex bison flex pddl_3-1-te
					
