/*
  This code is meant as an example of to write inline x86 assembly code.
  
  Compilation:
      $ gcc -O1 0.c -o 0
      
  Execution:
      $ ./0 10 3

  For more info about inline assembly:

  https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
*/

//
#include <stdio.h>
#include <stdlib.h>

//Example of an addition without register labeling.
//Registers are addressed using their positionning 0, 1, 2, ...
int asm_iadd0(int a, int b)
{
  //
  int s = 0;

  __asm__ volatile(
		   
		   "add %1, %2;\n" //b += a
		   
		   "mov %2, %0;\n" //s = b
		   
		   : //outputs
		     "=r" (s)  //%0 is the register's ID

		   : //inputs
		     "r" (a),  //%1
		     "r" (b)   //%2
		     
		   : //clobber
		     "cc", "memory");

  return s;
}

//Using register labels
int asm_iadd1(int a, int b)
{
  //
  int s = 0;

  __asm__ volatile(
		   
		   "add %[_a], %[_b];\n" //b += a

		   "mov %[_b], %[_s];\n" //s = b
		   
		   : //outputs
		     [_s] "=r" (s)  //value

		   : //inputs
		     [_a] "r" (a),  //value
		     [_b] "r" (b)   //value
		     
		   : //clobber
		     "cc", "memory");

  return s;
}

//
int asm_iadd2(int a, int b)
{
  //
  int s = 0;

  __asm__ volatile(

		   "add %[_a], %[_b];\n" //b += a

		   "mov %[_b], (%[_s]);\n" //*s = b
		   
		   : //outputs
		   : //inputs
		     [_a] "r" (a),  //value
		     [_b] "r" (b),  //value
		     [_s] "r" (&s)  //pointer
		     
		   : //clobber
		     "cc", "memory");

  return s;
}

//
int asm_imul(int a, int b)
{
  //
  int m = 0;

  __asm__ volatile (
		    "imul %[_a], %[_b];\n"

		    "mov %[_b], %[_m];\n"
		    
		    : //outputs
		      [_m] "=r" (m)
		      
		    : //inputs
		      [_a] "r" (a),  //value
		      [_b] "r" (b)   //value

		    : //clobber
		      "cc", "memory");

  return m;
}

//
void asm_idiv(int a, int b, int *q, int *r)
{
  //Quotient
  int qq = 0;
  
  //Remainder
  int rr = 0;

  __asm__ volatile (
		    "xor %%eax, %%eax;\n" //rax = 0
		    "xor %%edx, %%edx;\n" //rdx = 0

		    //Set eax value to a
		    "mov %[_a], %%eax;\n"
		    
		    //Divides RAX by operand --> RAX = a / b, RDX = a % b 
		    "idiv %[_b];\n"
		    
		    //
		    "mov %%eax, %[_q];\n"
		    "mov %%edx, %[_r];\n"
		    
		    : //outputs
		      [_q] "=r" (qq), //value
		      [_r] "=r" (rr)  //value
		      
		    : //inputs
		      [_a] "r" (a),  //value
		      [_b] "r" (b)   //value

		    : //clobber
		      "cc", "memory", "eax", "edx");

  //
  *q = qq;
  *r = rr;
}

//
int asm_reduc(int *p, int n)
{
  //Accumulation variable
  int r = 0;

  //Array size in bytes
  int s = sizeof(int) * n;

  //
  __asm__ volatile (
		    //Initialize registers
		    "xor %%eax, %%eax;\n" //eax = 0 --> accumulation register
		    "xor %%rcx, %%rcx;\n" //rcx = 0 --> array element index
		    
		    //Set up loop label
		    "1:;\n"
		    
		    //Load value from memory address (p + rcx) and add it to eax
		    "add (%[_p], %%rcx), %%eax;\n"
		    
		   //Loop control
		    "add $4, %%rcx;\n"     //Move rcx by 4 bytes - sizeof(int) - at each iteration
		    "cmp %[_s], %%ecx;\n"  //Compare ecx to array size
		    "jl 1b;\n"             //Jump to loop entry label if ecx < array size
		    
		    "mov %%eax, %[_r];\n"  //Store final result in output variable
		    
		    : //outputs
		      [_r] "=r" (r)
		      
		    : //inputs
		      [_p] "r" (p),
		      [_s] "r" (s)
		      
		    : //clobber
		      "cc", "memory", "eax", "rcx"
		    );
  
  //
  return r;
}

int C_reduc(int *p, int n)
{
  int r = 0;

  for (int i = 0; i < n; i++)
    r += p[i];

  return r;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s a b\n", argv[0]), 1;

  //
  int a = atoi(argv[1]);
  int b = atoi(argv[2]);

  int q = 0;
  int r = 0;
  
  //
  printf("%d + %d = %d\n"  , a, b, asm_iadd0(a, b));
  printf("%d + %d = %d\n"  , a, b, asm_iadd1(a, b));
  printf("%d + %d = %d\n\n", a, b, asm_iadd2(a, b));

  //
  printf("%d * %d = %d\n\n", a, b, asm_imul(a, b));

  //
  asm_idiv(a, b, &q, &r);
  
  printf("%d / %d => q: %d, r: %d\n\n", a, b, q, r);

  //
  int p[100];
  int n = 100;

  //Initialize array 
  for (int i = 0; i < n; i++)
    p[i] = i + 1;

  //
  int r_asm = asm_reduc(p, 100);
  int r_C   = C_reduc(p, n);

  //
  printf("reduc asm: %d\n", r_asm);
  printf("reduc C  : %d\n", r_C);
  
  //
  return 0;
}
