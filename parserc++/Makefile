# Makefile

ifndef CC
	CC = gcc
endif	
ifndef CXX
	CXX = g++
endif

CXXFLAGS	= -std=c++11 -Wall -g


SOURCES = $(wildcard *.cpp src/*.cpp)
OBJECTS = lex.o parse.o $(SOURCES:.cpp=.o)
EXEC	= prog

$(EXEC):		$(OBJECTS)
		$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)
		

lex.o:				scanner/lex.cc
		$(CXX) $(CXXFLAGS) -c scanner/lex.cc -o lex.o

parse.o:			parser/parse.cc
		$(CXX) $(CXXFLAGS) -c parser/parse.cc -o parse.o
		
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
src/%.o: /src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


all:				bison flex $(EXEC)

clean:
	rm -f *.o *~ parser/*.o parser/*~ scanner/*.o scanner/*~ src/*.o src/*~

mrproper:			clean
	rm -f $(EXEC)

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

debug:				bison flex_debug $(EXEC)

reset: 				cleanall bison flex $(EXEC)

parser:				cleanbisonflex bison flex $(EXEC)
					
