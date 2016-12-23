#include <stdio.h>
#include <stdlib.h>
#include "my_grep.h"
#include "my_grep_functions.h"

void main(int argc, char *argv[])
{
	FILE *file=NULL;
	line_descriptor_struct line_descriptor;
	grep_options_struct grep_options;
	char *line=NULL;

	if (argc<MIN_ARGUMENT_NUMBER){
		printf("NOT enough arguments\n"); 
		exit(0);
	}
	get_grep_arguments(&grep_options,argv,argc);
	file=open_file_or_stdin(grep_options);
	init_line_descriptor_struct(&line_descriptor);
	read_line(file,&line,&line_descriptor);
	while(!feof(file)){

		fill_line_descriptor(&line_descriptor,line,grep_options);
		if (line_descriptor.number_of_lines_remained_to_print>0){
			print_match(line_descriptor,grep_options);
			line_descriptor.number_of_lines_remained_to_print--;
		}
		read_line(file,&line,&line_descriptor);
	}
	if(grep_options.is_c_active==TRUE)	{
		printf("%d\n",line_descriptor.number_of_matches);
	}
	free(line);
	free(grep_options.regex_array);
	fclose(file);
}


