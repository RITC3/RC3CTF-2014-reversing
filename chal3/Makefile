CC=gcc
CFLAGS=-std=c99
CLIBFLAGS=
CSOURCE=chal3.c

all: chal3

chal3: clean
	$(CC) $(CFLAGS) -o chal3 $(CSOURCE) $(CLIBFLAGS)

clean:
	rm -rf chal3

debug: clean
	$(CC) $(CFLAGS) -ggdb -o chal3 $(CSOURCE) $(CLIBFLAGS)
