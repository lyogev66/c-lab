#ifndef MY_GREP_H
#define MY_GREP_H

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
	int argv_index_for_target_str;
	int argv_index_for_file_name;
} grep_options_struct;



void init_grep_options(grep_options_struct *grep_options);
void get_grep_options(grep_options_struct *grep_options,char *argv[],int argc);
FILE* open_file_or_open_stdin(char *argv[], grep_options_struct grep_options);
int take_care_of_v_case(char *pointer_to_searched_substring,grep_options_struct grep_options);
int take_care_of_b_case(grep_options_struct grep_options);

#endif