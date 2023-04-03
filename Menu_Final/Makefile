PROGRAM = Purity
CC= gcc

CFLAGS += `pkg-config --cflags gtk+-3.0`

LIBS = `pkg-config --libs gtk+-3.0`

SRCS = menu.c

GLADE_FILE = Interface.glade

all : $(PROGRAM)

$(PROGRAM) : $(SRCS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(SRCS) $(LIBS) 

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM)

.PHONY: all clear run
