//
#include <stdio.h>
#include <stdlib.h>

//
long long dotprod(int *a, int *b, unsigned long long n)
{
  //
  long long d = 0;

  //
  for (unsigned long long i = 0; i < n; i++)
    d += (a[i] * b[i]);

  //
  return d;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 2)
    return printf("Usage: %s [n]\n", argv[0]), 1;

  //Number of array elements
  unsigned long long n = atoll(argv[1]);
  
  if (n < 5)
    return printf("Error: 'n' must be >= 5\n"), 2;
  
  //
  int *a = malloc(sizeof(int) * n);
  int *b = malloc(sizeof(int) * n);

  if (!a || !b)
    return printf("Error: cannot allocate memory\n"), 3;
  
  //Initialize arrays
  for (unsigned long long i = 0; i < n; i++)
    {
      a[i] = i + 1;
      b[i] = i - 1;
    }
  
  //Scale the vector by s
  long long d = dotprod(a, b, n);

  //Print vector
  printf("dotprod: %lld\n", d);
  
  //
  free(a);
  free(b);
  
  //
  return 0;
}
