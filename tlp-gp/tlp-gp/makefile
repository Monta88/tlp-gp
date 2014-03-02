CC=g++
CFLAGS=-W -Wall -ansi -pedantic
EXEC=prog

all: $(EXEC)

prog: domain.o type.o fluent.o action.o durative_action.o intervalle.o variable.o main.o
	$(CC) -o prog domain.o main.o $(LDFLAGS)

main.o: main.cpp 
	gcc -o main.o -c main.cpp -W -Wall -ansi -pedantic

domain.o: domain.cpp type.hpp fluent.h action.h
	$(CC) -o domain.o -c domain.cpp $(LDFLAGS)

type.o: type.cpp type.hpp
	$(CC) -o type.o -c type.cpp $(LDFLAGS)

fluent.o: fluent.cpp fluent.h
	$(CC) -o fluent.o -c fluent.cpp $(LDFLAGS)

action.o: action.cpp action.h
	$(CC) -o action.o -c action.cpp $(LDFLAGS)

durative_action.o: durative_action.cpp durative_action.hpp
	$(CC) -o action.o -c durative_action.cpp $(LDFLAGS)

intervalle.o: intervalle.cpp intervalle.hpp variable.hpp
	$(CC) -o intervalle.o -c intervalle.cpp $(LDFLAGS)

variable.o: variable.cpp variable.hpp
	$(CC) -o variable.o -c variable.cpp $(LDFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf prog
