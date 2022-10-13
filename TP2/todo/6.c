//
#include <stdio.h>
#include <stdlib.h>

//
void vsqrt(float *a, float *b)
{
  __asm__ volatile (
		    "sqrtps (%[_a]), %%xmm0;\n"

		    "movaps %%xmm0, (%[_b]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (a),
		      [_b] "r" (b)
		      
		    : //clobbers
		      "cc", "memory", "xmm0"
		    );
}

//
void vrsqrt(float *a, float *b)
{
  __asm__ volatile (
		    "rsqrtps (%[_a]), %%xmm0;\n"

		    "movaps %%xmm0, (%[_b]);\n"
		    
		    : //outputs
		      
		    : //inputs
		      [_a] "r" (a),
		      [_b] "r" (b)
		      
		    : //clobbers
		      "cc", "memory", "xmm0"
		    );
}

//
int main(int argc, char **argv)
{
  //
  float __attribute__((aligned(16))) a[4] = { 25.0, 16.0, 9.0, 4.0 };
  float __attribute__((aligned(16))) b[4] = { 0.0, 0.0, 0.0, 0.0 };

  //
  vsqrt(a, b);
  printf("%f %f %f %f\n", b[0], b[1], b[2], b[3]);

  //
  vrsqrt(a, b);
  printf("%f %f %f %f\n", b[0], b[1], b[2], b[3]);
  
  //
  return 0;
}
