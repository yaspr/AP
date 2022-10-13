//
#include <stdio.h>
#include <stdlib.h>

//
unsigned long long collatz(unsigned long long n)
{
  //Number of iterations
  unsigned long long i = 1;
  unsigned long long v = n;
  
  //
  while (v != 1)
    {
      //Even 
      if ((v % 2) == 0)
	v /= 2;
      else //Odd
	v = (3 * v) + 1;

      //Count iterations
      i++;
    }

  //
  return i;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 2)
    return printf("Usage: %s [n]\n", argv[0]), 1;
  
  //
  unsigned long long n = atoll(argv[1]);

  //
  printf("collatz(%llu): %llu\n", n, collatz(n));
  
  //
  return 0;
}
