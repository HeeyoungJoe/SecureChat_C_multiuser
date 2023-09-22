#include <string.h>
#include <stdlib.h>
#include <stdio.h>

# define KEY_SIZE 287
# define USER_CODE_SIZE 6

void	puterror(char *str);
int	nposatoi(char *str, unsigned int n);
int	strstartswith(char *s1, char *s2);
int	strendswith(char *s1, char *s2);

// 1. Receiving
// finding the type of message
// cutting down string into several string of information 
// per message type, doint what it needs to do with the cut down string



int	return_message_type(char *str)
{
	if (str == NULL)
		return -1;
	int message_type = nposatoi(str, 2);
	return message_type;
}

char	**return_splitted_message(char *str, char **commands)
{
	int	idx = 0;
	char	**splitted = (char **)malloc(sizeof(char *) * 10);
	int	splitted_idx = 0;
	while (str != (char *)0 && *(commands[idx]) != '\0')
	{
		if (strstartswith("usercode", commands[idx]) == 1)
		{
			splitted[splitted_idx++] = strndup(str, USER_CODE_SIZE);
			str += USER_CODE_SIZE;
		}
		else if (strstartswith("key", commands[idx]) == 1)
		{
			splitted[splitted_idx++] = strndup(str, KEY_SIZE);
			str += KEY_SIZE;
		}
		else if (strendswith("message", commands[idx]) == 1)
		{
			splitted[splitted_idx++] = strndup(str, strlen(str));
			str = 0;
		}
		else
		{
			printf("RECEIVE MSG ERROR : undefined command caught - %s\n", commands[idx]);
			return (char **)0;
		}

		idx++;
	}
	splitted[idx] = 0;
	return splitted;
}
			

