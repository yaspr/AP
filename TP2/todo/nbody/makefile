all: nbody0

nbody0: nbody0.c
	gcc -g -Ofast -funroll-loops -finline-functions -ftree-vectorize $< -o $@ -lm -lSDL2 

clean:
	rm -Rf *~ nbody0
