#include<stdio.h>
#include<stdlib.h>
#define BUFFER 1024

void Create_file()
{

	char input[BUFFER];
	FILE *file;
	file = fopen("text.txt", "w");

	while (fgets(input, BUFFER, stdin))
	{
		fputs(input, file);
	}
	fclose(file);
}
