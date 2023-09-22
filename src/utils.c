int	strnequal(char *s1, char *s2, int n)
{
	int	idx = 0;
	while (idx < n && s1[idx] != '\0' && s2[idx] != '\0')
	{
		if (s1[idx] != s2[idx])
			return -1;
		idx++;
	}
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
