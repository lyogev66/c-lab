#include "common.h"
#include "input.h"
#include "list.h"

//do you see this?

void main()
{
	a_list *list=NULL;
	input_struct parsed_input;
	int programs_status=OK;
	

	while(programs_status)
	{
	parsed_input=input_main();
	programs_status=list_main(parsed_input,&list);
	}
}


