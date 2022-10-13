CC=gcc

CFLAGS=-Wall -g3

OFLAGS=-O1

all: genseq 1 2 3 4 5 6

1: 1.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

2: 2.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

3: 3.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

4: 4.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

5: 5.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

6: 6.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

genseq: genseq.c
	$(CC) $(CFLAGS) $(OFLAGS) $< -o $@

clean:
	rm -Rf 1 2 3 4 5 6 genseq
