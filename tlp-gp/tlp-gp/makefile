CC=g++
CFLAGS=-W -Wall -ansi -pedantic
EXEC=prog
LDFLAGS=
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

prog: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.h 

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf prog
