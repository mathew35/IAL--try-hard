CFLAGS=-std=c99 -Wall -pedantic -g
BIN=graph
CC=gcc
RM=rm -f

ALL: graph.o 
	$(CC) $(CFLAGS) -o $(BIN) graph.o
clean:
	$(RM) *.o $(BIN) *.out
