#ifndef MY_GREP_H
#define MY_GREP_H

#define MIN_ARGUMENT_NUMBER		2
#define TRUE					1
#define FALSE					0


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
void read_line(FILE *file,char **line,matched_struct *match_line);
int is_match_in_line(char *line,grep_options_struct grep_options);
void print_match(matched_struct matched,grep_options_struct grep_options);
void init_grep_arguments(grep_options_struct *grep_options);
void get_grep_arguments(grep_options_struct *grep_options,char *argv[],int argc);
FILE * open_file_or_stdin(grep_options_struct grep_options);
void init_match_line(matched_struct *matched_line);
char* strcasestr(char* str1, char* str2);

#endif
