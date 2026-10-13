/***************
* sandcrypt... *
****************
*
*  this program encrypts files with a password
*
*  files can be decrypted by using the same password
*  that the file was originally encrypted with
*
********/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  /* check for correct amount of args */
  if(argc != 4) {
    printf("usage: sandcrypt <file in> <file out> <password>\n");
    return 1;
  }

  /* make sure the input file is valid */
  if(access(argv[1], F_OK)){
    printf("file \"%s\" does not exist\n", argv[1]);
    return 1;
  }

  /* make sure nothing is overwritten */
  if(!access(argv[2], F_OK)){
    printf("file \"%s\" already exists\n", argv[2]);
    return 1;
  }

  /* initialize encryption */
  unsigned char state = 0xff;

  for (int i = 0;i < strlen(argv[3]); i++) {
    state += argv[3][i];
    state *= state;
    state *= 7;
    state += 2;
    state = state ^ argv[3][i];
  }

  /* initialize vars */
  unsigned char *buf = malloc(sizeof(unsigned char));
  FILE *in, *out;
  int i = 0;

  in = fopen(argv[1], "rb");
  out = fopen(argv[2], "wb");

  /* do the copying (byte by byte) */
  while(fread(buf, 1, 1, in)){
    i++;
    if(i > strlen(argv[3])) i = 0;
    state += argv[3][i];
    state *= state;
    state *= 7;
    state += 2;
    state = state ^ argv[3][i];
    *buf = state ^ *buf;
    fwrite(buf, 1, 1, out);
  }

  return 0;
}
