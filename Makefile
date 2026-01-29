CC = gcc
CFLAGS = -I./include -g -Wall -Wextra
AR = ar
ARFLAGS = rcs

TARGET = libstringbuilder.a
OBJS = string_builder.o

TEST_TARGET = test/sb_test
TEST_SRC = test/main.c

all: $(TARGET)

debug: CFLAGS += -g -DSB_DEBUG
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.o: src/%.c	
	$(CC) $(CFLAGS) -c $< -o $@

test/%.o: test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(TARGET)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJ) $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)

.PHONY: all clean test
