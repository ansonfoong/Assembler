#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getOpcode(char * str)
{
  if(strcmp(str, "halt") == 0)
    return 0;
  else if(strcmp(str, "add") == 0)
    return 1;
  else if(strcmp(str, "and") == 0)
    return 2;
  else if(strcmp(str, "divide") == 0)
    return 3;
  else if(strcmp(str, "multiply") == 0)
    return 4;
  else if(strcmp(str, "subtract") == 0)
    return 5;
  else if(strcmp(str, "or") == 0)
    return 6;
  else if(strcmp(str, "rightshift") == 0)
    return 7;
  else if(strcmp(str, "interrupt") == 0)
    return 8;
  else if(strcmp(str, "addimmediate") == 0)
    return 9;
  else
    return -1;
}

char getRegister(char * text)
{

  if(*text == 'r' || *text == 'R')
    text++;

  return atoi(text);
}

void setRegisters(int opcode, unsigned char * bytes, char * tokens[])
{

  if(opcode == 1 || opcode == 2 || opcode == 3 || opcode == 4 || opcode == 5 || opcode == 6)
  {
    char * registerOne = malloc(strlen(tokens[1]));
    char * registerTwo = malloc(strlen(tokens[2]));
    char * registerThree = malloc(strlen(tokens[3]));

    strcpy(registerOne, tokens[1]);
    strcpy(registerTwo, tokens[2]);
    strcpy(registerThree, tokens[3]);

    bytes[0] |= getRegister(registerOne);
    bytes[1] = getRegister(registerTwo) << 4 | getRegister(registerThree);
  }
  else if(opcode == 7) // if Opcode is 7, only uses 1 register and has tokens[0] as 10.
  {
    char * registerOne = malloc(strlen(tokens[1])); // Stores the register.
    strcpy(registerOne, tokens[1]); // r0 - r15 is stored in registerOne.

    bytes[0] |= getRegister(registerOne);

    printf("%d\n", getRegister(registerOne));
  }

}
int setBytes(int opcode, unsigned char * bytes, char * tokens[])
{

  char * ins = malloc(strlen(tokens[0]));

  if(opcode == 1) // If the instruction is ADD, then there are 3 registers.
  {
    bytes[0] = 0x10;
    setRegisters(1, bytes, tokens);
    printf("%x %x\n", bytes[0], bytes[1]);
    return 2;
  }
  else if(opcode == 2) // Instruction is "and", 3R.
  {
    bytes[0] = 0x20;
    setRegisters(2, bytes, tokens);
    printf("%x %x\n", bytes[0], bytes[1]);
    return 2;
  }
  else if(opcode == 3) // Instruction is "divide", 3R.
  {
    bytes[0] = 0x30;
    setRegisters(3, bytes, tokens);
    printf("%x %x\n", bytes[0], bytes[1]);
    return 2;
  }
  else if(opcode == 4) // Instruction is "Multiply", 3R.
  {
    bytes[0] = 0x40;
    setRegisters(4, bytes, tokens);
    printf("%x %x\n", bytes[0], bytes[1]);
    return 2;
  }
  else if(opcode == 5) // Subtract. 3R
  {
    bytes[0] = 0x50;
    setRegisters(5, bytes, tokens);
    printf("%x %x\n", bytes[0], bytes[1]);
    return 2;
  }
  else if(opcode == 6) // OR. 3R.
  {
    bytes[0] = 0x60;
    setRegisters(6, bytes, tokens);
    printf("%x %x\n", bytes[0], bytes[1]);
    return 2;
  }
  else if(opcode == 7) // Shift Right. sft.
  {
    bytes[0] = 0x70;
    setRegisters(7, bytes, tokens);
    return 2;
  }
  else
    return -1;
}

int stringTrim(char * str, unsigned char * bytes)
{
  char * copiedAssemblyLine = malloc(strlen(str));
  char * arrayOfTokens[5];
  int i = 0;
  strcpy(copiedAssemblyLine, str);
  arrayOfTokens[i] = strtok(copiedAssemblyLine, " ");
  while(arrayOfTokens[i] != NULL) {
    //printf("Tokens: %s\n", arrayOfTokens[i]);
    arrayOfTokens[++i] = strtok(NULL, " ");
  }
  //int k = 0;
  //printf("Instruction: %s\nOpcode: %d\n", arrayOfTokens[0], getOpcode(arrayOfTokens[0]));
  int opcodeOfInstruction = getOpcode(arrayOfTokens[0]); // Get the opcode by passing in the Instruction String.
  int byteCount = setBytes(opcodeOfInstruction, bytes, arrayOfTokens); // Pass in the bytes array and opcode.

  return byteCount;
}

int main(int argc, char ** argv)
{
  if(argc != 3)
  {
    printf("%s\n", "Invalid number of arguments.");
    exit(0);
  }
  else {
    printf("Correct number of arguments.\n");
    printf("Opening %s for reading and opening %s for writing.\n", argv[1], argv[2]);
    FILE * input = fopen(argv[1], "r");
    FILE * output = fopen(argv[2], "w");

    if(input == NULL) // Check if the File is NULL.
    {
      printf("%s\n", "Error...");
    } else { // If File Pointer is not NULL, then loop the file.
      int i = 0;
      char line[1000];
      while(fgets(line, 1000, input) != NULL)
      {
        unsigned char bytes[4];
        //printf("Line: %s\n", line);
        int byteCount = stringTrim(line, bytes);
        printf("Writing %x ", bytes[0]);
        printf("Writing %x\n", bytes[1]);
        printf("Byte Count: %d\n", byteCount);
        // fwrite(bytes, byteCount, 100, output);
        fprintf(output, "%x ", bytes[0]);
        fprintf(output, "%x\n", bytes[1]);
        i++;
      }
      //printf("%d\n", i);

      fclose(input);
      fclose(output);
    }
  }

  return 0;
}
