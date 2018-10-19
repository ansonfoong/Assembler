#include <stdio.h>
#include <stdlib.h>

int main() {

  FILE * binaryFile = fopen("out", "rb");
  if(binaryFile == NULL)
    exit(0);
  else {
    int c;
    while((c = fgetc(binaryFile)) != EOF)
    {
      printf("%02x ", c);
    }
  }

}
