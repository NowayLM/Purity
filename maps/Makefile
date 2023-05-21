CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
LIBS= -lxml2 -lm
all: prog

prog: maps2.o
	$(CC) $(CFLAGS) -o prog maps2.o $(LIBS)

maps2.o: maps2.c
	$(CC) $(CFLAGS) -c maps2.c -lm$(LIBS)

clean:
	rm -f maps2.o prog PurityMap.txt
