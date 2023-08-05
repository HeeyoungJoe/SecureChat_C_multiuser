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
    char public_key[247]={"0"};//길이는 1임

    // Test node creation
    // Root user node 
    root = newNode(sockfd, nickname,user_code,public_key);
    now = root;

    printf("\n[TEST.C/NEW NODE] Root: %s has %s as code and %s as public_key\n",root->user_name,root->user_code,root->public_key);

    // Test node update 
    updateUserName(root,user_code,"Keith");
    printf("\n[TEST.C/UPDATE USERNAME] Root: %s has %s as code and %s as public_key\n",root->user_name,root->user_code,root->public_key);

    return 0;
}