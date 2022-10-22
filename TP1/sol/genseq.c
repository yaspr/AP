//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//
unsigned randxy(unsigned x, unsigned y)
{
  return (x + (rand() % y)) % y; 
}

//
int main(int argc, char **argv)
{
  //
  srand(getpid());
  
  //
  if (argc < 2)
    return printf("Usage: %s [output file] [length]\n", argv[0]), 1;

  //
  unsigned long long len = atoll(argv[2]);
  
  //
  const char bases[4] = "ATCG"; 
  
  //
  FILE *fp = fopen(argv[1], "wb");

  if (!fp)
    return printf("Error: cannot create file '%s'\n", argv[2]), 2;

  //Generate random DNA sequence
  for (unsigned long long i = 0; i < len; i++)
    fprintf(fp, "%c", bases[randxy(0, 4)]);

  //Newline at EOF
  fprintf(fp, "\n");
  
  //
  fclose(fp);
    
  //
  return 0;
}
