CC=gcc
CFLAGS=-Wall -g
OFLAGS=-O3 -fopenmp
AFLAGS=-march=native -mtune=native
LFLAGS=-lpthread -lm

all: reduc_parallel

reduc_parallel: reduc_parallel.c
	$(CC) $(AFLAGS) $(CFLAGS) $(OFLAGS) $< -o $@ $(LFLAGS) 

clean:
	rm -Rf reduc_parallel
