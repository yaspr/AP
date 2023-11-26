//
#define _GNU_SOURCE

//
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <pmmintrin.h>

//
#include "types.h"

//
void init(f64 *restrict a, u64 n, u8 type)
{
  srand(0);
  
  //Initialize with random values
  if (type == 'r')
    for (u64 i = 0; i < n; i++)
      a[i] = (f64)RAND_MAX / (f64)rand();
  else //Clear
    if (type == 'z')
      for (u64 i = 0; i < n; i++)
	a[i] = 0.0;
    else //Initialize with a random constant
      if (type == 'c')
	{
	  const f64 c = (f64)RAND_MAX / (f64)rand();
	  
	  for (u64 i = 0; i < n; i++)
	    a[i] = c;
	}
}

//
f64 reduc_sequential(f64 *restrict a, u64 n)
{
  f64 r = 0.0;
  
  for (u64 i = 0; i < n; i++)
    r += a[i];
  
  return r;
}

#define ROUND_DOWN(x, s) ((x) & ~((s)-1))

//
f64 sum_vector2(f64 *restrict a, u64 n)
{
  __m128d sum2 = _mm_set1_pd(0.0f);
  
  u64 i = 0;

  for(; i < ROUND_DOWN(n, 2); i += 2)
    {
      __m128d a2 = _mm_load_pd(a + i);
      sum2 = _mm_add_pd(sum2, a2);
    }
  
  __m128d t1 = _mm_hadd_pd(sum2, sum2);
  f64 sum = _mm_cvtsd_f64(t1);
  
  for(; i < n; i++) 
    sum += a[i];
  
  return sum;
}

//
f64 reduc_openmp(f64 *restrict a, u64 n, u64 nt)
{
  f64 suma[nt];
  const u64 offset = ROUND_DOWN(n / nt, nt);
  
#pragma omp parallel for num_threads(nt) 
  for(u64 i = 0; i < nt; i++)
    suma[i] = sum_vector2(&a[i * offset], offset);
  
  f64 sum = 0.0f;
  
  for(u64 i = 0; i < nt; i++)
    sum += suma[i]; 
  
  for(u64 i = nt * offset; i < n; i++)
    sum += a[i];
  
  return sum;    
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [n] [t]\n", argv[0]), -1;

  u64 n = atoll(argv[1]);
  u64 nt = atoll(argv[2]);

  if (!n)
    {
      printf("Error: number of array elements is 0\n");
      exit(-1);
    }

  if (!nt)
    {
      printf("Error: number threads is 0\n");
      exit(-1);
    }
  
  //Size in bytes
  u64 s = n * sizeof(f64);

  //
  printf("\nallocated memory  : %llu B, %llu KiB, %llu MiB, %llu GiB\n",
	 s,
	 s >> 10,
	 s >> 20,
	 s >> 30);

  //
  f64 t1 = 0.0, t2 = 0.0;
  
  //
  f64 *restrict a = aligned_alloc(64, s);

  init(a, n, 'c');

  //Sequential
  t1 = omp_get_wtime();
  
  f64 rs  = reduc_sequential(a, n);

  t2 = omp_get_wtime();  

  f64 elapsed_s = (f64)(t2 - t1);

  //Parallel
  t1 = omp_get_wtime();
  
  f64 rp  = reduc_openmp(a, n, nt);

  t2 = omp_get_wtime();

  f64 elapsed_p = (f64)(t2 - t1);
    
  printf("\nsequential result : %lf\n", rs);
  printf("sequential elapsed: %.5lf s\n", elapsed_s);
  printf("\nparallel result   : %lf\n", rp);
  printf("parallel elapsed  : %.5lf s\n", elapsed_p);
  
  f64 delta = fabs(rs - rp);
  f64 speedup = (elapsed_s / elapsed_p);
  
  printf("\nresults delta      : %lf (%e)\n", delta, delta);
  printf("speedup           : %.3lf\n", speedup);
  
  //
  free(a);
  
  //
  return 0;
}
