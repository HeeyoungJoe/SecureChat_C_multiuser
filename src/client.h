#include "proto.h"

#ifndef LIST
#define LIST

typedef struct UserNode {
    int data;
    char user_code[LENGTH_CODE];
    char public_key[LENGTH_KEY]; //initialized -> 0으로 시작 not initialized->1로 시작
    struct UserNode* prev;
    struct UserNode* link;
    char* user_name;
} UserList;

UserList *newNode(int sockfd, char* user_name,char*user_code,char* public_key) {
    UserList *np = (UserList *)malloc( sizeof(UserList) +1);

    np->data=sockfd;
    np->prev = NULL;
    np->link = NULL;

    strncpy(np->user_code,user_code,LENGTH_CODE);
    if(user_name){
        np->user_name=(char*) malloc(sizeof(char)*LENGTH_NAME);
        strncpy((np->user_name), user_name,strlen(user_name));
    }
    
    if(public_key){
        strncpy((np->public_key), public_key,strlen(public_key));
    }
    printf("[CLIENT.H/NEW NODE] Wrap up: user code %s",np->user_code);
    return np;
}
UserList *updatePublicKey(UserList * li,char*user_code,char*public_key){

    UserList *p=li; //while문으로 변경하기 
    int isUpdated=0;
    while(p){
        if(user_code && p->user_code){ //if not null
            if(strncmp(p->user_code,user_code,LENGTH_CODE)==0){ //찾는 유저가 맞다면
                printf("\n[CLIENT.H/UPDATE PUBLIC KEY] Found matching user code %s",p->user_code);
                memset(p->public_key,0,sizeof(char)*LENGTH_KEY);
                strncpy(p->public_key,public_key,strlen(public_key));     
                isUpdated=1;
                break;
            }
            else{
                p=p->link;
            }

        }
        else{
            
            printf('\n[CLIENT.H UPDATE PUBLIC KEY] Either the given user code or the pointers user code is null');
        }
    }
    if(isUpdated==0){
        printf("[CLIENT.H/UPDATEPUBLICKEY]: failed to find user, failed to update PUBLIC KEY\n");
    }
}
UserList *updateUserName(UserList * root,char*user_code,char*user_name){

    UserList *p=root; //while문으로 변경하기 
    int isUpdated=0;
    while(p){ //while p is not null 
        if((user_code) && (p->user_code)){ //if not null
            if(strncmp(p->user_code,user_code,LENGTH_CODE)==0){ //찾는 유저가 맞다면
                printf("\n[CLIENT.H/UPDATE USERNAME] Found matching user %s",p->user_name);
                memset(p->user_name,0,sizeof(char)*LENGTH_NAME);
                strncpy((p->user_name),user_name,strlen(user_name));
                isUpdated=1;
                break;
            }
            else{
                p=p->link;
            }

        }
        else{
            printf('\n[CLIENT.H UPDATE USERNAME] Either the given user code or the pointers user code is null');
            break;
        }
    }
    if(isUpdated==0){
        printf("[CLIENT.H/UPDATEUSERNAME]: failed to find user, failed to update USER NAME\n");
    }
}

char* getPublicKey(UserList *li, char*user_code){
    char public_key[LENGTH_CODE];
    UserList *p=li; //while문으로 변경하기 
    while(p){
        if(user_code){ //if not null
            if(strncmp(p->user_code,user_code,LENGTH_CODE)==0){ //찾는 유저가 맞다면
                if(p->public_key){
                    return p->public_key;
                }
                else{
                    printf("[CLIENT.H/GETPUBLICKEY]: User has no PUBLIC KEY");
                }
            }
            else{
                p=p->link;
            }

        }
    }
    printf("[CLIENT.H/GETPUBLICKEY]: Failed to find user");
    return public_key;
}

#endif // LIST