//
#include <stdio.h>
#include <stdlib.h>

//c[0..3] = a[0..3] + b[0..3] 
void vadd(double *a, double *b, double *c)
{
  //
  __asm__ volatile (
		    "movapd (%[_a]), %%xmm0;\n"
		    
		    "addpd (%[_b]), %%xmm0;\n"

		    "movapd %%xmm0, (%[_c]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (a),
		      [_b] "r" (b),
		      [_c] "r" (c)
		      
		    : //clobbers
		      "cc", "memory", "xmm0", "xmm1"
		    );
}

//c[0..3] = a[0..3] * b[0..3] 
void vmul(double *a, double *b, double *c)
{
  //
  __asm__ volatile (
		    "movapd (%[_a]), %%xmm0;\n"

		    "mulpd (%[_b]), %%xmm0;\n"

		    "movapd %%xmm0, (%[_c]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (a),
		      [_b] "r" (b),
		      [_c] "r" (c)
		      
		    : //clobbers
		      "cc", "memory", "xmm0", "xmm1"
		    );
}

//c[0..3] = a[0..3] / b[0..3] 
void vdiv(double *a, double *b, double *c)
{
  //
  __asm__ volatile (
		    "movapd (%[_a]), %%xmm0;\n"

		    "divpd (%[_b]), %%xmm0;\n"
		    
		    "movapd %%xmm0, (%[_c]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (a),
		      [_b] "r" (b),
		      [_c] "r" (c)
		      
		    : //clobbers
		      "cc", "memory", "xmm0", "xmm1"
		    );
}

//
int main(int argc, char **argv)
{
  //
  double __attribute__((aligned(16))) a[2] = { 2.2, 4.4 };
  double __attribute__((aligned(16))) b[2] = { 2.0, 2.0 };
  double __attribute__((aligned(16))) c[2] = { 0.0, 0.0 };
  
  //
  vadd(a, b, c);
  printf("%f %f\n", c[0], c[1]);

  //
  vmul(a, b, c);
  printf("%f %f\n", c[0], c[1]);
  
  //
  vdiv(a, b, c);
  printf("%f %f\n", c[0], c[1]);
  
  //
  return 0;
}
