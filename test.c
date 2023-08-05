#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include "src/client.h"


UserList *root, *now;

int main(){

    int sockfd=38293;
    char nickname[100];
    strncpy(nickname,"Jackie",6);
    char user_code[6];
    strncpy(user_code,"000111",6);
    char public_key[247]={"0"};
    // Root user node 
    root = newNode(sockfd, nickname,user_code,public_key);
    now = root;
    return 0;
}