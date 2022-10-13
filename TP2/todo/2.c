//
#include <stdio.h>
#include <stdlib.h>

//
double fadd(double a, double b)
{
  //
  double c = 0.0;

  //
  __asm__ volatile (
		    "movsd (%[_a]), %%xmm0;\n"
		    "movsd (%[_b]), %%xmm1;\n"

		    "addsd %%xmm1, %%xmm0;\n"

		    "movsd %%xmm0, (%[_c]);\n"
		    
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
double fmul(double a, double b)
{
  //
  double c = 0.0;

  //
  __asm__ volatile (
		    "movsd (%[_a]), %%xmm0;\n"
		    "movsd (%[_b]), %%xmm1;\n"

		    "mulsd %%xmm1, %%xmm0;\n"

		    "movsd %%xmm0, (%[_c]);\n"
		    
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
double fdiv(double a, double b)
{
  //
  double c = 0.0;

  //
  __asm__ volatile (
		    "movsd (%[_a]), %%xmm0;\n"
		    "movsd (%[_b]), %%xmm1;\n"
		    
		    "divsd %%xmm1, %%xmm0;\n"
		    
		    "movsd %%xmm0, (%[_c]);\n"
		    
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
  printf("%.12lf\n", fadd(1.618033988749, 3.141592653589));
  printf("%.12lf\n", fmul(1.618033988749, 3.141592653589));
  printf("%.12lf\n", fdiv(1.618033988749, 3.141592653589));
  
  //
  return 0;
}
