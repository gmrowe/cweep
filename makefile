SHELL := /bin/bash

CC = clang
TARGET = mines
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -O0 -g
INCLUDE = -Iraylib/src
LIBS = -L. -lraylib

# Directory structure
SRC_DIR = ./src
BUILD_DIR = ./build

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/board.c
OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/board.o


# Check the operating system
UNAME_S := $(shell uname -s)

# Check if DEBUG is defined
# should be run as `make DEBUG=1 all`
ifeq ($(DEBUG), 1)
    CFLAGS += $(DEBUG_FLAGS)
endif

# Set flags and libraries based on the operating system
ifeq ($(UNAME_S),Darwin) # macOS
    FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa \
#                  -framework GLUT -framework OpenGL
    LINK_FLAGS = $(FRAMEWORKS) $(LIBS)
else ifeq ($(UNAME_S),Linux) # Linux
    LINK_FLAGS = -lGL -lm -lpthread -ldl -lrt -lX11 $(LIBS)
else
    $(error Unsupported operating system: $(UNAME_S))
endif

all: clean directories $(TARGET)

directories:
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJ) $(INCLUDE) $(LINK_FLAGS)

$(BUILD_DIR)/board.o: $(SRC_DIR)/board.c $(SRC_DIR)/board.h
	$(CC) -c $(CFLAGS) $(INCLUDE) $(SRC_DIR)/board.c -o $(BUILD_DIR)/board.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/board.h $(SRC_DIR)/main.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

run: $(TARGET)
	./$(TARGET)

.PHONY: clean directories
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
