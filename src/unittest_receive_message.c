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
	/*int	i;

	char	**commands = (char **)malloc(sizeof(char *) * return_strcount(argv + 2));
	for (i = 2; i < return_strcount(argv); i++)
	{
		*(commands + i - 2) = strdup(argv[i]);
	}
	commands[i - 2] = 0;
	
	int	idx = 0;
	char	**splitted = return_splitted_message(argv[1], commands);
	
	while (splitted[idx] != 0)
		printf("[%d] %s\n", idx, splitted[idx++]);
	*/

	int	idx = 0;
	char	**splitted = create_command(argv[1], ' '); 
	
	while (splitted[idx] != 0)
		printf("[%d] %s\n", idx, splitted[idx++]);
	return 0;
}
