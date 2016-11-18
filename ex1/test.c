#include "input.h"
#include "list.h"


void main()
{
	int end_program=10;
	a_list *list=NULL;
	//gets(input_string);
	input_struct parsed_input;
	char input_string[]="exiT 1";
	//strcpy(input_string,"ADD_end 1");
	
	//list *list=NULL;
	//create_list(list);
	//release_list

	while(TRUE)
	{
	parsed_input=input_main(input_string);
	end_program=list_main(parsed_input,&list);
	}
}


