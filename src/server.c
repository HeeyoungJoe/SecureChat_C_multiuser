#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "proto.h"
#include "server.h"


// Global variables
int server_sockfd = 0, client_sockfd = 0;
ClientList *root, *now;
char tmp_user_code[7];

int equals(char *w1, char *w2, int length){
    char *ww1[length];
    char *ww2[length];
    strncpy(ww1,w1,length);
    strncpy(ww2,w2,length);
    if(ww1==ww2){
        return 1;//true
    }
    else{
        return 0;//false
    }
}
void catch_ctrl_c_and_exit(int sig) {
    ClientList *tmp;
    while (root != NULL) {
        printf("\nClose socketfd: %d\n", root->data);
        close(root->data); // close all socket include server_sockfd
        tmp = root;
        root = root->link;
        free(tmp);
    }
    printf("Bye\n");
    exit(EXIT_SUCCESS);
}

void send_to_all_clients(ClientList *np, char tmp_buffer[]) {
    ClientList *tmp = root->link;
    while (tmp != NULL) {
        if (np->data != tmp->data) { // all clients except itself.
            printf("Send to sockfd %d: \"%s\" \n", tmp->data, tmp_buffer);
            send(tmp->data, tmp_buffer, LENGTH_SEND, 0);
        }
        tmp = tmp->link;
    }
}

//첫 메세지가 아닐 때
//01->공개키 공유 요청  [01]-[보내는 user code]-[공개키를 원하는 user code]
//02->전체 공개 메세지 [02]-[보내는 user code]-[암호화 안 된 메세지]
//03->귓속말 메세지 [03]-[보내는 user code]-[받는 user code]-[암호화 된 메세지]


