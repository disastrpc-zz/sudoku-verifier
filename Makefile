CC=gcc
CFLAGS=-lpthread

handler: main.c handler.c
	$(CC) -o solver main.c handler.c $(CFLAGS)
