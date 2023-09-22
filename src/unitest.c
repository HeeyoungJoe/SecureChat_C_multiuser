#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int	natoi(char *str, int n);

int	main(int argc, char **argv)
{
	//test natoi
	printf("natoi for %s :  %d\n", argv[1], natoi(argv[1], strlen(argv[1])));
	return 0;
}
