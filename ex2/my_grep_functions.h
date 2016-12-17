#ifndef MY_GREP_FUNCTIONS_H
#define MY_GREP_FUNCTIONS_H

void get_grep_arguments(grep_options_struct *grep_options,char *argv[],int argc);
FILE * open_file_or_stdin(grep_options_struct grep_options);	
void init_line_descriptor_struct(line_descriptor_struct *line_descriptor);
void read_line(FILE *file,char **line,line_descriptor_struct *line_descriptor);
void fill_line_descriptor(line_descriptor_struct *line_descriptor,char *line,grep_options_struct grep_options);
void print_match(line_descriptor_struct line_descriptor,grep_options_struct grep_options);

#endif