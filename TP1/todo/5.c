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
u64 hamming(u8 *a, u8 *b, u64 n)
{
  //
  u64 h = 0;

  //
  for (u64 i = 0; i < n; i++)
    h += (a[i] ^ b[i]);

  //
  return h;
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [seq1] [seq2]\n", argv[0]), 1;
  
  //
  seq_t *s1 = load_seq(argv[1]);

  if (!s1)
    error();

  //
  seq_t *s2 = load_seq(argv[2]);

  if (!s2)
    error();
  
  //
  if (s1->len != s2->len)
    return printf("Error: sequences must match in length"), 2;
  
  //
  u64 h = hamming(s1->bases, s2->bases, s1->len);

  printf("hamming: %llu\n", h);
  
  //
  release_seq(s1); free(s1);
  release_seq(s2); free(s2);
  
  //
  return 0;
}