void selective_message_send(ClientList *np, char tmp_buffer[]){

    char selector[3],user_code[7];
    strncpy(selector,tmp_buffer,2);
    strncpy(user_code,tmp_buffer+2,6);

    if (equals(selector,"01",2)){//공개키 요구
        char recv_user_code[7];
        strncpy(recv_user_code,tmp_buffer+8,6);
        ClientList *tmp = root->link;
        while (tmp != NULL) {
            if (recv_user_code == tmp->user_code) { 
                //공개키 보내기
                //[14]-[공개키를 원하는 user code]-[공개키] 
                int message_length=137;
                char message[message_length];
                strncpy(message,"14",2);
                strncpy(message+2,recv_user_code,6);
                strncpy(message+8,tmp->public_key,128);
                printf("Send to user %d: \"%s\" \n", np->data, message);
                send(np->data, message, message_length, 0);

                return 0;
            }
            tmp = tmp->link;
        }
    }
    else if (equals(selector,"02",2))
    {
        send_to_all_clients(np,tmp_buffer);
    }
    else if (equals(selector,"03",2))
    {
        char recv_user_code[7];
        strncpy(recv_user_code,tmp_buffer+8,6);
        ClientList *tmp = root->link;
        while (tmp != NULL) {
            if (recv_user_code == tmp->user_code) { 
                //귓속말 보내기
                //[13]-[보내는 user code]-암호화된 메세지
                int message_length=strlen(tmp_buffer-6);
                char message[message_length];
                strncpy(message,"13",2);
                strncpy(message+2,tmp_buffer+8,strlen(tmp_buffer+8));//보내는 user code, selector 제외 
                printf("Send to sockfd %d: \"%s\" \n", tmp->data, message);
                send(tmp->data, message, message_length, 0);

                return 0;
            }
            tmp = tmp->link;
        }
    }
    else{
        printf("Selector error: %s",selector);
    }
    
    return 0;
}
void client_handler(void *p_client) {
    int leave_flag = 0;
    char public_key[128]={};
    char nickname[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_MSG] = {};
    char send_buffer[LENGTH_SEND] = {};
    ClientList *np = (ClientList *)p_client;
    ClientList *tmp=root->link;

    // Naming
    if (recv(np->data, nickname, LENGTH_NAME, 0) <= 0 || strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1) {
        printf("%s didn't input name.\n", np->ip);
        leave_flag = 1;
    } else {
        
        //[10]-[내 user code]-Done
        char code_init_message[9];
        int point=2;
        strncpy(code_init_message,"10",2);
        strncpy(code_init_message+2,np->user_code,6);
        send(np->data,code_init_message,strlen(code_init_message),0);

        //[11]-[다른 유저 코드]-[다른 유저 이름] 나에게 -Done
        while(tmp!=NULL){            
            char code_recv_message[50];
            strncpy(code_recv_message,'11',2);
            strncpy(code_recv_message+2,tmp->user_code,6);
            strncpy(code_recv_message+8,tmp->name,strlen(tmp->name));
            send(np->data,code_recv_message,strlen(code_recv_message),0);
            tmp=tmp->link;
        }

        //[11]-[유저 코드]-[유저 이름] 다른 모두에게 -Done
        char code_update_message[50];
        strncpy(code_update_message,"11",2);
        strncpy(code_update_message+2,np->user_code,6);
        strncpy(code_update_message+8,np->name,strlen(np->name));
        send_to_all_clients(np->data,code_update_message);


        strncpy(np->name, nickname, LENGTH_NAME);
        printf("%s(%s)(%d) join the chatroom.\n", np->name, np->ip, np->data);
        sprintf(send_buffer, "%s(%s) join the chatroom.", np->name, np->ip);
        send_to_all_clients(np, send_buffer);
    }

    // np의 공개키 받기 - Done
    if (recv(np->data, public_key, 128, 0) <= 0 || strlen(public_key) < 2 || strlen(public_key) >= LENGTH_NAME-1) {
        printf("%s didn't input public key.\n", np->ip);
        leave_flag = 1;
    } else {
        strncpy(np->public_key,public_key,strlen(public_key)); 
        printf("%s public key received",np->name);
    }

    // Conversation
    while (1) {
        
        if (leave_flag) {
            break;
        }
        int receive = recv(np->data, recv_buffer, LENGTH_MSG, 0);
        if (receive > 0) {
            //귓속말인지 broadcast 되어도 될 말인지 키 요청인지 구분 -Done
            if (strlen(recv_buffer) == 0) {
                continue;
            }
            //sprintf(send_buffer, "%s：%s from %s", np->name, recv_buffer, np->ip);
            selective_message_send(np,send_buffer);
        } else if (receive == 0 || strcmp(recv_buffer, "exit") == 0) {
            printf("%s(%s)(%d) leave the chatroom.\n", np->name, np->ip, np->data);
            sprintf(send_buffer, "%s(%s) leave the chatroom.", np->name, np->ip);
            leave_flag = 1;
        } else {
            printf("Fatal Error: -1\n");
            leave_flag = 1;
        }
        //귓속말과 비귓속말 따로 보내기 
        //send_to_all_clients(np, send_buffer);
    }

    // Remove Node
    close(np->data);
    if (np == now) { // remove an edge node
        now = np->prev;
        now->link = NULL;
    } else { // remove a middle node
        np->prev->link = np->link;
        np->link->prev = np->prev;
    }
    free(np);
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit);

    // Create socket
    server_sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (server_sockfd == -1) {
        printf("Fail to create a socket.");
        exit(EXIT_FAILURE);
    }

    // Socket information
    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = PF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_port = htons(8888);

    // Bind and Listen
    bind(server_sockfd, (struct sockaddr *)&server_info, s_addrlen);
    listen(server_sockfd, 5);

    // Print Server IP
    getsockname(server_sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Start Server on: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));

    // Initial linked list for clients
    root = newNode(server_sockfd, inet_ntoa(server_info.sin_addr));
    now = root;

    while (1) {
        client_sockfd = accept(server_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);

        // Print Client IP
        getpeername(client_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
        printf("Client %s:%d come in.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

        // Append linked list for clients
        ClientList *c = newNode(client_sockfd, inet_ntoa(client_info.sin_addr));
        strncpy(c->user_code,now->user_code,6);
        strncpy(c->user_code,inc(c->user_code,c->user_code+5),6);//NEED TEST
        c->prev = now;
        now->link = c;
        now = c;

        pthread_t id;
        if (pthread_create(&id, NULL, (void *)client_handler, (void *)c) != 0) {
            perror("Create pthread error!\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}