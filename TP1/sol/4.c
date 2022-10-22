//
#include <stdio.h>
#include <stdlib.h>

//
long long dotprod_c(int *a, int *b, unsigned long long n)
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
long long dotprod_asm(int *a, int *b, unsigned long long n)
{
  //
  long long d = 0;

  //Size in bytes
  unsigned long long sizeb = sizeof(int) * n;
  
  //
  __asm__ volatile(
		   "xor %%rcx, %%rcx;\n"
		   "xor %%rdx, %%rdx;\n"
		   
		   "1:;\n"
		   "mov (%[_a], %%rcx), %%eax;\n" //eax = a[i]
		   "mov (%[_b], %%rcx), %%ebx;\n" //ebx = b[i]

		   "imul %%ebx, %%eax;\n"       // eax *= ebx
		   "add %%eax, %%edx;\n"        // rdx += rax
		   
		   "add $4, %%rcx;\n"
		   "cmp %[_sizeb], %%rcx;\n"
		   "jl 1b;\n"

		   "mov %%rdx, %[_d];\n"
		   
		   : //outputs
		     [_d] "=r" (d)
		     
		   : //inputs
		     [_a]     "r" (a),
		     [_b]     "r" (b),
		     [_sizeb] "r" (sizeb)
		     
		   : //clobber
		     "cc", "memory", "eax", "ebx", "rcx", "rdx"
		   );

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
  long long d_c = dotprod_c(a, b, n);

  //Print vector
  printf("dotprod_c: %lld\n", d_c);

    //Initialize arrays
  for (unsigned long long i = 0; i < n; i++)
    {
      a[i] = i + 1;
      b[i] = i - 1;
    }
  
  //Scale the vector by s
  long long d_asm = dotprod_asm(a, b, n);

  //Print vector
  printf("dotprod_asm: %lld\n", d_asm);

  //
  free(a);
  free(b);
  
  //
  return 0;
}
