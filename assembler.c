#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char *ltrim(char *s) { 
	while (*s == ' ' || *s == '\t') s++;
	return s;
}
char getRegister(char *text) {
	if (*text == 'r' || *text=='R') text++;
	return atoi(text);
}
int assembleLine(char *text, unsigned char* bytes) {
	text = ltrim(text);
	char *keyWord = strtok(text," ");
	if(strcmp(keyWord, "\n") == 0) // If the line is equal to the \n, return -1 to handle empty lines. 
		return -1;
	if(strcmp("halt\n", keyWord) == 0) // 3R Instruction, 2 Bytes
	{	
		bytes[0] = bytes[1] = 0x00;
		return 2;
	}
	else if (strcmp("add",keyWord) == 0) { // 3R Instruction, 2 Bytes
		bytes[0] = 0x10 | getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}
	else if(strcmp("and", keyWord) == 0) // 3R Instruction, 2 Bytes
	{
		bytes[0] = 0x20 | getRegister(strtok(NULL, " ")); // Set the OPCODE to 2 and OR it with the Register Number.
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " ")); // Get Register, Shift Left by 4 bits.
		return 2;
	}
	else if(strcmp("divide", keyWord) == 0) // 3R Instruction, 2 Bytes
	{
		bytes[0] = 0x30 | getRegister(strtok(NULL, " ")); // Set the OPCODE to 3, and then OR it with the Register Number.
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("multiply", keyWord) == 0) // 3R Instruction, 2 Bytes
	{
		bytes[0] = 0x40 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("subtract", keyWord) == 0) // 3R Instruction, 2 Bytes
	{
		bytes[0] = 0x50 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("or", keyWord) == 0) // 3R Instruction, 2 Bytes
	{
		bytes[0] = 0x60 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("rightshift", keyWord) == 0) // sft instruction, 2 Bytes
	{
		bytes[0] = 0x70 | getRegister(strtok(NULL, " "));
		bytes[1] = 0x20 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("leftshift", keyWord) == 0) // sft instruction, 2 Bytes
	{
		bytes[0] = 0x70 | getRegister(strtok(NULL, " "));
		bytes[1] = 0x00 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("interrupt", keyWord) == 0)
	{
		bytes[0] = 0x80;
		int bit = getRegister(strtok(NULL, " ")); // Store it once so we can perform two separate operations.
		bytes[0] |= (bit >> 8); // We want the top 4 of 12 bits, so shift right by 8.
		bytes[1] = bit & 0xFF; // AND the bottom 8 bits.
		return 2;
	}
	else if(strcmp("addimmediate", keyWord) == 0)
	{	
		bytes[0] = 0x90 | getRegister(strtok(NULL, " ")); // OPCODE 9, OR it with the first Register.
		bytes[1] = 0x00 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("branchifequal", keyWord) == 0)
	{
		bytes[0] = 0xA0 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4;
		int temp = atoi(strtok(NULL, " "));
		bytes[1] |= (temp >> 16); // Shift 16
		bytes[2] = (temp >> 8);
		bytes[3] = temp & (0xFF);
		return 4;
	}
	else if(strcmp("branchifless", keyWord) == 0)
	{
		bytes[0] = 0xB0 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4;
		int temp = atoi(strtok(NULL, " "));
		bytes[1] |= (temp >> 16); // Shift 16
		bytes[2] = (temp >> 12);
		bytes[3] = temp & (0xFF);
		return 4;
	}
	else if(strcmp("jump", keyWord) == 0)
	{
		bytes[0] = 0xC0;
		int offset = atoi(strtok(NULL, " " ));
		bytes[0] |= (offset >> 24) & 0xF;
		bytes[1] = (offset >> 16) & (0xFF);
		bytes[2] = (offset >> 8) & 0xFF;
		bytes[3] = offset & (0xFF);
		return 4;
	}
	else if(strcmp("iterateover", keyWord) == 0)
	{
		bytes[0] = 0xD0 | getRegister(strtok(NULL, " "));
		bytes[1] = 0x00 | atoi(strtok(NULL, " "));
		int offset = atoi(strtok(NULL, " "));
		bytes[2] = (offset >> 8) & 0xFF;
		bytes[3] = (offset & 0xFF);
		return 4;
	}
	else if(strcmp("load", keyWord) == 0)
	{
		bytes[0] = 0xE0 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else if(strcmp("store", keyWord) == 0)
	{
		bytes[0] = 0xF0 | getRegister(strtok(NULL, " "));
		bytes[1] = getRegister(strtok(NULL, " ")) << 4 | getRegister(strtok(NULL, " "));
		return 2;
	}
	else
		return -1;
}
int main(int argc, char **argv) {

	if(argc != 3)
	{
		printf("%s\n", "Invalid amount of arguments.");
		exit(0);
	}
	else {
		FILE *src = fopen(argv[1],"r");
		FILE *dst = fopen(argv[2],"w");
		if(src != NULL)
		{
			while (!feof(src)) {
				unsigned char bytes[4];
				char line[1000];
				printf ("about to read\n");
				if (NULL != fgets(line, 1000, src)) {
					printf ("read: %s\n", line);
					int byteCount = assembleLine(line,bytes);
					if(byteCount == -1)
						printf("Invalid\n");
					else
						fwrite(bytes,byteCount,1,dst);
				}
			}
		}
		else
			printf("Error opening file.\n");
		fclose(src);
		fclose(dst);
	}
	return 0;
}
