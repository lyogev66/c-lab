#include "common.h"
#include "input.h"


void turn_mixedcase_type_to_lowercase(char * string)
{
	int string_index=0;

	while((isalpha(string[string_index])||(string[string_index] == '_')) && (string[string_index] != '\0'))
	{
		if (!strcmp(string,"_"))
		{
			string_index++;
			continue;
		}
		string[string_index]=tolower(string[string_index]);
		string_index++;
	}
}

int get_action_type(char * str)
{
	if (!strcmp(str,"add_start"))
		return ADD_START;
	if(!strcmp(str,"add_end"))
		return ADD_END;
	if(!strcmp(str,"add_after"))
		return ADD_AFTER;
	if(!strcmp(str,"index"))
		return INDEX;
	if(!strcmp(str,"del"))
		return DEL;
	if(!strcmp(str,"print"))
		return PRINT_LIST;
	if(!strcmp(str,"exit"))
		return EXIT_LIST;
	else
		return NO_INDEX;
}

int get_first_index(char * str)
{
	return atoi(str);
}

int get_second_index(char * str)
{
	return atoi(str);
}

input_struct parse_data_to_input_struct(char *str)
{
	input_struct parsed_struct;
	char * semi_string=NULL;

	parsed_struct.action=NO_INDEX;
	parsed_struct.index1=NO_INDEX;
	parsed_struct.index2=NO_INDEX;

	semi_string=strtok(str," ");
	parsed_struct.action=get_action_type(semi_string);
	semi_string=strtok(NULL," ");
	if	(semi_string!=NULL)
	{
		parsed_struct.index1=get_first_index(semi_string);
		semi_string=strtok(NULL," ");
		if	(semi_string!=NULL)
			parsed_struct.index2=get_second_index(semi_string);
	}
	return parsed_struct;
}

input_struct input_main()
{
	char input_data_string[100];
	input_struct parsed_struct;

	gets(input_data_string);
	turn_mixedcase_type_to_lowercase(input_data_string);
	parsed_struct=parse_data_to_input_struct(input_data_string);
	return parsed_struct;
}