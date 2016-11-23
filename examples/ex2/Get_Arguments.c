#include "Get_Arguments.h"
#include "Get_Arguments.h"

#include <stdlib.h>

void Initialize_array(arguments* arg)
{
	arg->LineOffset=0;
	arg->PrintLineNumbersOnly=0;
	arg->IgnoreUpperCase=0;
	arg->PrintLineNumbers=0;
	arg->NegativeExpression=0;
	arg->ExactMatch=0;
	arg->IsRegularExpression=0;
	arg->NumOfLinesToAppend=0;
	arg->FileName[0]='\0';
	arg->IsFromFile = 1;
}

void Fill_Arguments(char **argv,int argc, arguments *input_arguments)
{
	char current_arg;
	int i;
	Initialize_array(input_arguments);
	if(argc == 2 || argv[argc-2][0]=='-'){
		strcpy(input_arguments->Expression,argv[argc-1]);
		strcpy(input_arguments->FileName, "text.txt");
		input_arguments->IsFromFile = 0;
	}
	else{
		strcpy(input_arguments->FileName,argv[argc-1]);
		strcpy(input_arguments->Expression,argv[argc-2]);
	}

	for (i = 1;i<argc-2;i++)
	{
		current_arg = argv[i][1];
		switch (current_arg)
		{
			case 'A':
				i++;
				input_arguments->NumOfLinesToAppend = atoi(argv[i]);
				break;
			case 'b':
				input_arguments->LineOffset=1;
				break;
			case 'c':
				input_arguments->PrintLineNumbersOnly=1;
				break;
			case 'i':
				input_arguments->IgnoreUpperCase=1;
				break;		
			case 'n':
				input_arguments->PrintLineNumbers=1;
				break;		
			case 'v':
				input_arguments->NegativeExpression=1;
				break;		
			case 'x':
				input_arguments->ExactMatch=1;
				break;		
			case 'E':
				input_arguments->IsRegularExpression=1;
				break;					
			default:
				break;
		}
	}
}
