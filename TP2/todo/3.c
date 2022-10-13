//
#include <stdio.h>
#include <stdlib.h>

//c[0..3] = a[0..3] + b[0..3] 
void vadd(float *a, float *b, float *c)
{
  //
  __asm__ volatile (
		    "movaps (%[_a]), %%xmm0;\n"
		    
		    "addps (%[_b]), %%xmm0;\n"

		    "movaps %%xmm0, (%[_c]);\n"
		    
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
void vmul(float *a, float *b, float *c)
{
  //
  __asm__ volatile (
		    "movaps (%[_a]), %%xmm0;\n"

		    "mulps (%[_b]), %%xmm0;\n"

		    "movaps %%xmm0, (%[_c]);\n"
		    
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
void vdiv(float *a, float *b, float *c)
{
  //
  __asm__ volatile (
		    "movaps (%[_a]), %%xmm0;\n"

		    "divps (%[_b]), %%xmm0;\n"
		    
		    "movaps %%xmm0, (%[_c]);\n"
		    
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
  float __attribute__((aligned(16))) a[4] = { 2.2, 4.4, 6.6, 8.8 };
  float __attribute__((aligned(16))) b[4] = { 2.0, 2.0, 2.0, 2.0 };
  float __attribute__((aligned(16))) c[4] = { 0.0, 0.0, 0.0, 0.0 };
  
  //
  vadd(a, b, c);
  printf("%f %f %f %f\n", c[0], c[1], c[2], c[3]);

  //
  vmul(a, b, c);
  printf("%f %f %f %f\n", c[0], c[1], c[2], c[3]);
  
  //
  vdiv(a, b, c);
  printf("%f %f %f %f\n", c[0], c[1], c[2], c[3]);
  
  //
  return 0;
}
