#ifndef ARGUMENTS
#define ARGUMENTS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct arg
{
	char LineOffset;
	char PrintLineNumbersOnly;
	char IgnoreUpperCase;
	char PrintLineNumbers;
	char NegativeExpression;
	char ExactMatch;
	char IsRegularExpression;
	char IsFromFile;
	int NumOfLinesToAppend;
	char Expression[1024];
	char FileName[1024];

 }arguments;


void Initialize_array(arguments* arg);
void Fill_Arguments(char **argv,int argc,arguments *input_arguments);
char IsMatchRegularExpression(arguments *arg, int expressionIndex, char *resolvedExpression, 
	int resolvedIndex, char *line);

#endif
