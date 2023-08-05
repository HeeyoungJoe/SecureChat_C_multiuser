#include "proto.h"

#ifndef LIST
#define LIST

typedef struct UserNode {
    int data;
    char user_code[LENGTH_CODE];
    char public_key[248]; //initialized -> 0으로 시작 not initialized->1로 시작
    struct UserNode* prev;
    struct UserNode* link;
    char user_name[31];
} UserList;

UserList *newNode(int sockfd, char* user_name,char*user_code,char* public_key) {
    UserList *np = (UserList *)malloc( sizeof(UserList) +1);
    np->data=sockfd;
    np->prev = NULL;
    np->link = NULL;

    // Integrity check for user name
    // If user name is initialized, startwith 0
    // If user naem was never given, startwith 1
    if(user_name){
        strncpy(np->user_name,"0",1);
        strncpy((np->user_name+1), user_name,strlen(user_name));
    }
    else{
        strncpy(np->user_name,"1",1);
    }
    
    // Integrity check for public key
    // If public key is initialized, startwith 0
    // If public key was never given, startwith 1
    if(public_key){
        strncpy(np->public_key,"0",1);
        strncpy((np->public_key+1), public_key,strlen(public_key));
    }
    else{
        strncpy(np->user_name,"1",1);
    }
    strncpy(np->user_code,user_code,LENGTH_CODE);
    return np;
}

void printUser(int type, UserList* user){
    //type==1: user_name
    //type==2: user_code 
    //type==3: public_key 
    if(type==1){
        printf("%s",user->user_name+1);
    }
    else if(type==2){
        printf("%s",user->user_code+1);
    }
    else if(type==3){        
        printf("%s",user->public_key+1);
    }


}
UserList *updatePublicKey(UserList * li,char*user_code,char*public_key){

    UserList *p=li; //while문으로 변경하기 
    int isUpdated=0;
    while(p){
        if(user_code){ //if not null
            if(strncmp(p->user_code,user_code,LENGTH_CODE)==0){ //찾는 유저가 맞다면
                if(strncmp(p->public_key,"0",1)==0){
                    strncpy(p->public_key,"1",1);
                    strncpy((p->public_key+1),public_key,LENGTH_KEY);
                }
                else{
                    strncpy(p->public_key,"0",1);
                    strncpy((p->public_key+1),public_key,LENGTH_KEY);
                }
                isUpdated=1;
                break;
            }

            else{
                p=p->link;
            }

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