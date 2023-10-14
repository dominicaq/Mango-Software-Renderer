CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): main.c
	$(CC) $(CFLAGS) -o $@ $<

gdb: $(EXECUTABLE)
	gdb ./$<

run: $(EXECUTABLE)
	./$<

clean:
	rm -f $(EXECUTABLE)
