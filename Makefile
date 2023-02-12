CC= gcc
CFLAGS= -Wall -Wextra

main: 
	gcc -o main algo/main.c structs/graph.c structs/queue.c

clean:
	$(RM) main