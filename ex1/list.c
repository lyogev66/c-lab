#include "common.h"
#include "input.h"
#include "list.h"


a_list* create_node(int val)
{
	a_list *new_node = (a_list *)malloc(sizeof(a_list));
	new_node->num = val;
	new_node->next = NULL;
	return(new_node);
}

int add_start(a_list **list, int num_to_add)
{
	a_list *new_head = create_node(num_to_add);
	new_head->next=*list;
	*list=new_head;
	return(OK);
}

int add_end(a_list **list, int num_to_add)
{
	a_list *current=*list;
	if (current==NULL)
		add_start(list, num_to_add);
	else
	{
		while (current->next != NULL)
			current = current->next;
		current->next = create_node(num_to_add);
	}
	return(OK);
}

int add_after(a_list **list, int num_to_add, int num_to_add_after)
{
	a_list *current=*list;
	while(current!=NULL)
	{
		if (current->num == num_to_add_after)
		{
			a_list *new_item = create_node(num_to_add);
			new_item->next=current->next;
			current->next=new_item;
			return(OK);;
		}
		current = current->next;
	}
	printf("the item was not found\n");
	return(QUIT);
}

int get_index(a_list **list, int num_to_find)
{
	int index=0;
	a_list *current=*list;
	while(current!=NULL)
	{
		if (current->num == num_to_find)
		{
			printf("%d\n", index);
			return(OK);
		}
		else
		{
			index++;
			current=current->next;
		}
	}
	printf("-1\n");
	return(OK);
}

int del_first(a_list **list)
{
	a_list *temp = NULL;
	if (*list == NULL)
	{
		printf("the item was not found\n");
		return(QUIT);
	}
	temp = (*list)->next;
	free(*list);
	*list = temp;
	return(OK);
}

int del(a_list **list, int index_to_del)
{
	int index=0;
	a_list *current=*list;
	while(current->next!=NULL)
	{
		if (index == index_to_del - 1)
		{
			a_list *temp = NULL;
			temp = current->next;
			current->next = temp->next;
			free(temp);
			return (OK);
		}
		index++;
		current = current->next;
	}
	printf("the item was not found\n");
	return(QUIT);
}

int print(a_list **list)
{
	a_list *current=*list;
	printf("[");
	if (current == NULL)
	{
		printf("]\n");
	}
	else
	{
		while (current->next != NULL)
		{
			printf("%d, ", current->num);
			current = current->next;
		}
		printf("%d]\n", current->num);
	}
	return(OK);
}

int terminate(a_list **list)
{
	if (*list == NULL)
		return(QUIT);
	a_list *current=*list;
	while(current->next!=NULL)
	{
		a_list *temp=current;
		current=current->next;
		free(temp);
	}
	free(current);
	return(QUIT);
}

int list_main(input_struct parsed_input, a_list **list)
{
	switch(parsed_input.action) {
	case ADD_START:
		return(add_start(list, parsed_input.index1));
	case ADD_END:
		return(add_end(list, parsed_input.index1));
	case ADD_AFTER:
		return(add_after(list, parsed_input.index1, parsed_input.index2));
	case INDEX:
		return(get_index(list, parsed_input.index1));
	case DEL:
		if (parsed_input.index1 == 0)
			return(del_first(list));
		else
			return(del(list, parsed_input.index1));
	case PRINT_LIST:
		return(print(list));
	case EXIT_LIST:
		return(terminate(list));
	default :
		return(QUIT);
	}
}
