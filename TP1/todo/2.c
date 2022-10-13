//
#include <stdio.h>
#include <stdlib.h>

//
unsigned long long fibo(unsigned long long n)
{
  //
  if ((n == 0) || (n == 1))
    return 1;
  
  //
  int t = 0;
  int f0 = 1;
  int f1 = 1;

  //
  for (unsigned long long i = 1; i < n; i++)
    {
      t = f1;
      f1 += f0;
      f0 = t;
    }

  //
  return f1;
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
  printf("fibo(%llu): %llu\n", n, fibo(n));
  
  //
  return 0;
}
