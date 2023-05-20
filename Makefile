CC = gcc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g
SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf
EXECUTABLE = pathfinder
TEST_EXECUTABLE = testQueue
SOURCES = maps/drawFinal.c structs/graph.c structs/queue.c algo/algo.c algo/dijkstra.c maps/drawMap.c maps/finalMap.c
TEST_SOURCES = test/testqueue.c
OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
DEPFILES = $(SOURCES:.c=.d) $(TEST_SOURCES:.c=.d)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(SDL_FLAGS) -o $(EXECUTABLE) -lm

$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(filter-out maps/drawFinal.o, $(OBJECTS)) $(SDL_FLAGS) -o $(TEST_EXECUTABLE) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(EXECUTABLE) $(TEST_EXECUTABLE) $(DEPFILES)

-include $(DEPFILES)
