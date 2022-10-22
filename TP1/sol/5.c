//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

//Defining error codes
#define ERR_FNAME_NULL   0
#define ERR_MALLOC_NULL  1
#define ERR_STAT         3
#define ERR_OPEN_FILE    4
#define ERR_READ_BYTES   5
#define ERR_NULL_POINTER 6

//
#define REPS 10000000

//
typedef unsigned char      u8;
typedef unsigned long long u64;

//Sequence definitions
typedef struct {

  //Sequence elements/bytes
  u8 *bases;

  //Sequence length
  u64 len;

} seq_t;

//Global error variable
u64 err_id = 0;

//Error messages
const char *err_msg[] = {

  "file name pointer NULL",
  "memory allocation fail, 'malloc' returned NULL",
  "cannot 'stat' file",
  "cannot open file, 'fopen' returned NULL",
  "mismatch between read bytes and file length",
  
  NULL
};

//
static inline u64 rdtsc()
{
  u64 a, d;
  
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));

  return ((d << 32) | a);
}

//
void error()
{
  //
  printf("Error (%llu): %s\n", err_id, err_msg[err_id]);
  
  //
  exit(-1);
}

//
seq_t *load_seq(const char *fname)
{
  //
  if (!fname)
    {
      err_id = ERR_FNAME_NULL;
      return NULL;
    }

  //
  struct stat sb;

  if (stat(fname, &sb) < 0)
    {
      err_id = ERR_STAT;
      return NULL;
    }
  
  //Allocate sequence 
  seq_t *s = malloc(sizeof(seq_t));
  
  if (!s)
    {
      err_id = ERR_MALLOC_NULL;
      return NULL;
    }
  
  //Length of sequence is file size in bytes
  s->len = sb.st_size;

  //Allocating memory for sequence bases
  s->bases = malloc(sizeof(u8) * sb.st_size);
  
  if (!s->bases)
    {
      err_id = ERR_MALLOC_NULL;
      return NULL;
    }

  //Opening the file
  FILE *fp = fopen(fname, "rb");

  if (!fp)
    {
      err_id = ERR_OPEN_FILE;
      return NULL;
    }

  //Reading bytes from file
  size_t read_bytes = fread(s->bases, sizeof(u8), s->len, fp);

  //Closing file
  fclose(fp);

  //Check if bytes were fully read
  if (read_bytes != s->len)
    {
      err_id = ERR_READ_BYTES;
      return NULL;
    }
  
  //
  return s;
}

//
void release_seq(seq_t *s)
{
  //
  if (s)
    {
      //
      if (s->bases)
	free(s->bases);
      else
	err_id = ERR_NULL_POINTER;
	  
      //
      s->len = 0;
    }
  else
    err_id = ERR_NULL_POINTER;
}

//
u64 hamming_c(u8 *a, u8 *b, u64 n)
{
  //
  u64 h = 0;

  //
  for (u64 i = 0; i < n; i++)
    h += __builtin_popcount(a[i] ^ b[i]);
  
  //
  return h;
}

//
u64 hamming_asm_8bits(u8 *a, u8 *b, u64 n)
{
  //
  u64 h = 0;

  //Size in bytes
  u64 sizeb = n;
  
  //
  __asm__ volatile (
		    "xor %%rax, %%rax;\n"
		    "xor %%rbx, %%rbx;\n"
		    "xor %%rcx, %%rcx;\n"
		    "xor %%rdx, %%rdx;\n"

		    "1:;\n"

		    //Loading bytes
		    "movb (%[_a], %%rcx), %%al;\n" 
		    "movb (%[_b], %%rcx), %%bl;\n"

		    //Xoring bytes
		    "xor %%bl, %%al;\n"

		    //Popcount on 16bits (no 8 bits popcount on x86)
		    "popcnt %%ax, %%bx;\n"
		    
		    "add %%rbx, %%rdx;\n"
		    
		    "add $1, %%rcx;\n"
		    "cmp %[_sizeb], %%rcx;\n"
		    "jl 1b;\n"
		    
		    "mov %%rdx, %[_h];\n"
		    
		    : //outputs
		      [_h] "=r" (h)
		      
		    : //inputs
		      [_a]     "r" (a),
		      [_b]     "r" (b),
		      [_sizeb] "r" (sizeb)
		      
		    :
		    "cc", "memory", "rax", "rbx", "rcx", "rdx");
  
  //
  return h;
}

//
u64 hamming_asm_64bits(u8 *a, u8 *b, u64 n)
{
  //
  u64 h = 0;

  //Size in bytes
  u64 sizeb = n;
  
  //
  __asm__ volatile (
		    "xor %%rcx, %%rcx;\n"
		    "xor %%rdx, %%rdx;\n"

		    "1:;\n"

		    //Loading 64bit blocks rather than bytes
		    "movq (%[_a], %%rcx), %%rax;\n" 
		    "movq (%[_b], %%rcx), %%rbx;\n"

		    //
		    "xor %%rbx, %%rax;\n"
		    "popcnt %%rax, %%rbx;\n"

		    "add %%rbx, %%rdx;\n"
		    
		    "add $8, %%rcx;\n"
		    "cmp %[_sizeb], %%rcx;\n"
		    "jl 1b;\n"

		    "mov %%rdx, %[_h];\n"
		    
		    : //outputs
		      [_h] "=r" (h)
		      
		    : //inputs
		      [_a]     "r" (a),
		      [_b]     "r" (b),
		      [_sizeb] "r" (sizeb)
		      
		    :
		    "cc", "memory", "rax", "rbx", "rcx", "rdx");
  
  //
  return h;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [seq1] [seq2]\n", argv[0]), 1;
  
  //Loading first sequence
  seq_t *s1 = load_seq(argv[1]);

  if (!s1)
    error();

  //Loading second sequence
  seq_t *s2 = load_seq(argv[2]);

  if (!s2)
    error();
  
  //Check size
  if (s1->len != s2->len)
    return printf("Error: sequences must match in length"), 2;
  
  //
  u64 h_c     = 0; 
  u64 h_asm8  = 0; 
  u64 h_asm64 = 0; 

  //
  u64 a = 0, b = 0;
  u64 e_c = 0, e_asm8 = 0, e_asm64 = 0;
  
  //C
  b = rdtsc();
  
  for (u64 i = 0; i < REPS; i++)
    h_c     = hamming_c(s1->bases, s2->bases, s1->len);

  a = rdtsc();
  e_c = a - b;

  //ASM 8 bits
  b = rdtsc();
  
  for (u64 i = 0; i < REPS; i++)
    h_asm8 = hamming_asm_8bits(s1->bases, s2->bases, s1->len);

  a = rdtsc();
  e_asm8 = a - b;

  
  //ASM 8 bits
  b = rdtsc();
  
  for (u64 i = 0; i < REPS; i++)
    h_asm64 = hamming_asm_64bits(s1->bases, s2->bases, s1->len);

  a = rdtsc();
  e_asm64 = a - b;
  
  //
  printf("hamming_c    : %llu, cycles: %llu\n", h_c    , e_c     / REPS);
  printf("hamming_asm8 : %llu, cycles: %llu\n", h_asm8 , e_asm8  / REPS);
  printf("hamming_asm64: %llu, cycles: %llu\n", h_asm64, e_asm64 / REPS);
  
  //
  release_seq(s1); free(s1);
  release_seq(s2); free(s2);
  
  //
  return 0;
}
