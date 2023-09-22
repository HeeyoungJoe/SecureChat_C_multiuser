#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int	natoi(char *str, int n);
int	strendswith(char *end, char *str);
int	strstartswith(char *s1, char *s2);

int	main(int argc, char **argv)
{
	//test natoi
	//printf("natoi for %s :  %d\n", argv[1], natoi(argv[1], strlen(argv[1])));
	
	//test strendswith
	//printf("Does %s endswith cute?: %d\n", argv[1], strendswith("cute", argv[1]));
	
	//test strncompare
	printf("check if s2 starts with s1 /  %s and %s : %d\n", argv[1], argv[2], strstartswith(argv[1], argv[2]));
	return 0;
}
