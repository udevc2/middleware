CC=gcc 
CFLAGS=-Wall

all: middleware
middleware: middleware.o
middleware.o: middleware.c

clean:
	rm -f middleware middleware.o

run: middleware
	./middleware
