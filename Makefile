CC= gcc
CFLAGS= -Wall -Wextra

main: pathfinder testQueue

pathfinder: 
	gcc -o pathfinder algo/main.c structs/graph.c structs/queue.c algo/algo.c

testQueue:
	gcc -o testQueue test/testqueue.c structs/graph.c structs/queue.c


clean:
	$(RM) main
	$(RM) testQueue
	$(RM) pathfinder
