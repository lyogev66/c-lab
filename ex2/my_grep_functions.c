#include "my_grep.h"
#include "helper.h" //remove in unix
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAD_WITH_END_OF_STRING 1
#define CURRENT_CHAR 0
#define NEXT_CHAR 1
#define SECOND_ARG_SQR_BRACK 3
#define INITIAL_ARRAY_SIZE 1
#define TWICE_BRACKET 2

void init_grep_arguments(grep_options_struct *grep_options)
{
  grep_options->value_for_a = 0;
  grep_options->is_b_active = FALSE;
  grep_options->is_c_active = FALSE;
  grep_options->is_i_active = FALSE;
  grep_options->is_n_active = FALSE;
  grep_options->is_v_active = FALSE;
  grep_options->is_x_active = FALSE;
  grep_options->is_E_active = FALSE;
  grep_options->searched_str = NULL;
  grep_options->file_name = NULL;
  grep_options->regex_array = NULL;
  grep_options->size_of_regex_array = 0;
}

void read_line(FILE *file, char **line, line_descriptor_struct *line_descriptor)
{
  char *lineptr = *line;
  size_t allocated_size_for_line, n_value_get_line;

  allocated_size_for_line = getline(line, &n_value_get_line, file);
  line_descriptor->file_bytes_counter += allocated_size_for_line;
  line_descriptor->line_counter++;
  if (feof(file)) {
    free(lineptr);
  }
}

