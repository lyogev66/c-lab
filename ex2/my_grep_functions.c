#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   ///tolower
#include "my_grep.h"
#include "helper.h" ///remove in unix

#define PAD_WITH_END_OF_STRING 1

void init_grep_arguments(grep_options_struct *grep_options);
int is_match_in_line(char *line,grep_options_struct grep_options);
void tolower_str(char * string);
int is_match_recursive_case(char * line_copy,char *search_string_copy);

void fill_line_descriptor(line_descriptor_struct *line_descriptor,char *line,grep_options_struct grep_options)
{
	int print_revers_lines=(grep_options.is_v_active==TRUE);
	line_descriptor->match_found=is_match_in_line(line,grep_options)?(!print_revers_lines):print_revers_lines;
	line_descriptor->current_line=line;
	if(line_descriptor->match_found==TRUE){
			line_descriptor->number_of_lines_remained_to_print=1+grep_options.value_for_a;
			line_descriptor->number_of_matches+=line_descriptor->match_found;
	}
}

void read_line(FILE *file,char **line,line_descriptor_struct *line_descriptor)
{
	size_t allocated_size_for_line,n_value_get_line;
	allocated_size_for_line=getline(line ,&n_value_get_line,file);
	line_descriptor->file_bytes_counter+=allocated_size_for_line;
	line_descriptor->line_counter++;
}

void create_copy_strings(char *line,grep_options_struct grep_options,char **line_copy,char **search_string_copy)
{
	*line_copy=(char*)malloc(sizeof(char)*strlen(line)+PAD_WITH_END_OF_STRING);
	*search_string_copy=(char*)malloc(sizeof(char)*strlen(grep_options.searched_str)+PAD_WITH_END_OF_STRING);
	if (line_copy==NULL||search_string_copy==NULL){
		printf("Memory allocation failed\n");
		exit(1);
	}
	strcpy(*line_copy,line);
	strcpy(*search_string_copy,grep_options.searched_str);
}

int is_match_in_line(char *line,grep_options_struct grep_options)
{
	int found_match=FALSE;
	unsigned int current_index;
	char *line_copy=NULL,*search_string_copy=NULL;
	create_copy_strings(line,grep_options,&line_copy,&search_string_copy);
	if (grep_options.is_i_active==TRUE){
		tolower_str(line_copy);
		tolower_str(search_string_copy);
	}
	if(grep_options.is_E_active==TRUE){
		for(current_index=0;current_index<=strlen(line_copy);current_index++)
		{
			found_match=is_match_recursive_case(line_copy+current_index,search_string_copy);
			if (found_match==TRUE){
				goto FREE_AND_RETURN;
			}
		}
	}
	if(grep_options.is_x_active==FALSE){
		found_match=(strstr(line_copy,search_string_copy)!=NULL);
	}else{
		found_match=(strncmp(line_copy,search_string_copy,strlen(search_string_copy))==0);
	}
FREE_AND_RETURN:
	free(line_copy);
	free(search_string_copy);
	return found_match;
}

void print_for_n_flag(line_descriptor_struct line_descriptor,grep_options_struct grep_options)
{
	if(grep_options.is_n_active==TRUE){
		if(line_descriptor.match_found==TRUE){
			printf("%d:",line_descriptor.line_counter);
		}else{
			printf("%d-",line_descriptor.line_counter);
		}
	}
}

void print_for_b_flag(line_descriptor_struct line_descriptor,grep_options_struct grep_options)
{
	size_t bytes_from_begginging=((line_descriptor.file_bytes_counter)-strlen(line_descriptor.current_line));

	if(grep_options.is_b_active==TRUE){
		if(line_descriptor.match_found==TRUE){
			printf("%ld:",bytes_from_begginging);
		}else{
			printf("%ld-",bytes_from_begginging);
		}
	}
}

void print_match(line_descriptor_struct line_descriptor,grep_options_struct grep_options)
{
	if(grep_options.is_c_active==FALSE){
		print_for_n_flag(line_descriptor,grep_options);
		print_for_b_flag(line_descriptor,grep_options);
		printf("%s",line_descriptor.current_line);
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
	grep_options->is_E_active=FALSE;
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
		}else  if (!strcmp(argv[argc_index],"-b")) {
			grep_options->is_b_active=TRUE;
		}else if (!strcmp(argv[argc_index],"-c")) {
			grep_options->is_c_active=TRUE;
		} else if (!strcmp(argv[argc_index],"-i")) {
			grep_options->is_i_active=TRUE;
		} else if (!strcmp(argv[argc_index],"-n")) {
			grep_options->is_n_active=TRUE;
		} else if (!strcmp(argv[argc_index],"-v")) {
			grep_options->is_v_active=TRUE;
		} else if (!strcmp(argv[argc_index],"-x")) {
			grep_options->is_x_active=TRUE;
		} else if (!strcmp(argv[argc_index],"-E")) {
			grep_options->is_E_active=TRUE;
		} else if (search_term_found==FALSE) {
			grep_options->searched_str=argv[argc_index];
			search_term_found=TRUE;
		} else if (target_file_found==FALSE) {
			grep_options->file_name=argv[argc_index];
			target_file_found=TRUE;
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

void init_line_descriptor_struct(line_descriptor_struct * line_descriptor)
{
	line_descriptor->line_counter=0;
	line_descriptor->number_of_matches =0;
	line_descriptor->match_found =FALSE;
	line_descriptor->current_line =NULL;
	line_descriptor->file_bytes_counter=0;
	line_descriptor->number_of_lines_remained_to_print=0;
}

void tolower_str(char * string)
{
	unsigned int char_index=0;

	for(char_index=0;char_index<=strlen(string);char_index++)
	{
		string[char_index]=tolower(string[char_index]);
	}
}

int is_match_recursive(char *line_copy,char *search_string_copy,int previous_char_backslash)
{
	char current_char_line=line_copy[0],current_char_searched_string=search_string_copy[0];
	int end_of_line=(line_copy[0]=='\0');

	if (search_string_copy[0]=='\0'){
		return TRUE;
	}
	if(!end_of_line && current_char_searched_string=='\\' && !previous_char_backslash){
		return is_match_recursive(line_copy,search_string_copy+1,TRUE);
	}else {
		if (!end_of_line && (current_char_searched_string==current_char_line)){
			return is_match_recursive(line_copy+1,search_string_copy+1,FALSE);
		}
	}
	return FALSE;
}

int is_match_recursive_case(char * line_copy,char *search_string_copy)
{
	int previous_char_backslash=FALSE;
	return is_match_recursive(line_copy,search_string_copy,previous_char_backslash);
}
