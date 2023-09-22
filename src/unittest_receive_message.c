#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int	return_message_type(char *str);
char	**return_splitted_message(char *str, char **commands);
int	return_strcount(char **strs);

int	main(int argc, char **argv)
{
	//test return_message_type
	//printf("message type is %d\n", return_message_type(argv[1]));

	//test return_splitted_message
	int	idx = 0;
	printf("return_strcount: %d\n", return_strcount(argv + 2));
	/*
	char	**commands = (char **)malloc(sizeof(char *) * return_strcount(argv + 2));
	for (int i = 2; i < return_strcount(argv + 2); i++)
	{
		*(commands + i) = strdup(argv[i]);
	}
	commands[return_strcount(argv + 2)] = 0;

	char	**splitted = return_splitted_message(argv[1], commands);

	while (splitted[idx] != 0)
		printf("[%d] %s\n", idx, splitted[idx++]);
	*/
	return 0;
}
