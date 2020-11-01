CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

all: hash.o htest1 

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c -o hash.o

htest1.o:htest1.c
	$(CC) $(CFLAGS) -c htest1.c -o htest1.o

htest1:htest1.o hash.o
	$(CC) $(CFLAGS) htest1.o hash.o -o htest1

htest2.o:htest2.c
	$(CC) $(CFLAGS) -c htest2.c -o htest2.o

htest2:htest2.o hash.o
	$(CC) $(CFLAGS) htest2.o hash.o -o htest2

htest3.o:htest3.c
	$(CC) $(CFLAGS) -c htest3.c -o htest3.o

htest3:htest3.o hash.o
	$(CC) $(CFLAGS) htest3.o hash.o -o htest3

clean:
	rm -i hash.o htest1.o htest1 htest2.o htest2 htest3.o htest3