void create_copy_strings(char *line, grep_options_struct grep_options, char **line_copy, char **search_string_copy)
{
  *line_copy = (char *)malloc(sizeof(char) * strlen(line) + PAD_WITH_END_OF_STRING);
  *search_string_copy = (char *)malloc(sizeof(char) * strlen(grep_options.searched_str) + PAD_WITH_END_OF_STRING);
  if (line_copy == NULL || search_string_copy == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  strcpy(*line_copy, line);
  strcpy(*search_string_copy, grep_options.searched_str);
}

void print_for_n_flag(line_descriptor_struct line_descriptor, grep_options_struct grep_options)
{
  if (grep_options.is_n_active == TRUE) {
    if (line_descriptor.match_found == TRUE) {
      printf("%d:", line_descriptor.line_counter);
    } else {
      printf("%d-", line_descriptor.line_counter);
    }
  }
}

void print_for_b_flag(line_descriptor_struct line_descriptor, grep_options_struct grep_options)
{
  size_t bytes_from_begginging = ((line_descriptor.file_bytes_counter) - strlen(line_descriptor.current_line));

  if (grep_options.is_b_active == TRUE) {
    if (line_descriptor.match_found == TRUE) {
      printf("%ld:", bytes_from_begginging);
    } else {
      printf("%ld-", bytes_from_begginging);
    }
  }
}

void print_match(line_descriptor_struct line_descriptor, grep_options_struct grep_options)
{
  if (grep_options.is_c_active == FALSE) {
    print_for_n_flag(line_descriptor, grep_options);
    print_for_b_flag(line_descriptor, grep_options);
    printf("%s", line_descriptor.current_line);
  }
}

void allocate_cell_for_regex_array(grep_options_struct *grep_options)
{
  int size_of_regex_array = grep_options->size_of_regex_array;
  size_of_regex_array++;
  if (size_of_regex_array == INITIAL_ARRAY_SIZE) {
    grep_options->regex_array = (regex_struct *)malloc(INITIAL_ARRAY_SIZE * sizeof(regex_struct));
  } else {
    grep_options->regex_array =
        (regex_struct *)realloc(grep_options->regex_array, (size_of_regex_array) * sizeof(regex_struct));
  }
  grep_options->size_of_regex_array = size_of_regex_array;
}

void insert_char_to_regex_array(grep_options_struct *grep_options, type_of_character type, char current)
{
  allocate_cell_for_regex_array(grep_options);
  grep_options->regex_array[grep_options->size_of_regex_array - 1].charecter = current;
  grep_options->regex_array[grep_options->size_of_regex_array - 1].type = type;
}

void insert_sqr_to_regex_array(grep_options_struct *grep_options, type_of_character type, char starting_char,
                               char ending_char)
{
  allocate_cell_for_regex_array(grep_options);
  grep_options->regex_array[grep_options->size_of_regex_array - 1].start_value_sqr_brac = starting_char;
  grep_options->regex_array[grep_options->size_of_regex_array - 1].end_value_sqr_brac = ending_char;
  grep_options->regex_array[grep_options->size_of_regex_array - 1].type = type;
}

void insert_round_to_regex_array(grep_options_struct *grep_options, type_of_character type, char *curl_barc_str1,
                                 char *curl_barc_str2)
{
  allocate_cell_for_regex_array(grep_options);
  grep_options->regex_array[grep_options->size_of_regex_array - 1].curl_barc_str1 = curl_barc_str1;
  grep_options->regex_array[grep_options->size_of_regex_array - 1].curl_barc_str2 = curl_barc_str2;
  grep_options->regex_array[grep_options->size_of_regex_array - 1].type = type;
}

void remove_backslash_from_search_string(grep_options_struct *grep_options)
{
  char *current_substring = current_substring = grep_options->searched_str;
  for (; current_substring[CURRENT_CHAR] != '\0'; current_substring++) {
    if (current_substring[CURRENT_CHAR] == '\\') {
      strcpy(current_substring, current_substring + 1);
      current_substring++;
    }
  }
}

char *find_or_condition_location(char *temp)
{
  while (temp[CURRENT_CHAR] != '|') {
    temp++;
  }
  return temp;
}

int count_number_of_char_to_jump(char *start_char_brack)
{
  int counter = 0;
  char *temp_location = start_char_brack;
  while (temp_location[NEXT_CHAR] != ')') {
    counter++;
    temp_location++;
  }
  return counter;
}

void convert_substring_to_two_string(grep_options_struct *grep_options)
{
  char *temp1_str_ptr = NULL;
  int current_index = 0;

  while (grep_options->regex_array[current_index].type != ROUND_BRACK) {
    current_index++;
  }
  temp1_str_ptr = grep_options->regex_array[current_index].curl_barc_str1;
  while (temp1_str_ptr[CURRENT_CHAR] != '|') {
    temp1_str_ptr++;
  }
  temp1_str_ptr[CURRENT_CHAR] = '\0';
  temp1_str_ptr = grep_options->regex_array[current_index].curl_barc_str2;
  while (temp1_str_ptr[CURRENT_CHAR] != ')') {
    temp1_str_ptr++;
  }
  temp1_str_ptr[CURRENT_CHAR] = '\0';
}

void parse_expression(grep_options_struct *grep_options)
{
  char *current_substring = grep_options->searched_str, *temp = NULL;
  int flag_run_convert_substring_to_two_string = 0;
  if (grep_options->is_E_active == TRUE) {
    for (; current_substring[CURRENT_CHAR] != '\0'; current_substring++) {
      if (current_substring[CURRENT_CHAR] == '\\') {
        insert_char_to_regex_array(grep_options, NORMAL_CHAR, current_substring[NEXT_CHAR]);
        current_substring++;
      } else if (current_substring[CURRENT_CHAR] == '.') {
        insert_char_to_regex_array(grep_options, DOT_CHAR, '.');
      } else if (current_substring[CURRENT_CHAR] == '[') {
        insert_sqr_to_regex_array(grep_options, SQR_BRACK, current_substring[NEXT_CHAR],
                                  current_substring[SECOND_ARG_SQR_BRACK]);
        current_substring += SECOND_ARG_SQR_BRACK + 1;
      } else if (current_substring[CURRENT_CHAR] == '(') {
        flag_run_convert_substring_to_two_string++;
        temp = find_or_condition_location(current_substring);
        insert_round_to_regex_array(grep_options, ROUND_BRACK, (current_substring + NEXT_CHAR), (temp + NEXT_CHAR));
        current_substring += count_number_of_char_to_jump(current_substring + NEXT_CHAR) + TWICE_BRACKET;
      } else {
        insert_char_to_regex_array(grep_options, NORMAL_CHAR, current_substring[CURRENT_CHAR]);
      }
    }
    if (flag_run_convert_substring_to_two_string != 0) {
      convert_substring_to_two_string(grep_options);
    }
  } else {
    remove_backslash_from_search_string(grep_options);
  }
}

void get_grep_arguments(grep_options_struct *grep_options, char *argv[], int argc)
{
  int argc_index, search_term_found = FALSE, target_file_found = FALSE;
  init_grep_arguments(grep_options);

  for (argc_index = 1; argc_index < argc; argc_index++) {
    if (!strcmp(argv[argc_index], "-A")) {
      argc_index++;
      grep_options->value_for_a = atoi(argv[argc_index]);
    } else if (!strcmp(argv[argc_index], "-b")) {
      grep_options->is_b_active = TRUE;
    } else if (!strcmp(argv[argc_index], "-c")) {
      grep_options->is_c_active = TRUE;
    } else if (!strcmp(argv[argc_index], "-i")) {
      grep_options->is_i_active = TRUE;
    } else if (!strcmp(argv[argc_index], "-n")) {
      grep_options->is_n_active = TRUE;
    } else if (!strcmp(argv[argc_index], "-v")) {
      grep_options->is_v_active = TRUE;
    } else if (!strcmp(argv[argc_index], "-x")) {
      grep_options->is_x_active = TRUE;
    } else if (!strcmp(argv[argc_index], "-E")) {
      grep_options->is_E_active = TRUE;
    } else if (search_term_found == FALSE) {
      grep_options->searched_str = argv[argc_index];
      search_term_found = TRUE;
    } else if (target_file_found == FALSE) {
      grep_options->file_name = argv[argc_index];
      target_file_found = TRUE;
    }
  }
  parse_expression(grep_options);
}

FILE *open_file_or_stdin(grep_options_struct grep_options)
{
  FILE *file = NULL;
  if (grep_options.file_name != NULL) {
    file = fopen(grep_options.file_name, "r");
    if (file == NULL) {
      exit(1);
    }
  } else {
    file = stdin;
  }
  return file;
}

void init_line_descriptor_struct(line_descriptor_struct *line_descriptor)
{
  line_descriptor->line_counter = 0;
  line_descriptor->number_of_matches = 0;
  line_descriptor->match_found = FALSE;
  line_descriptor->current_line = NULL;
  line_descriptor->file_bytes_counter = 0;
  line_descriptor->number_of_lines_remained_to_print = 0;
}

void tolower_str(char *string)
{
  unsigned int char_index = 0;

  for (char_index = 0; char_index <= strlen(string); char_index++) {
    string[char_index] = tolower(string[char_index]);
  }
}

int is_match_recursive(char *line_copy, grep_options_struct grep_options, int regex_index)
{
  char current_char_line = line_copy[CURRENT_CHAR];
  regex_struct current_char_cell = grep_options.regex_array[regex_index];
  int end_of_line = (current_char_line == '\0');
  char optional_match;

  if (grep_options.size_of_regex_array == regex_index) {
    return TRUE;
  } else if (end_of_line) {
    return FALSE;
  }
  regex_index++;
  switch (current_char_cell.type) {
    case NORMAL_CHAR: {
      if (current_char_line == current_char_cell.charecter) {
        return is_match_recursive(line_copy + NEXT_CHAR, grep_options, regex_index);
      }
      return FALSE;
    }
    case DOT_CHAR: {
      for (optional_match = 33; optional_match <= 126; optional_match++) {
        if (current_char_line == optional_match) {
          return is_match_recursive(line_copy + NEXT_CHAR, grep_options, regex_index);
        }
      }
      return FALSE;
    }
    case SQR_BRACK: {
      for (optional_match = current_char_cell.start_value_sqr_brac;
           optional_match <= current_char_cell.end_value_sqr_brac; optional_match++) {
        if (current_char_line == optional_match) {
          return is_match_recursive(line_copy + NEXT_CHAR, grep_options, regex_index);
        }
      }
      return FALSE;
    }
    case ROUND_BRACK: {
      if (!strncmp(line_copy, current_char_cell.curl_barc_str1, strlen(current_char_cell.curl_barc_str1))) {
        return is_match_recursive(line_copy + strlen(current_char_cell.curl_barc_str1), grep_options, regex_index);
      } else if (!strncmp(line_copy, current_char_cell.curl_barc_str2, strlen(current_char_cell.curl_barc_str2))) {
        return is_match_recursive(line_copy + strlen(current_char_cell.curl_barc_str2), grep_options, regex_index);
      }
      return FALSE;
    }
  }
  return FALSE;
}

int is_match_recursive_case(char *line_copy, grep_options_struct grep_options)
{
  int starting_index = 0;
  return is_match_recursive(line_copy, grep_options, starting_index);
}

int is_match_in_line(char *line, grep_options_struct grep_options)
{
  int found_match = FALSE;
  unsigned int current_index;
  char *line_copy = NULL, *search_string_copy = NULL;
  create_copy_strings(line, grep_options, &line_copy, &search_string_copy);
  if (grep_options.is_i_active == TRUE) {
    tolower_str(line_copy);
    tolower_str(search_string_copy);
  }
  if (grep_options.is_E_active == TRUE) {
    for (current_index = 0; current_index <= strlen(line_copy); current_index++) {
      found_match = is_match_recursive_case(line_copy + current_index, grep_options);
      if (found_match == TRUE) {
        goto FREE_AND_RETURN;
      }
    }
  }
  if (grep_options.is_x_active == FALSE) {
    found_match = (strstr(line_copy, search_string_copy) != NULL);
  } else {
    found_match = (strncmp(line_copy, search_string_copy, strlen(search_string_copy)) == 0);
  }
FREE_AND_RETURN:
  free(line_copy);
  free(search_string_copy);
  return found_match;
}

void fill_line_descriptor(line_descriptor_struct *line_descriptor, char *line, grep_options_struct grep_options)
{
  int print_revers_lines = (grep_options.is_v_active == TRUE);
  line_descriptor->match_found = is_match_in_line(line, grep_options) ? (!print_revers_lines) : print_revers_lines;
  line_descriptor->current_line = line;
  if (line_descriptor->match_found == TRUE) {
    line_descriptor->number_of_lines_remained_to_print = 1 + grep_options.value_for_a;
    line_descriptor->number_of_matches += line_descriptor->match_found;
  }
}