//
#include <stdio.h>
#include <stdlib.h>

//
void scale(int *p, unsigned long long n, int s)
{
  //
  for (unsigned long long i = 0; i < n; i++)
    p[i] *= s;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [n] [s]\n", argv[0]), 1;

  //Number of array elements
  unsigned long long n = atoi(argv[1]);

  //Scaling factor
  int s = atoi(argv[2]);
  
  if (n < 5)
    return printf("Error: 'n' must be >= 5\n"), 2;
  
  //
  int *p = malloc(sizeof(int) * n);

  if (!p)
    return printf("Error: cannot allocate memory\n"), 3;

  //Initialize array
  for (unsigned long long i = 0; i < n; i++)
    p[i] = i + 1;

  //Scale the vector by s
  scale(p, n, s);

  //Print vector
  for (unsigned long long i = 0; i < n; i++)
    printf("%5d%c", p[i], ((i + 1) % 10) ? '\t' : '\n');

  putchar('\n');
  
  //
  free(p);
  
  //
  return 0;
}
