#ifndef LIST
#define LIST

typedef struct ClientNode {
    int data;
    char user_code[LENGTH_CODE];
    char public_key[LENGTH_CODE];
    struct ClientNode* prev;
    struct ClientNode* link;
    char ip[16];
    char name[31];
} ClientList;

ClientList *newNode(int sockfd, char* ip,char*user_code) {
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) +1);
    np->data = sockfd;
    np->prev = NULL;
    np->link = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->name, "NULL", 5);
    strncpy(np->user_code,user_code,LENGTH_CODE);
    return np;
}

char* inc(char *num, char* p)//https://stackoverflow.com/questions/891715/recursive-incrementer
{   
    if( *p>='0' && *p<='8' )
    {
        (*p)++;       //==> change
    }
    else if ( *p=='9' )
    {
        *p = '0';
        inc(num, --p);
    }

    return num;
}
#endif // LIST