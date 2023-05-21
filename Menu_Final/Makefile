PROGRAM = Purity
CC= gcc

CFLAGS += `pkg-config --cflags gtk+-3.0`

SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

LIBS = `pkg-config --libs gtk+-3.0` -lm 

SRCS = menu.c ../algo/algo.c ../structs/graph.c ../structs/queue.c ../algo/dijkstra.c ../maps/drawMap.c ../maps/finalMap.c

GLADE_FILE = Interface.glade

all : $(PROGRAM)

$(PROGRAM) : $(SRCS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(SDL_FLAGS) $(SRCS) $(LIBS) 

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM)

.PHONY: all clear run
