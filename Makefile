CC=gcc
CFLAGS=-Wall -Wextra -pedantic

nes: src/main.c src/bus.c src/cpu.c src/memory.c
		$(CC) $(CFLAGS) -o nes src/main.c src/bus.c src/cpu.c src/memory.c