#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"


#define ERROR 1
#define OK 0

typedef struct grep_options_struct
{
	int value_for_a;
	//bool is_b_active;
	//bool is_c_active;
	//bool is_i_active;
	//bool is_n_active;
	//bool is_v_active;
	//bool is_x_active;
	int argv_index_for_target_str;
	int argv_index_for_file_name;
} grep_options_struct;

void get_grep_options(grep_options_struct *grep_options,char *argv[])
{
	//parsing of string to search and file name will be here

	grep_options->argv_index_for_target_str=1;
	grep_options->argv_index_for_file_name=2;
}

FILE* get_file(char *argv[], grep_options_struct grep_options)
{
	FILE *fp;

	if (grep_options.argv_index_for_file_name==-1)
		return stdin; 
	fp=fopen(argv[grep_options.argv_index_for_file_name],"r");
	if(fp==NULL)
		exit(ERROR);
	return fp;
}


int main(int argc, char *argv[])
{
	FILE *file;

	char **line=NULL;
	size_t n;
	char *pointer_to_searched_substring;
	grep_options_struct grep_options;


	
	get_grep_options(&grep_options,argv);
	file=get_file(argv,grep_options);


	//get_file_name(file_name,argv,index_for_file_name_in_argv);
	//=argv[2];

	//file=fopen(file_name,"r");

	getline(line,&n,file);
	printf("%s",line);
	while(!feof(file))
	{

		//test_line()


		//print if found
		pointer_to_searched_substring=strstr(*line,argv[grep_options.argv_index_for_target_str]);
		//if (!pointer_to_searched_substring==NULL)
			//printf("%s", *line);//will be inside a function so ok
		getline(line,&n,file);
	}

	free(line);
	fclose(file);
	return OK;


}



