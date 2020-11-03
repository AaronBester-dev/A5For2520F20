CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: hash.o

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c -o hash.o

clean:
	rm -i hash.o 