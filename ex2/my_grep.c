#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_grep.h"
#include "helper.h"


int main(int argc, char *argv[])
{
	int line_number=0,go_into_print;
	FILE *file;
	size_t n_value_for_getline=0;
	char *pointer_to_searched_substring,*line=NULL;
	grep_options_struct grep_options;


	get_grep_options(&grep_options,argv,argc);
	file=open_file_or_open_stdin(argv,grep_options);

	getline(&line,&n_value_for_getline,file);
	while(!feof(file))
	{

		pointer_to_searched_substring=strstr(line,argv[grep_options.argv_index_for_target_str]);
		go_into_print=take_care_of_v_case(pointer_to_searched_substring,grep_options);
		if (go_into_print==PRINT_CURRENT_LINE)
		{
			if (take_care_of_b_case(grep_options)==TRUE)
				printf("%d:",line_number);
			printf("%s", line);//will be inside a function so ok
		}
		getline(&line,&n_value_for_getline,file);
		line_number++;
	}

	free(line);
	fclose(file);
	return FUNCTION_RETURN_OK;

}


