CC = clang
TARGET = mines
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -O0 -g
INCLUDE = -Iraylib/src
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa \
             -framework GLUT -framework OpenGL
LIBS = -L. -lraylib
SRC = main.c board.c
OBJ = main.o board.o

all: clean $(TARGET)

# Check if DEBUG is defined
# should be run as `make DEBUG=1 all`
ifeq ($(DEBUG), 1)
    CFLAGS += $(DEBUG_FLAGS)
endif

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJ) $(INCLUDE) $(FRAMEWORKS) $(LIBS)

board.o: board.c board.h
	$(CC) -c $(CFLAGS) $(INCLUDE) board.c -o board.o

main.o: board.h main.c
	$(CC) -c $(CFLAGS) $(INCLUDE) main.c -o main.o

run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -rf $(TARGET) *.o
