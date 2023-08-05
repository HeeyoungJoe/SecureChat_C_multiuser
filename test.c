#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include "src/client.h"
#include "src/proto.h"


UserList *root, *now;

int main(){

    int sockfd=38293;
    char nickname[LENGTH_NAME];
    strncpy(nickname,"Jackie",LENGTH_NAME);
    char user_code[LENGTH_CODE];
    strncpy(user_code,"000111",LENGTH_CODE);
    char public_key[247]="takarazukahanagumi0601";//길이는 1임

    // Test node creation
    // Root user node 
    root = newNode(sockfd, nickname,user_code,public_key);
    now = root;

    printNode(root, "[TEST.C/NEW NODE]");

    // Test node public key update 
    updatePublicKey(root,user_code,"asdk;ljf;394091");
    printNode(root, "[TEST.C/UPDATE Public key] ");
    
    return 0;
}