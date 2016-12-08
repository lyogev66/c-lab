#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
//#include "my_grep.h"
#include "helper.h"

#define MIN_ARGUMENT_NUMBER		2
#define TRUE					1
#define FALSE					0
#define FUNCTION_RETURN_ERROR	1
#define FUNCTION_RETURN_OK		0
#define NO_INDEX				-1
#define PRINT_CURRENT_LINE		1
#define	DONT_PRINT_CURRENT_LINE	0

typedef struct grep_options_struct
{
	int value_for_a;
	int is_b_active;
	int is_c_active;
	int is_i_active;
	int is_n_active;
	int is_v_active;
	int is_x_active;
	char *searched_str;
	char *file_name;
} grep_options_struct;

typedef struct matched_struct
{
	char *match_line;
	size_t file_bytes_counter;
	int match_found,line_counter,number_of_matches,number_of_lines_remained_to_print;
}matched_struct;

void report_line_match(matched_struct *matched_line,char *line,grep_options_struct grep_options);
void read_line(FILE *file,char *line,matched_struct *match_line);
int is_match_in_line(char *line,grep_options_struct grep_options);
void print_match(matched_struct matched,grep_options_struct grep_options);
void init_grep_arguments(grep_options_struct *grep_options);
void get_grep_arguments(grep_options_struct *grep_options,char *argv[],int argc);
FILE * open_file_or_stdin(grep_options_struct grep_options);
void init_match_line(matched_struct *matched_line);
char* strcasestr(char* str1, char* str2);
//int take_care_of_b_case(grep_options_struct grep_options);
//int take_care_of_c_case(grep_options_struct grep_options);
//int take_care_of_i_case(grep_options_struct grep_options);
//int take_care_of_n_case(grep_options_struct grep_options);



void main(int argc, char *argv[])
{
	FILE *file=NULL;
	matched_struct matched_line;
	size_t alocated_size_for_line=0;
	grep_options_struct grep_options;
	char *pointer_to_searched_substring,*line=NULL;

	if (argc<MIN_ARGUMENT_NUMBER){
		printf("NOT enough arguments\n"); //ned to handle
		exit(0);
	}
	get_grep_arguments(&grep_options,argv,argc);
	file=open_file_or_stdin(grep_options);

	init_match_line(&matched_line);
	while(!feof(file)){
		read_line(file,&line,&matched_line);
		//if(==-1){
		//	assert("getline failed\n");
		//	//printf("getline failed or empty file\n");
		//	goto EXIT1;
		//}

		matched_line.match_found=is_match_in_line(line,grep_options);
		report_line_match(&matched_line,line,grep_options);
		if (matched_line.number_of_lines_remained_to_print>0){
			print_match(matched_line,grep_options);
			matched_line.number_of_lines_remained_to_print--;
		}
	}
	if(grep_options.is_c_active==TRUE)
	{
		printf("%d\n",matched_line.number_of_matches );
	}
	free(matched_line.match_line);
	fclose(file);
}


void report_line_match(matched_struct *matched_line,char *line,grep_options_struct grep_options)
{
	if(matched_line->match_found==TRUE){
			matched_line->match_line=line;
			matched_line->number_of_lines_remained_to_print=1+grep_options.value_for_a;
			matched_line->number_of_matches+=matched_line->match_found;
	}
}

void read_line(FILE *file,char **line,matched_struct *match_line)
{
	size_t allocated_size_for_line,n_value_get_line;
	int get_line_return_value;
	allocated_size_for_line=getline(line ,&n_value_get_line,file);
	//if (get_line_return_value==-1)	{
	//	printf("getline failed\n");
	//	exit(1);
	//}
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
			if (strcmp(line,grep_options.searched_str)==NULL){
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

void print_match(matched_struct matched,grep_options_struct grep_options)
{
	if(grep_options.is_c_active==FALSE){
		if(grep_options.is_n_active==TRUE){
			printf("%d\:",matched.line_counter);
		}
		if(grep_options.is_b_active==TRUE){
			printf("%d\:",matched.file_bytes_counter);
		}
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






//int take_care_of_b_case(grep_options_struct grep_options)
//{
//	if (grep_options.is_b_active==TRUE)
//		return TRUE;
//	return FALSE;
//
//}
//
//int take_care_of_c_case(grep_options_struct grep_options)
//{
//	if (grep_options.is_c_active==TRUE)
//		return TRUE;
//	return FALSE;
//
//}
//
//int take_care_of_i_case(grep_options_struct grep_options)
//{
//	if (grep_options.is_i_active==TRUE)
//		return TRUE;
//	return FALSE;
//
//}
//
//int take_care_of_n_case(grep_options_struct grep_options)
//{
//	if (grep_options.is_n_active==TRUE)
//		return TRUE;
//	return FALSE;
//
//}
//
//char* strcasestr(char* str1, const char* str2)
//{
//	char* p1 = str1;
//    const char* p2 = str2;
//    char* r = *p2 == 0 ? str1 : 0;
//
//    while(*p1 != 0 && *p2 != 0)
//    {
//        if (tolower(*p1)==tolower(*p2)) {
//            if (r==0)
//                r=p1;
//            p2++;
//		}
//		else {
//            p2 = str2;
//            if (tolower(*p1)==tolower(*p2)) {
//                r = p1;
//                p2++;
//            }
//            else
//				r=0;
//		}
//        p1++ ;
//    }
//    return *p2 == 0 ? r : 0;
//}