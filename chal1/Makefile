CSOURCE=scanner.c tokenize.c
CC=gcc
CFLAGS=-std=c99
CLIBFLAGS=-lm

all: chal1

chal1: clean
	$(CC) $(CFLAGS) -o chal1 $(CSOURCE) $(CLIBFLAGS)

static: clean
	$(CC) $(CFLAGS) -static -o chal1 $(CSOURCE) $(CLIBFLAGS)

clean:
	rm -rf chal1
