#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "my_grep.h"

void main(int argc, char *argv[])
{
	FILE *file=NULL;
	matched_struct matched_line;
	grep_options_struct grep_options;
	char *line=NULL;

	if (argc<MIN_ARGUMENT_NUMBER){
		printf("NOT enough arguments\n"); 
		exit(0);
	}
	get_grep_arguments(&grep_options,argv,argc);
	file=open_file_or_stdin(grep_options);

	init_match_line(&matched_line);
	read_line(file,&line,&matched_line);
	while(!feof(file)){

		matched_line.match_found=is_match_in_line(line,grep_options);
		report_line_match(&matched_line,line,grep_options);
		if (matched_line.number_of_lines_remained_to_print>0){
			print_match(matched_line,grep_options);
			matched_line.number_of_lines_remained_to_print--;
		}
		read_line(file,&line,&matched_line);
	}
	if(grep_options.is_c_active==TRUE)
	{
		printf("%d\n",matched_line.number_of_matches);
	}
	free(matched_line.match_line);
	fclose(file);
}


