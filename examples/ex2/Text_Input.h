#ifndef INPUT
#define INPUT
#define BUF_SIZE 1024
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "Get_Arguments.h"


char find_a_word_in_sentence(char *line, arguments *arg);
int find_file_sentences_number(FILE *file);
void print_manager(char* line, arguments *arg, int line_number, int sum_bytes, char isAppend);
void print_lines(FILE *file,arguments *arg);



#endif
