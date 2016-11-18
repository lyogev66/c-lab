#ifndef LIST_H
#define LIST_H


typedef struct a_list {
	int num;
	struct a_list *next;
} a_list;

int list_main(input_struct parsed_input, a_list **list);

#endif
