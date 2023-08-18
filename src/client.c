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
#include "client.h"
#include "string.h"

'''
고민
항상 이름은 \0으로 끝나는 걸 어떻게 해야 보장할까?
'''
// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char my_user_code[LENGTH_CODE]={};
char my_nickname[LENGTH_NAME] = {};
char my_public_key[LENGTH_KEY]={};
UserList *root, *now;

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

//if message startswith 13, it is a secret message
//ask for the public key of sender to server
//receive the public key of sender from server
//decrypt the message
//show in screen

void recv_secret_handler(){//called as a new

}
void send_secret_handler(){ //called in send_msg_handler

}
void recv_msg_handler() {
    char tmp_user_code[LENGTH_CODE];
    char tmp_user_name[LENGTH_NAME];
    char tmp_message[LENGTH_MSG];
    char tmp_key[LENGTH_KEY];
    char tmp_encrypted_message[LENGTH_MSG];
    char tmp_decrypted_message[LENGTH_MSG];
    char receiveMessage[LENGTH_SEND] = {};
    while (1) {
        int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);
        if (receive > 0) {

            //if startswith 11, receive usercode and username pair
            if(strncmp(receiveMessage,"11",2)==0){
                strncpy(tmp_user_code,receiveMessage+2,LENGTH_CODE);
                strcpy(tmp_user_name,receiveMessage+2+LENGTH_CODE);
                //save it in struct
                root->link=newNode(sockfd,user_name,user_code,NULL);
            }
            
            //if startswith 13, receive usercode and secret message 
            else if(strncmp(receiveMessage,"13",2)==0){
                printf("[CLIENT.C RECV MSG HANDLER] Secret message incoming");
                strncpy(tmp_user_code,receiveMessage+2,LENGTH_CODE);
                strcpy(tmp_encrypted_message,receiveMessage+2+LENGTH_CODE);

                //decrypt 
                if(my_public_key){
                    tmp_decrypted_message="HI" //decrypt(tmp_encrypted_message,my_public_key)
                    printf("[CLIENT.C RECV MSG HANDLER] %s",tmp_decrypted_message);
                }//else, fail
                else{
                    //NEED WORK
                    printf("[CLIENT.C RECV MSG HANDLER] Secret message was not decrypted because my public key did not exist")
                }
                


            }
            
            //if startswith 14, receive usercode and public key pair 
            else if (strncmp(receiveMessage,"14",2)==0)
            {
                printf("[CLIENT.C RECV MSG HANDLER] received key");
                strncpy(tmp_user_code,receiveMessage+2,LENGTH_CODE);
                strncpy(tmp_key,receiveMessage+2+LENGTH_CODE,LENGTH_KEY);

            }
            
            //if startswith 02, receive usercode and message 
            else if (strncmp(receiveMessage,"02",2)==0)
            {
                printf("[CLIENT.C RECV MSG HANDLER] received pub message");
                strncpy(tmp_user_code,receiveMessage+2,LENGTH_CODE);
                strncpy(tmp_message,receiveMessage+2+LENGTH_CODE,LENGTH_MSG);

            }
            
            
            
        } else if (receive == 0) {
            break;
        } else { 
            // -1 
        }
    }
}

void send_msg_handler() {
    char message[LENGTH_MSG] = {};
    while (1) {
        str_overwrite_stdout();
        while (fgets(message, LENGTH_MSG, stdin) != NULL) {
            str_trim_lf(message, LENGTH_MSG);
            if(strncmp(message,"SECRET",6)==0){
                printf("Send secret message to: ");
                //list of other users
            }
            if (strlen(message) == 0) {
                str_overwrite_stdout();
            } else {
                break;
            }
        }
        //1. Stub
        char sent_message[8+LENGTH_MSG];
        strncpy(sent_message,"01",2);
        strncpy(sent_message+2,"999999",6);
        send(sockfd, sent_message, 8+LENGTH_MSG, 0);
        if (strcmp(sent_message, "exit") == 0) {
            break;
        }
    }
    catch_ctrl_c_and_exit(2);
}

int main()
{
    signal(SIGINT, catch_ctrl_c_and_exit);

    // Naming
    printf("Please enter your name: ");
    if (fgets(nickname, LENGTH_NAME, stdin) != NULL) {
        str_trim_lf(nickname, LENGTH_NAME);
    }
    if (strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1) {
        printf("\nName must be more than one and less than thirty characters.\n");
        exit(EXIT_FAILURE);
    }

    // Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
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
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_info.sin_port = htons(8888);

    // Connect to Server
    int err = connect(sockfd, (struct sockaddr *)&server_info, s_addrlen);
    if (err == -1) {
        printf("Connection to Server error!\n");
        exit(EXIT_FAILURE);
    }
    
    // Names
    getsockname(sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
    getpeername(sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);
    printf("Connect to Server: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));
    printf("You are: %s:%d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
    print("If you want to send a secret message, type SECRET\n\n");


    // Receive user code from server first
    // [10][user_code]
    char receiveMessage[LENGTH_SEND];
    int receive=recv(sockfd, receiveMessage, LENGTH_SEND, 0);

    if (receive > 0) { //recieve가 제대로 되어야 시작해라 
        while(strncmp(receiveMessage,"10",2)!=0){ //10으로 시작하지 않는다면 버리고 다시 받아라 
            receive=recv(sockfd, receiveMessage, LENGTH_SEND, 0);
            if (receive <=0){
                continue;
            }
        }
        strncpy(my_user_code,receiveMessage+2,6);
        printf("[CLIENT.C/MAIN] you received your code:%s",strcat(my_user_code,"\0"));
    }
    
    
    // Send server my nickname 
    //[00][user_code][nickname]
    char sendMessage[LENGTH_SEND];
    strncpy(sendMessage,"00",2);
    strncpy(sendMessage+2,my_user_code,LENGTH_CODE);
    strncpy(sendMessage+2+LENGTH_CODE,my_nickname,strlen(my_nickname));
    send(sockfd, sendMessage, LENGTH_NAME, 0);

    // Send server my public key 
    // My Keys
    // 통합 
    char sendMessage[LENGTH_SEND];
    strncpy(sendMessage,"04",2);
    strncpy(sendMessage+2,my_user_code,LENGTH_CODE);
    strncpy(sendMessage+2+LENGTH_CODE,my_public_key,strlen(my_public_key));
    send(sockfd, sendMessage, LENGTH_NAME, 0);

    // Root user node 
    root = newNode(sockfd, my_nickname,my_user_code,my_public_key);
    now = root;

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}