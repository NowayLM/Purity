CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -MMD -MP
EXECUTABLE = pathfinder
TEST_EXECUTABLE = testQueue
SOURCES = algo/main.c structs/graph.c structs/queue.c algo/algo.c algo/dijkstra.c
TEST_SOURCES = test/testqueue.c
OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
DEPFILES = $(SOURCES:.c=.d) $(TEST_SOURCES:.c=.d)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(filter-out algo/main.o, $(OBJECTS)) -o $(TEST_EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(EXECUTABLE) $(TEST_EXECUTABLE) $(DEPFILES)

-include $(DEPFILES)