//
#include <stdio.h>
#include <stdlib.h>

//
unsigned long long collatz_c(unsigned long long n)
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

/* // */
/* unsigned long long collatz_asm(unsigned long long n) */
/* { */
/*   // */
/*   unsigned long long i = 0; */

/*   // */
/*   __asm__ volatile ( */
/* 		    ".intel_syntax noprefix;\n" */
	  
/* 		    "mov rbx, 1;\n" */
/* 		    "cmp %1, 1;\n" */
/* 		    "je loop_done1;\n" */
	  
/* 		    "loop_entry1:;\n" */
	  
/* 		    "test %1, 1;\n" */
/* 		    "jnz odd;\n" */
	  
/* 		    //Even --> n = 2 x n */
/* 		    "shr %1, 1;\n" //%0 <<= 1 --> %0 *= 2 */
/* 		    "jmp loop_exit1;\n" */

/* 		    //Odd --> n = 3 x n + 1 */
/* 		    "odd:;\n"  */
	  
/* 		    "imul %1, 3;\n" //%0 *= 3 */
/* 		    "inc %1;\n"     //%0 += 1 */
	  
/* 		    "loop_exit1:;\n" */
	  
/* 		    "inc rbx;\n"    //i++ */
/* 		    "cmp %1, 1;\n"  //n == 1 */
/* 		    "jne loop_entry1;\n" */

/* 		    "loop_done1:;\n" */
	  
/* 		    "mov %0, rbx;\n" */
	  
/* 		    ".att_syntax prefix;\n" */
	  
/* 		    : //Outputs */
/* 		      "=r" (i) */
	    
/* 		    : //Inputs */
/* 		      "r" (n) */
	    
/* 		    : //Clobber */
/* 		      "cc", "rbx"); */
  
/*   // */
/*   return i; */
/* } */

//
unsigned long long collatz_asm(unsigned long long n) {

  // Number of iterations
  unsigned long long i = 0;

  __asm__ volatile (

		    "xor %[_i], %[_i];\n"
		    "xor %%rax, %%rax;\n" // rax = 0
		    "xor %%rdx, %%rdx;\n" // rdx = 0
		    "mov $2, %%ecx\n"     // dividend, should be replaced by a bitshift

		    "loop:\n"
		    "inc %[_i];\n"
		    "cmp  $1, %[_v];\n"
		    "je loop_end;\n"

		    "test $1, %[_v]\n"
		    "jnz odd;\n"
		    // v is even
		    "mov %[_v], %%rax;\n"
		    "div %%rcx;\n"
		    "movsx %%eax, %[_v];\n"
		    "jmp loop;\n"
		    
		    "odd:\n"
		    "imul $3, %[_v];\n"
		    "inc %[_v];\n"
		    "jmp loop ;\n"
		    "loop_end:\n"

		    : // outputs
		      [_i] "=&r"(i)
		      
		    : // inputs
		      [_v] "r"(n)
		      
		    : // clobber
		      "cc", "memory", "eax", "rax", "rcx");

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

  if (n == 0)
    return printf("n = 0 not allowed!\n"), 2;
  
  //
  printf("collatz_c(%llu)  : %llu\n", n, collatz_c(n));
  printf("collatz_asm(%llu): %llu\n", n, collatz_asm(n));
  
  
  //
  return 0;
}
