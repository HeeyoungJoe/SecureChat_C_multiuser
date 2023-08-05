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

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char nickname[LENGTH_NAME] = {};
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
    char tmp_user_code[6];
    char receiveMessage[LENGTH_SEND] = {};
    while (1) {
        //if startswith 11, receive usercode and username pair
        //if startswith 14, receive usercode and public key pair 
        int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);
        if (receive > 0) {
            if(strncmp(receiveMessage,"11",2)==0){
                //save it in struct
                root->link=newNode(sockfd,user_name,user_code,NULL);
            }
            else if (strncmp(receiveMessage,"14",2)==0)
            {
            
            }
            else if (strncmp(receiveMessage,"02",2)==0)
            {
                printf("\r%s\n", receiveMessage);
                str_overwrite_stdout();
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
    send(sockfd, nickname, LENGTH_NAME, 0);
    
    // My Keys
    char public_key[247]={0};

    // Handshake with Server
    char receiveMessage[LENGTH_SEND];
    int receive = recv(sockfd, receiveMessage, LENGTH_SEND, 0);
    char userCode[6];
    if (receive > 0) { //내 user code를 받을 땐 10으로 시작 
        if(strncmp(receiveMessage,"10",2)==0){
            strncpy(userCode,receiveMessage+2,6);
        }
        else{
            printf("[CLIENT.C/MAIN]: Didn't receive client code\n");
            userCode="000000";
        }
    }

    // Root user node 
    root = newNode(sockfd, nickname,user_code,public_key);
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