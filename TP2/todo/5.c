//
#include <stdio.h>
#include <stdlib.h>

//
float fsqrt(float a)
{
  float b = 0.0;
  
  __asm__ volatile (
		    "sqrtss (%[_a]), %%xmm0;\n"

		    "movss %%xmm0, (%[_b]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (&a),
		      [_b] "r" (&b)
		      
		    : //clobbers
		      "cc", "memory", "xmm0"
		    );

  return b;
}

//
float frsqrt(float a)
{
  float b = 0.0;
  
  __asm__ volatile (
		    "rsqrtss (%[_a]), %%xmm0;\n"

		    "movss %%xmm0, (%[_b]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (&a),
		      [_b] "r" (&b)
		      
		    : //clobbers
		      "cc", "memory", "xmm0"
		    );

  return b;  
}

//
int main(int argc, char **argv)
{
  //
  printf("%f\n", fsqrt(4.0));
  printf("%f\n", fsqrt(9.0));
  printf("%f\n", fsqrt(3.14));

  //
  putchar('\n');
  
  //
  printf("%f\n", frsqrt(4.0));
  printf("%f\n", frsqrt(9.0));
  printf("%f\n", frsqrt(3.14));

  //
  return 0;
}
