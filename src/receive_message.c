
# define KEY_SIZE 256

int	natoi(char *str, int n);
int	strs1equal(char *s1, char *s2);

// 1. Receiving
// finding the type of message
// cutting down string into several string of information 
// per message type, doint what it needs to do with the cut down string



int	return_message_type(char *str)
{
	if (str == NULL)
		return -1;
	int message_type = natoi(str, 2);
	return message_type;
}

char	**return_splitted_message(char *str, char **commands)
{
	int	idx = 0;
	char	**splitted = (char *)malloc(sizeof(char *) * 10);
	int	splitted_idx = 0;
	while (commands[idx] != '\0')
	{
		if (strnequal("usercode", commands[idx]) == 1)
		{
			splitted[splitted_idx++] = strndup(str, 6);
			str += 6;
		}
		else if (strnequal("key", commands[idx]) == 1)
		{
			splitted[splitted_idx++] = strndup(str, KEY_SIZE);
			str += KEY_SIZE;
		}
		else if (strendswith("message", commands[idx]) == 1)
		{
			splitted[splitted_idx++] = strndup(str
		idx++;

			

