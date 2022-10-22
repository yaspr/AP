//
#include <stdio.h>
#include <stdlib.h>

//
unsigned long long fibo_c(unsigned long long n)
{
  //
  if ((n == 0) || (n == 1))
    return 1;
  
  //
  unsigned long long t = 0;
  unsigned long long f0 = 1;
  unsigned long long f1 = 1;

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
unsigned long long fibo_asm(unsigned long long n)
{
  //
  if ((n == 0) || (n == 1))
    return 1;

  //
  unsigned long long f = 1;

  //
  __asm__ volatile (
		    ".intel_syntax noprefix;\n"
		    
		    "mov rcx, 1;\n" //i = 1 
		    
		    "mov rax, 1;\n" //f1 = 1
		    "mov rbx, 1;\n" //f0 = 1

		    "loop:;\n"
		    
		    "mov rdx, rax;\n" //t = f1
		    "add rax, rbx;\n" //f1 += f0
		    "mov rbx, rdx;\n" //f0 = t
		    
		    "add rcx, 1;\n"   //i++
		    "cmp rcx, %1;\n" 
		    "jl loop;\n"

		    "mov %0, rax;\n" //f = f1
		    
		    ".att_syntax prefix;\n"

		    : //outputs
		      "=r" (f)
		      
		    : //inputs
		      "r" (n)
		    
		    :
		    "cc", "memory", "rax", "rbx", "rcx", "rdx");
  
  //
  return f;
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
  printf("fibo_c(%llu): %llu\n", n, fibo_c(n));
  printf("fibo_asm(%llu): %llu\n", n, fibo_asm(n));
  
  //
  return 0;
}
