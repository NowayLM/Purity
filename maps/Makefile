CC=gcc
CFLAGS=-Wall -std=c99 -pedantic
LIBS= -lxml2
all: maps2

maps2: maps2.o
	$(CC) $(CFLAGS) -o maps2 maps2.o $(LIBS)

maps2.o: maps2.c
	$(CC) $(CFLAGS) -c maps2.c $(LIBS)

clean:
	rm -f maps2.o maps2
