#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"


#define TRUE 1
#define FALSE 0
#define ERROR 1
#define OK 0

typedef struct grep_options_struct
{
	int value_for_a;
	int is_b_active;
	int is_c_active;
	int is_i_active;
	int is_n_active;
	int is_v_active;
	int is_x_active;
	int argv_index_for_target_str;
	int argv_index_for_file_name;
} grep_options_struct;


void init_grep_options(grep_options_struct *grep_options)
{
	grep_options->is_b_active=FALSE;
	grep_options->is_c_active=FALSE;
	grep_options->is_i_active=FALSE;
	grep_options->is_n_active=FALSE;
	grep_options->is_v_active=FALSE;
	grep_options->is_x_active=FALSE;
	grep_options->argv_index_for_target_str=-1;
	grep_options->argv_index_for_file_name=-1;
}

void get_grep_options(grep_options_struct *grep_options,char *argv[],int argc)
{
	int argc_index, search_term_found=FALSE, target_file_found=FALSE;
	for (argc_index=1;argc_index<argc;argc_index++)
	{
		printf("%s",argv[argc_index]);

		if (!strcmp(argv[argc_index],"-b")) {
			grep_options->is_b_active=TRUE;
			continue;
		}
		if (!strcmp(argv[argc_index],"-c")) {
			grep_options->is_c_active=TRUE;
			continue;
		}
		if (!strcmp(argv[argc_index],"-i")) {
			grep_options->is_i_active=TRUE;
			continue;
		}
		if (!strcmp(argv[argc_index],"-n")) {
			grep_options->is_n_active=TRUE;
			continue;
		}
		if (!strcmp(argv[argc_index],"-v")) {
			grep_options->is_v_active=TRUE;
			continue;
		}
		if (!strcmp(argv[argc_index],"-x")) {
			grep_options->is_x_active=TRUE;
			continue;
		}
		if (search_term_found==FALSE) {
			grep_options->argv_index_for_target_str=argc_index;
			search_term_found=TRUE;
			continue;
		}
		if (target_file_found==FALSE) {
			grep_options->argv_index_for_file_name=argc_index;
			target_file_found=TRUE;
			continue;
		}
	}
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
	

	get_grep_options(&grep_options,argv,argc);
	file=get_file(argv,grep_options);


	getline(line,&n,file);
	while(!feof(file))
	{

		//test_line()


		//print if found
		pointer_to_searched_substring=strstr(*line,argv[grep_options.argv_index_for_target_str]);
		if (!pointer_to_searched_substring==NULL)
			printf("%s", *line);//will be inside a function so ok
		getline(line,&n,file);
	}

	free(line);
	fclose(file);
	return OK;



}
