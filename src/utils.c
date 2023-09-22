
int	natoi(char *str, int n)
{
	//starting from position 0 to null, 
	//check n characters and turn it into decimal int
	
	int	idx = 0;
	int	result = 0;
	if (str == NULL || n <= 0)
		return -1;
	while (str[idx] != '\0' && idx < n)
	{
		result *= 10;
		result += str[idx] - '0';
		idx++;
	}
	return result;
}
