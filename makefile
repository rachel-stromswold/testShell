CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wformat=2 -g
BINARY=superShell
OBJ=dispatch.o io.o main.o
.PHONY: clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
