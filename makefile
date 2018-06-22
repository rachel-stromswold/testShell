CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Wformat=2 -g -lm
BINARY=crystalSim
OBJ=main.o
.PHONY: clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
