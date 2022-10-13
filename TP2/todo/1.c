//
#include <stdio.h>
#include <stdlib.h>

//
float fadd(float a, float b)
{
  //
  float c = 0.0;

  //
  __asm__ volatile (
		    "movss (%[_a]), %%xmm0;\n"
		    "movss (%[_b]), %%xmm1;\n"

		    "addss %%xmm1, %%xmm0;\n"

		    "movss %%xmm0, (%[_c]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (&a),
		      [_b] "r" (&b),
		      [_c] "r" (&c)
		      
		    : //clobbers
		      "cc", "memory", "xmm0", "xmm1"
		    );

  //
  return c;
}

//
float fmul(float a, float b)
{
  //
  float c = 0.0;

  //
  __asm__ volatile (
		    "movss (%[_a]), %%xmm0;\n"
		    "movss (%[_b]), %%xmm1;\n"

		    "mulss %%xmm1, %%xmm0;\n"

		    "movss %%xmm0, (%[_c]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (&a),
		      [_b] "r" (&b),
		      [_c] "r" (&c)
		      
		    : //clobbers
		      "cc", "memory", "xmm0", "xmm1"
		    );

  //
  return c;
}

//
float fdiv(float a, float b)
{
  //
  float c = 0.0;

  //
  __asm__ volatile (
		    "movss (%[_a]), %%xmm0;\n"
		    "movss (%[_b]), %%xmm1;\n"
		    
		    "divss %%xmm1, %%xmm0;\n"
		    
		    "movss %%xmm0, (%[_c]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (&a),
		      [_b] "r" (&b),
		      [_c] "r" (&c)
		      
		    : //clobbers
		      "cc", "memory", "xmm0", "xmm1"
		    );
  
  //
  return c;
}

//
int main(int argc, char **argv)
{
  //
  printf("%f\n", fadd(1.61, 3.14));
  printf("%f\n", fmul(1.61, 3.14));
  printf("%f\n", fdiv(4.4, 2.0));
  
  //
  return 0;
}
