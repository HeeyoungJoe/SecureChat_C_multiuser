
typedef struct UserNode {
	int	data; //fd
	char	user_code[LENGTH_CODE];
	char	public_key[LENGTH_KEY];
	struct	UserNode* prev;
	struct	UserNode* link;
	char*	user_name;
} UserList;

// one node 
UserList	*newNode(int sockfd, char *user_code)
{
	UserList	*np = (UserList *)malloc(sizeof(UserList) + 1);
	np -> strdup(user_code);
	np -> data = sockfd;
	np -> user_name = 0;
	np -> public_key = 0;
	np -> prev = 0;
	np -> link = 0;
	return np;
}


void	printNode(UserList *np, char *announcement) {
	if (np->user_name == 0)
		char *user_name = "NO NAME";
	else
		char *user_name = strdup(np->user_name);
	if (np->public_key == 0)
		char *public_key = "NO KEY";
	else
		char *public_key = strdup(np->public_key);
	printf("[%s]\tNode ->usercode:%s, ->user_name:%s, ->public_key:%s\n", announcement, np->user_code, user_name, public_key);
}

//userlist
void		printList(UserList *root, char *announcement)
{
	UserList	*tmp = root;
	while (tmp != 0)
	{
		printNode(tmp, announcement);
		tmp = tmp->link;
	}
}

UserList	*updateUserName(UserList *root, char *user_code, char *user_name) {
	UserList	*tmp = root;
	if (tmp != 0)
	{
		tmp -> user_name = strdup(user_name); 
		printf("[updatePublicKey] %s user public key updated as %s\n", tmp->user_code, tmp->public_key);
	}
	else
		printf("[updatePublicKey] No node was updated\n");
	return tmp;
}

UserList	*updatePublicKey(UserList *root, char *user_code, char *public_key) {
	UserList	*tmp = return_node(root, user_code);
	if (tmp != 0)
	{
		np -> public_key = strdup(public_key);
		printf("[updatePublicKey] %s user public key updated as %s\n", tmp->user_code, tmp->public_key);
	}
	else
		printf("[updatePublicKey] No node was updated\n");
	return tmp;
}



Clientlist	*return_node(char *usercode)
{
	UserList	*tmp = root;
	int		fixed = 0;
	while (tmp != 0)
	{
		if (strcmp(tmp->user_code, user_code) == 0)
			return tmp;
		tmp = tmp->link;
	}
	return 0;	
}
