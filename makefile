CFLAGS=-std=c99 -Wall -pedantic -g 
BIN=graph
CC=gcc
RM=rm -f

ALL: array.o graph.o solution.o
	$(CC) $(CFLAGS) -o $(BIN) array.o graph.o solution.o
	$(RM) *.o
clean:
	$(RM) *.o $(BIN)
