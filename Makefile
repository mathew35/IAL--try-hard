
PRJ=graph_generator
#
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic

all: $(PRJ)

$(PRJ): $(PRJ).c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c

clean:
	rm -f *.o *.out $(PROGS)
#
