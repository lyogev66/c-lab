#include "Text_Input.h"
#include "Get_Arguments.h"
void lower_string(char *str) 
{
	int i;
	for(i = 0; str[i]; i++){
	  str[i] = tolower(str[i]);
	}
}

int find_file_sentences_number(FILE *file)
{
	char *line = NULL ;
	size_t len = 0;
	ssize_t read;
	rewind(file);
	int counter =0;
	while (read= getline(&line,&len,file)!=-1)
	{
		counter ++;
	}
	return counter;
}

char IsMatchRegularExpression(arguments *arg, int expressionIndex, 
	char *resolvedExpression, int resolvedIndex, char *line)
{
	char ret;
	int expressionIterator;
	int tempResolvedIndex;
	int dotIterator;
	int groupEndIndex;
	char fromSquareBrackets;
	char toSquareBrackets;
	char squareBracketsIterator;
	for (expressionIterator = expressionIndex; expressionIterator < strlen(arg->Expression); expressionIterator++)
	{
		switch (arg->Expression[expressionIterator])
		{
		case '.':
			for (dotIterator = 33; dotIterator < 126; dotIterator++)
			{
				resolvedExpression[resolvedIndex] = dotIterator;
				if(IsMatchRegularExpression(arg, expressionIterator + 1, resolvedExpression, resolvedIndex + 1, line))
					return 1;
			}
			return 0;
		case '(':
			expressionIterator++;
			for (groupEndIndex = expressionIterator; arg->Expression[groupEndIndex] != ')'; groupEndIndex++);
			while (arg->Expression[expressionIterator] != ')')
			{
				tempResolvedIndex = resolvedIndex;
				while (arg->Expression[expressionIterator] != '|' && arg->Expression[expressionIterator] != ')')
				{
					resolvedExpression[tempResolvedIndex] = arg->Expression[expressionIterator];
					tempResolvedIndex++;
					expressionIterator++;
				}
				if(IsMatchRegularExpression(arg, groupEndIndex + 1, resolvedExpression, tempResolvedIndex, line))
					return 1;
				if(arg->Expression[expressionIterator] != ')')
					expressionIterator++;
			}
			return 0;
		case '[':
			fromSquareBrackets = arg->Expression[expressionIterator + 1];
			toSquareBrackets = arg->Expression[expressionIterator + 3];
			expressionIterator += 5;
			for (squareBracketsIterator = fromSquareBrackets; squareBracketsIterator <= toSquareBrackets;
				squareBracketsIterator++)
			{
				resolvedExpression[resolvedIndex] = squareBracketsIterator;
				if(IsMatchRegularExpression(arg, expressionIterator, resolvedExpression, resolvedIndex + 1, line))
					return 1;
			}
			return 0;
		case '\\':
			expressionIterator++;
			resolvedExpression[resolvedIndex] = arg->Expression[expressionIterator];
			resolvedIndex++;
			break;
		default:
			resolvedExpression[resolvedIndex] = arg->Expression[expressionIterator];
			resolvedIndex++;
			break;
		}
	}
	resolvedExpression[resolvedIndex] = '\0';
	//Handle exact match
	ret = arg->ExactMatch ? strcmp(line, resolvedExpression) == 0 : strstr(line, resolvedExpression) != NULL;
	//handle V case
	return arg->NegativeExpression ? !ret : ret;
}

char find_a_word_in_sentence(char *line,arguments *arg)
{	 
   char ret;
   char resolvedExpression[1024];
   char lowerLine[1024];

   strcpy(lowerLine, line);
   lowerLine[strlen(lowerLine) - 1] = '\0';
// this fix -i case
   if(arg->IgnoreUpperCase)
   {
	   lower_string(lowerLine);
	   lower_string(arg->Expression);
   }

   //handle regualr expressions
   if (arg->IsRegularExpression) 
   {
	   return IsMatchRegularExpression(arg, 0, resolvedExpression, 0, lowerLine);
   }
   ret = arg->ExactMatch ? strcmp(lowerLine,arg->Expression)==0 : strstr(lowerLine, arg->Expression)!=NULL;
   return arg->NegativeExpression ? !ret : ret;
}

void print_manager(char* line, arguments *arg,int line_number,int sum_bytes, char isAppend)
{
	//// this fix -n case
	if(arg->PrintLineNumbers)
	{
		printf("%d",line_number);
		printf("%c", isAppend ? '-' : ':');
	}
	if(arg->LineOffset)
	{
		printf("%d",sum_bytes);	
		printf("%c", isAppend ? '-' : ':');
	}
	
	printf("%s",line);

}
void PrintLineNumbersOnly(FILE *file,arguments *arg)
{
	int count_sentences=0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	while (read= getline(&line,&len,file)!=-1)
	{
		if(find_a_word_in_sentence(line,arg))
			count_sentences++;
	}

	printf("%d",count_sentences);
	printf("\n");
	rewind(file);
	return;
}

void AppendLinesAndPrint(FILE *file,char *line, arguments *arg,int *line_number)
{
	int i;
	int offset;
	char *line2 = NULL;
	size_t len = 0;
	ssize_t read;
	for(i=0;i<=arg->NumOfLinesToAppend;i++)
	{
		offset = ftell(file);
		read=getline(&line2,&len,file);
		(*line_number)++;
		if(read==-1)
			break;
		else
		{
			if (arg->NumOfLinesToAppend == i) {
				if (!find_a_word_in_sentence(line2, arg))
					break;
			}			
			if(find_a_word_in_sentence(line2,arg))
				i=-1;
			print_manager(line2,arg,*line_number, offset,i!=-1);
		}
	}
}

void print_lines(FILE *file,arguments *arg)
{
	int i=0,line_number=0,lines=find_file_sentences_number(file);
	int temp_sum_bytes,temp_line_number;
	int offset;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	rewind(file);
	if(lines==0)
		return;
// this fix -c case
	if (arg->PrintLineNumbersOnly)
	{
		PrintLineNumbersOnly(file,arg);
		return;
	} 
	offset = ftell(file);
	while (read= getline(&line,&len,file)!=-1)
	{
		line_number++;

		if(find_a_word_in_sentence(line,arg))
		{
			print_manager(line,arg,line_number, offset,0);
			AppendLinesAndPrint(file,line,arg,&line_number);
		}	
		offset = ftell(file);
	}
}