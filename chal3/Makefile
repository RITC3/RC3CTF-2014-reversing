CC=gcc
CFLAGS=-std=c99 -DCTF_SPEC
CLIBFLAGS=
CSOURCE=chal3.c ctfserver.c

all: chal3

chal3: clean
	$(CC) $(CFLAGS) -o chal3 $(CSOURCE) $(CLIBFLAGS)

static: clean
	$(CC) $(CFLAGS) -static -o chal3 $(CSOURCE) $(CLIBFLAGS)

clean:
	rm -rf chal3

debug: clean
	$(CC) $(CFLAGS) -ggdb -o chal3 $(CSOURCE) $(CLIBFLAGS)
