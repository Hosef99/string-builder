CC := gcc
CFLAGS := -g -Wextra -Wall
CPPFLAGS := -Iinclude

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

TARGET := sb

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $^ -o $@
	
build: 
	mkdir -p build

clean:
	rm -rf build $(TARGET)

run: $(TARGET)
	./$(TARGET)
