#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"


#define TRUE 1
#define FALSE 0
#define ERROR 1
#define OK 0
#define FUNCTION_RETURN_ERROR 1
#define FUNCTION_RETURN_OK 0
#define NO_INDEX -1
#define PRINT_CURRENT_LINE 1
#define	DONT_PRINT_CURRENT_LINE 0

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
	grep_options->value_for_a=0;
	grep_options->is_b_active=FALSE;
	grep_options->is_c_active=FALSE;
	grep_options->is_i_active=FALSE;
	grep_options->is_n_active=FALSE;
	grep_options->is_v_active=FALSE;
	grep_options->is_x_active=FALSE;
	grep_options->argv_index_for_target_str=NO_INDEX;
	grep_options->argv_index_for_file_name=NO_INDEX;
}

void get_grep_options(grep_options_struct *grep_options,char *argv[],int argc)
{
	int argc_index, search_term_found=FALSE, target_file_found=FALSE;
	init_grep_options(grep_options);
	//grep_options->argv_index_for_file_name=NO_INDEX;

	for (argc_index=1;argc_index<argc;argc_index++)
	{
		if (!strcmp(argv[argc_index],"-A")) {
			argc_index++;
			grep_options->value_for_a=atoi(argv[argc_index]);
			continue;
		}
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

FILE* open_file_or_open_stdin(char *argv[], grep_options_struct grep_options)
{
	FILE *fp;

	if (grep_options.argv_index_for_file_name==NO_INDEX)
		return stdin; 
	fp=fopen(argv[grep_options.argv_index_for_file_name],"r");
	if(fp==NULL)
		exit(FUNCTION_RETURN_ERROR);//switch to assert
	return fp;
}

int take_care_of_v_case(char *pointer_to_searched_substring,grep_options_struct grep_options)
{
	if(grep_options.is_v_active==TRUE)
		if(pointer_to_searched_substring==NULL)
			return PRINT_CURRENT_LINE;
		else
			return DONT_PRINT_CURRENT_LINE;
	else
		if(pointer_to_searched_substring==NULL)
			return DONT_PRINT_CURRENT_LINE;
		else
			return PRINT_CURRENT_LINE;


}
int take_care_of_b_case(grep_options_struct grep_options)
{
	if (grep_options.is_b_active==TRUE)
		return TRUE;
	return FALSE;

}


int main(int argc, char *argv[])
{
	int line_number,go_into_print;
	FILE *file;
	char *line=NULL;
	size_t n=0;
	char *pointer_to_searched_substring;
	grep_options_struct grep_options;
	

	get_grep_options(&grep_options,argv,argc);
	file=open_file_or_open_stdin(argv,grep_options);


	line_number=0;
	getline(&line,&n,file);
	while(!feof(file))
	{

		//test_line()


		//print if found
		pointer_to_searched_substring=strstr(line,argv[grep_options.argv_index_for_target_str]);
		go_into_print=take_care_of_v_case(pointer_to_searched_substring,grep_options);
		if (go_into_print==PRINT_CURRENT_LINE)
		{
			if (take_care_of_b_case(grep_options)==TRUE)
				printf("%d:",line_number);

			printf("%s", line);//will be inside a function so ok
		}
		getline(&line,&n,file);
		line_number++;
	}


	free(line);
	fclose(file);
	return OK;

}