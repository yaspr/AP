//
#include <stdio.h>
#include <stdlib.h>

//
void scale_c(int *p, unsigned long long n, int s)
{
  //
  for (unsigned long long i = 0; i < n; i++)
    p[i] *= s;
}

//
void scale_asm(int *p, unsigned long long n, int s)
{
  //Size in bytes
  unsigned long long sizeb = sizeof(int) * n;

  //
  __asm__ volatile(
		   "xor %%rcx, %%rcx;\n"
		   
		   "1:;\n"
		   
		   "mov (%[_p], %%rcx), %%eax;\n" //eax = p[i]
		   "imul %[_s], %%eax;\n"         //eax *= s
		   "mov %%eax, (%[_p], %%rcx);\n" //p[i] = eax
		   
		   "add $4, %%rcx;\n"             //rcx += sizeof(int)
		   "cmp %[_sizeb], %%rcx;\n"
		   "jl 1b;\n"
		   
		   : //outputs
		     
		   : //inputs
		     [_p]     "r" (p),
		     [_s]     "r" (s),
		     [_sizeb] "r" (sizeb)
		     
		   : //clobber
		     "cc", "memory", "eax", "rcx");
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

  // ==== C ====
  printf("\t== C ==\n");
  
  //Scale the vector by s
  scale_c(p, n, s);

  //Print vector
  for (unsigned long long i = 0; i < n; i++)
    printf("%5d%c", p[i], ((i + 1) % 10) ? '\t' : '\n');

  putchar('\n');

  // ==== ASSEMBLY ====
  printf("\t== ASM ==\n");
  
  //Initialize array
  for (unsigned long long i = 0; i < n; i++)
    p[i] = i + 1;

  //Scale the vector by s
  scale_asm(p, n, s);

  //Print vector
  for (unsigned long long i = 0; i < n; i++)
    printf("%5d%c", p[i], ((i + 1) % 10) ? '\t' : '\n');

  putchar('\n');

  //
  free(p);
  
  //
  return 0;
}
