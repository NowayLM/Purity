CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lm pkg-config --cflags --libs gtk+-3.0
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC_FILES := $(wildcard $(SRC_DIR)/.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

TARGET = $(BIN_DIR)/Purity

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/.o $(TARGET)

.PHONY: all clean

