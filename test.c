#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include "src/client.h"
#include "src/proto.h"


UserList *root, *now;

int main(){

    int sockfd=38293;
    char nickname[LENGTH_NAME];
    strncpy(nickname,"Jackie",LENGTH_CODE);
    char user_code[LENGTH_CODE];
    strncpy(user_code,"000111",LENGTH_CODE);
    char public_key[247]={"0"};//길이는 1임

    // Test node creation
    // Root user node 
    root = newNode(sockfd, nickname,user_code,public_key);
    now = root;

    printf("\n[TEST.C/NEW NODE] Root: %s has %s as code and %s as public_key\n",root->user_name,root->user_code,root->public_key);

    // Test node username update 
    updateUserName(root,user_code,"Keith");
    printf("\n[TEST.C/UPDATE USERNAME] Root: %s has %s as code and %s as public_key\n",root->user_name,root->user_code,root->public_key);


    // Test node public key update 
    updatePublicKey(root,user_code,"asdk;ljf;aldjfoiewr10394091");
    printf("\n[TEST.C/UPDATE PUBLIC KEY] Root: %s has %s as code and %s as public_key\n",root->user_name,root->user_code,root->public_key);

    // Test get public key
    printf("\n[TEST.C/GET PUBLIC KEY] Root: %s has  %s as public_key\n",root->user_code,getPublicKey(root,root->user_code));

    return 0;
}