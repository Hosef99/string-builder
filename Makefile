TARGET = libstringbuilder.a
OBJS = string_builder.o

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: src/%.c include/%.h
	$(CC) -I./include -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
