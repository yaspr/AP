CC=gcc
CFLAGS=-Wall -g
OFLAGS=-O3 -fopenmp
AFLAGS=-march=native -mtune=native
LFLAGS=-lm

all: reduc_parallel reduc_parallel_atomic reduc_parallel_simd

reduc_parallel: reduc_parallel.c
	$(CC) $(AFLAGS) $(CFLAGS) $(OFLAGS) $< -o $@ $(LFLAGS) 

reduc_parallel_atomic: reduc_parallel_atomic.c
	$(CC) $(AFLAGS) $(CFLAGS) $(OFLAGS) $< -o $@ $(LFLAGS) 

reduc_parallel_simd: reduc_parallel_simd.c
	$(CC) $(AFLAGS) $(CFLAGS) $(OFLAGS) $< -o $@ $(LFLAGS) 

clean:
	rm -Rf reduc_parallel reduc_parallel_atomic reduc_parallel_simd
