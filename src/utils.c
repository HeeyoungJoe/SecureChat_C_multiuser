#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int	strendswith(char *end, char *str)
{
	int	end_len = strlen(end);
	int	idx = 0;
	int	str_len = strlen(str);
	int	str_idx = str_len - end_len;
	if (str_idx < 0)
		return -1;
	while (str_idx < str_len && idx < end_len - 1)
	{
		if (end[idx] != str[str_idx])
			return -1;
		str_idx++;
		idx++;
	}
	return 1;
}

int	strstartswith(char *s1, char *s2)
{
	int	idx = 0;
	int	n = strlen(s1);
	while (idx < n && s1[idx] != '\0' && s2[idx] != '\0')
	{
		if (s1[idx] != s2[idx])
			return -1;
		idx++;
	}
	if (s1[idx] != '\0' && s2[idx] == '\0')
		return -1;
	return 1;
}




int	natoi(char *str, int n)
{
	//starting from position 0 to null, 
	//check n characters and turn it into decimal int
	
	int	idx = 0;
	int	result = 0;
	if (str == (char *)0 || n <= 0)
		return -1;
	while (str[idx] != '\0' && idx < n)
	{
		result *= 10;
		result += str[idx] - '0';
		idx++;
	}
	return result;
}
