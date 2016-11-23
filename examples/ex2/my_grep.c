#include "Get_Arguments.h"
#include "Text_Input.h"

int main(int argc,char** argv)
{
	arguments arg;
	FILE *file;
		
	Fill_Arguments(argv,argc,&arg);
	if (!arg.IsFromFile)
	{
		Create_file();
	}

	file = fopen(arg.FileName, "r");
	
	if(file==NULL)
	{
		return;
	}
	
	print_lines(file,&arg);
	fclose(file);
}

