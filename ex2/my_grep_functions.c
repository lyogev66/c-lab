#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_grep.h"

char* strcasestr(char* str1, char* str2);
void init_grep_arguments(grep_options_struct *grep_options);

void report_line_match(matched_struct *matched_line,char *line,grep_options_struct grep_options)
{
	matched_line->match_line=line;
	if(matched_line->match_found==TRUE){
			matched_line->number_of_lines_remained_to_print=1+grep_options.value_for_a;
			matched_line->number_of_matches+=matched_line->match_found;
	}
}

void read_line(FILE *file,char **line,matched_struct *match_line)
{
	size_t allocated_size_for_line,n_value_get_line;
	allocated_size_for_line=getline(line ,&n_value_get_line,file);
	match_line->file_bytes_counter+=allocated_size_for_line;
	match_line->line_counter++;
}

int is_match_in_line(char *line,grep_options_struct grep_options)
{
	int return_expresion=(grep_options.is_v_active==TRUE);

	if (grep_options.is_i_active==FALSE){
		if(grep_options.is_x_active==FALSE){
			if (strstr(line,grep_options.searched_str)==NULL){
				return (return_expresion);
			}
			else {
				return (!return_expresion);
			}
		}else{
			if (strcmp(line,grep_options.searched_str)==0){
				return (return_expresion);
			}
			else {
				return (!return_expresion);
			}
		}
	} else{
		if (strcasestr(line,grep_options.searched_str)==NULL){
			return(return_expresion);
		}
		else {
			return (!return_expresion);
		}
	}
}

void print_for_n_flag(matched_struct matched,grep_options_struct grep_options)
{
	if(grep_options.is_n_active==TRUE){
		if(matched.match_found==TRUE){
			printf("%d:",matched.line_counter);
		}else{
			printf("%d-",matched.line_counter);
		}
	}
}
void print_for_b_flag(matched_struct matched,grep_options_struct grep_options)
{

		if(grep_options.is_b_active==TRUE){
			printf("%ld:",((matched.file_bytes_counter)-strlen(matched.match_line)));
		}
}

void print_match(matched_struct matched,grep_options_struct grep_options)
{
	if(grep_options.is_c_active==FALSE){
		print_for_n_flag(matched,grep_options);
		print_for_b_flag(matched,grep_options);
		printf("%s",matched.match_line);
	}

}	

void init_grep_arguments(grep_options_struct *grep_options)
{
	grep_options->value_for_a=0;
	grep_options->is_b_active=FALSE;
	grep_options->is_c_active=FALSE;
	grep_options->is_i_active=FALSE;
	grep_options->is_n_active=FALSE;
	grep_options->is_v_active=FALSE;
	grep_options->is_x_active=FALSE;
	grep_options->searched_str=NULL;
	grep_options->file_name=NULL;
}

void get_grep_arguments(grep_options_struct *grep_options,char *argv[],int argc)
{
	int argc_index, search_term_found=FALSE, target_file_found=FALSE;
	init_grep_arguments(grep_options);


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
			grep_options->searched_str=argv[argc_index];
			search_term_found=TRUE;
			continue;
		}
		if (target_file_found==FALSE) {
			grep_options->file_name=argv[argc_index];
			target_file_found=TRUE;
			continue;
		}
	}
}

FILE * open_file_or_stdin(grep_options_struct grep_options)
{ 
	FILE *file=NULL;
	if (grep_options.file_name!=NULL){
		file=fopen(grep_options.file_name,"r");
		if (file==NULL){
			exit(1);
		}
	}else{
		file=stdin;
	}
	return file;
}

void init_match_line(matched_struct * matched_line)
{
	matched_line->line_counter=0;
	matched_line->number_of_matches =0;
	matched_line->match_found =FALSE;
	matched_line->match_line =NULL;
	matched_line->file_bytes_counter=0;
	matched_line->number_of_lines_remained_to_print=0;
}

char* strcasestr(char* str1, char* str2)
{
	char* p1 = str1;
	const char* p2 = str2;
	char* r = *p2 == 0 ? str1 : 0;

	while(*p1 != 0 && *p2 != 0)
	{
		if (tolower(*p1)==tolower(*p2)) {
			if (r==0)
				r=p1;
			p2++;
		}
		else {
			p2 = str2;
			if (tolower(*p1)==tolower(*p2)) {
				r = p1;
				p2++;
			}
			else
				r=0;
		}
		p1++ ;
	}
	return *p2 == 0 ? r : 0;
}

