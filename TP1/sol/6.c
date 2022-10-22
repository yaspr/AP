//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

//
typedef unsigned char      u8;
typedef unsigned long long u64;

//
typedef struct {

  //
  u64 w;
  u64 h;

  //
  u64 t;
  
  //
  u8 *pixels;

} ppm_t;

//Loads a PPM file with pixels stored in binary format P6
//Netpbm format details can be found here: https://en.wikipedia.org/wiki/Netpbm#Description
ppm_t *load_ppm(const char *fname)
{
  //
  if (!fname)
    return printf("Error: file name is NULL"), NULL;

  //
  u64 w = 0;
  u64 h = 0;
  u64 t = 0;
  
  //
  FILE *fp = fopen(fname, "rb");

  if (!fp)
    return printf("Error: cannot open file '%s'\n", fname), NULL;

  //Create PPM holder
  ppm_t *p = malloc(sizeof(ppm_t));
  
  if (!p)
    return printf("Error: cannot allocate memory for ppm file\n"), NULL;

  //
  u8 id1 = 0, id2 = 0;
  
  fscanf(fp, "%c%c\n", &id1, &id2);

  if (id1 != 'P' || id2 != '6')
    return printf("Error: only PPM P6 binary format is handled\n"), NULL;
  
  //Read width and height of the image
  fscanf(fp, "%llu %llu\n", &w, &h);

  //Reading threshold
  fscanf(fp, "%llu\n", &t);
  
  //
  p->w = w;
  p->h = h;
  p->t = t;
  
  //Pixels are stored in RGB (3 bytes), hence the w * h * 3.
  p->pixels = malloc(sizeof(u8) * w * h * 3);

  if (!p->pixels)
    return printf("Error: cannot allocate memory for pixels\n"), NULL;

  //
  size_t read_bytes = fread(p->pixels, sizeof(u8), w * h * 3, fp);

  //
  fclose(fp);
  
  if (read_bytes != (w * h * 3))
    return printf("Error: mismatch between read bytes and image resolution\n"), NULL;
  
  //
  return p;
}

//
ppm_t *create_ppm(u64 w, u64 h, u64 t)
{
  //
  ppm_t *p = malloc(sizeof(ppm_t));

  if (!p)
    return printf("Error: cannot allocate memory for ppm\n"), NULL;
  
  //
  p->w = w;
  p->h = h;
  p->t = t;
  
  //
  p->pixels = malloc(sizeof(u8) * w * h * 3);

  if (!p->pixels)
    return printf("Error: cannot allocate memory for pixels\n"), NULL;

  //First touch through initialization
  memset(p->pixels, 0, w * h *3);
  
  //
  return p;
}

//
void write_ppm(ppm_t *p, const char *fname)
{
  //
  if (!p || !fname)
    printf("Error: pointer is NULL"), exit(-1);

  //
  FILE *fp = fopen(fname, "wb");

  if (!fp)
    printf("Error: cannot create file '%s'\n", fname), exit(-1);

  //Writing format identifier
  fprintf(fp, "P6\n");
  
  //Writing image dimensions
  fprintf(fp, "%llu %llu\n", p->w, p->h);

  //
  fprintf(fp, "%llu\n", p->t);
  
  //Writing pixels in binary format
  fwrite(p->pixels, sizeof(u8), p->w * p->h * 3, fp);

  //
  fclose(fp);
}

//
void release_ppm(ppm_t *p)
{
  if (p)
    {
      if (p->pixels)
	free(p->pixels);

      p->w = 0;
      p->h = 0;
    }
  else
    {
      printf("Error: pointer is NULL\n");
      exit(-1);
    }
}

//Inverts the pixels
void invert_c(u8 *in, u64 len, u8 *out)
{
  //
  for (u64 i = 0; i < len; i++)
    out[i] = 255 - in[i];
}

//
void invert_asm(u8 *in, u64 len, u8 *out)
{
  //
  __asm__ volatile(
		   "xor %%rcx, %%rcx;\n"

		   "1:;\n"

		   "movq (%[_in], %%rcx), %%rax;\n"
		   "not %%rax;\n"
		   "movq %%rax, (%[_out], %%rcx);\n"
		   
		   "add $8, %%rcx;\n"
		   "cmp %[_s], %%rcx;\n"
		   "jl 1b;\n"
		   
		   : //outputs

		   : //inputs
		     [_in]  "r" (in),
		     [_out] "r" (out),
		     [_s]   "r" (len)
		     
		   : //clobber
		     "cc", "memory", "rcx"
		   );
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 2)
    return printf("Usage: %s [ppm input image]\n", argv[0]), 1;

  //
  ppm_t *p_in = load_ppm(argv[1]);
  
  if (!p_in)
    exit(-1);

  //
  ppm_t *p_out_c = create_ppm(p_in->w, p_in->h, p_in->t);

  if (!p_out_c)
    exit(-1);

  //
  ppm_t *p_out_asm = create_ppm(p_in->w, p_in->h, p_in->t);

  if (!p_out_asm)
    exit(-1);

  //
  invert_c(p_in->pixels, p_in->w * p_in->h * 3, p_out_c->pixels);

  invert_asm(p_in->pixels, p_in->w * p_in->h * 3, p_out_asm->pixels);

  //
  write_ppm(p_out_c, "out_c.ppm");
  write_ppm(p_out_asm, "out_asm.ppm");

  //
  release_ppm(p_in); free(p_in);
  release_ppm(p_out_c); free(p_out_c);
  release_ppm(p_out_asm); free(p_out_asm);
  
  //
  return 0;
}
