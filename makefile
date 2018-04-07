CC=gcc
CFLAGS=-I .

all: pow2.c pow2_tests.c
	$(CC) -o pow2_tests pow2_tests.c pow2.c $(CFLAGS)
