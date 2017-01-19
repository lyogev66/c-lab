#ifndef MY_GREP_H
#define MY_GREP_H

#include <stdio.h>

#define MIN_ARGUMENT_NUMBER 2
#define TRUE 1
#define FALSE 0

typedef enum { NORMAL_CHAR, DOT_CHAR, SQR_BRACK, ROUND_BRACK } type_of_character;

typedef struct regex_struct {
  type_of_character type;
  char charecter, start_value_sqr_brac, end_value_sqr_brac;
  char *curl_barc_str1, *curl_barc_str2;
} regex_struct;

typedef struct grep_options_struct {
  int value_for_a;
  int is_b_active;
  int is_c_active;
  int is_i_active;
  int is_n_active;
  int is_v_active;
  int is_x_active;
  int is_E_active;
  char *searched_str;
  char *file_name;
  regex_struct *regex_array;
  int size_of_regex_array;

} grep_options_struct;

typedef struct line_descriptor_struct {
  char *current_line;
  size_t file_bytes_counter;
  int match_found, line_counter, number_of_matches, number_of_lines_remained_to_print;
} line_descriptor_struct;

#endif