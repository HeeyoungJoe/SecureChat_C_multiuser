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
#include <mysql/mysql.h>
#include "proto.h"
#include "server.h"


// Global variables
int server_sockfd = 0, client_sockfd = 0;
ClientList *root, *now;
extern const int LENGTH_USERCODE=6;

struct client_handler_arg {
    ClientList* p_client;
    ClientList* root_node;
};

int equals(char *w1, char *w2, int length){
    char ww1[length];
    char ww2[length];
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
        if (np->data != tmp->data) { // all clients except server
            printf("Send to sockfd %d: \"%s\" \n", tmp->data, tmp_buffer);
            send(tmp->data, tmp_buffer, LENGTH_SEND, 0);
        }
        tmp = tmp->link;
    }
}

char * encodeMessage(char*type,){
    char msg[100];

    return msg;
}

char * decodeMessage(char* type){
    char msg[100];

    return msg;

}

void getKey(char** key_buffer, char* user_code){

}
int getFd(char* user_code){
    return 0;
}
//첫 메세지가 아닐 때
//01->공개키 공유 요청  [01]-[보내는 user code]-[공개키를 원하는 user code]
//02->전체 공개 메세지 [02]-[보내는 user code]-[암호화 안 된 메세지]
//03->귓속말 메세지 [03]-[보내는 user code]-[받는 user code]-[암호화 된 메세지]


/* int selective_message_send(ClientList *np, char tmp_buffer[]){

    char selector[3],user_code[LENGTH_USERCODE];
    strncpy(selector,tmp_buffer,2);
    strncpy(user_code,tmp_buffer+2,6);

    if (equals(selector,"01",2)){//공개키 요구
        char recv_user_code[LENGTH_USERCODE];
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

                return 1;
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
        char recv_user_code[LENGTH_USERCODE];
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

                return 1;
            }
            tmp = tmp->link;
        }
    }
    else{
        printf("Selector error: %s",selector);
        return 0;
    }
    
    return 1;
} */
void client_handler(void *p_client) {
    int leave_flag = 0;
    char recv_type_flag[2];
    char user_code[LENGTH_CODE] = {};
    char public_key[128]={};
    char nickname[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_MSG] = {};
    char send_buffer[LENGTH_SEND] = {};
    char tmp_user_code[LENGTH_CODE];
    char tmp_user_name[LENGTH_NAME];
    char tmp_message[LENGTH_MSG];
    char tmp_key[LENGTH_KEY];
    char tmp_encrypted_message[LENGTH_MSG];
    ClientList *np = (ClientList *)p_client;
    ClientList *tmp =(ClientList *)malloc(sizeof(ClientList)+1);
    tmp=root->link;


    // 1. 코드 주기 

    if (recv(np->data,user_code,LENGTH_CODE+2,0)<= 0 || strlen(user_code) < 2 || strlen(user_code) >= LENGTH_CODE-1){
        printf("%s didn't input name.\n", np->ip);
        leave_flag = 1;
    } else {        
        //[10]-[내 user code]-Done/Test done
        char *code_init_message=(char*)malloc(sizeof(char)*(2+LENGTH_USERCODE));
        strncpy(code_init_message,"10",2);
        strncpy(code_init_message+2,np->user_code,6);
        send(np->data,code_init_message,strlen(code_init_message),0);
        printf("\nUser code sent: %s\n",code_init_message);
        free(code_init_message);
                

    }

    // 2. 닉네임 받기
    // 3. 다른 유저들에게 이 유저의 닉네임과 코드 뿌리기 

    if (recv(np->data, nickname, LENGTH_NAME+8, 0) <= 0 || strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1) {
        printf("%s didn't input name.\n", np->ip);
        leave_flag = 1;
    } else {
        // [00]-[user_code]-[user_nickname]
        strncpy(np->name, nickname, LENGTH_NAME);
        printf("%s(%s)(%d) join the chatroom.\n", np->name, np->ip, np->data);
        sprintf(send_buffer, "%s(%s) join the chatroom.", np->name, np->ip);
        
    
        //[12]-[유저 코드]-[유저 이름] 다른 모두에게 -Done /Test Done
        char code_update_message[50];
        strncpy(code_update_message,"12",2);
        strncpy(code_update_message+2,np->user_code,6);
        strncpy(code_update_message+8,np->name,strlen(np->name));
        send_to_all_clients(np,code_update_message); // NEED WORK: root(=server) 도 제외하는지 확인
        printf("신고식 legend:%s",code_update_message); 

    }

    // 3. 이 유저에게 다른 유저들의 닉네임과 코드 페어 주기

    // NEED WORK : thread를 별도로 주면 좋을까? 
    // [11]-[다른 유저 코드]-[다른 유저 이름] 새로 들어온 클라이언트에게 기존의 클라이언트 정보 전달 -Done
    //Segmentation fault core dumped
    tmp=root->link;
    while(tmp!=NULL){          
        if(tmp!=np){  
            char *code_recv_message=(char*)malloc(sizeof(char)*(2+LENGTH_USERCODE*2)+1);
            strncpy(code_recv_message,'11',2);
            strncpy(code_recv_message+2,tmp->user_code,6);
            strncpy(code_recv_message+8,tmp->name,strlen(tmp->name));
            send(np->data,code_recv_message,strlen(code_recv_message),0);
        }
        tmp=tmp->link;
    } 


    // 4. np의 공개키를 받기

    if (recv(np->data, public_key, LENGTH_KEY+8, 0) <= 0 || strlen(nickname) < 2 || strlen(nickname) >= LENGTH_KEY-1) {
        printf("%s didn't input public key.\n", np->ip);
        leave_flag = 1;
    } else {
        // [04][내 user code] [내 public key]
        // np의 공개키 받기 - Done
        strncpy(np->public_key,public_key,strlen(public_key)); 
        printf("%s public key received",np->name);
    }



    // 5. 대화하기 
    while (1) {
        
        // 떠날지 말지 결정
        if (leave_flag) {
            break;
        }

        // 메세지 받기
        int receive = recv(np->data, recv_buffer, LENGTH_MSG, 0);
        if (receive > 0) {

            strncpy(recv_type_flag,recv_buffer,2);

            // 1) 귓속말 요청: 
            // - 그 사람만 읽을 수 있는 메세지를 보내기 위해, 퍼블릭 키 요청
            //   [01]-[보내는 user code]-[공개키를 원하는 user code]
            if(recv_type_flag=="01"){
                //find usercode
                strncpy(tmp_user_code,recv_buffer+2,LENGTH_CODE);
                getKey(*tmp_key,tmp_user_code); //NEED WORK: Not coded yet
                strncpy(tmp_user_code,recv_buffer+2+LENGTH_CODE,LENGTH_CODE);
                send(getFd(tmp_user_code),tmp_key,LENGTH_KEY,0);

                
            }

            // - "그 사람" 찾아서 MYSQL에서 퍼블릭 키 찾아 보내주기
            // [14]-["그 사람" user code]-[공개키]
            else if (recv_type_flag=="14"){

            }
            // 2) 귓속말 전달
            // [13]-[보내는 user code]-암호화된 메세지 
            // send
            else if ( recv_type_flag=="13"){
                strncpy(tmp_user_code,recv_buffer+2,LENGTH_CODE);
                send(getFd(tmp_user_code),recv_buffer,strlen(recv_buffer),0);
            }
            // 3) 공개말 전달
            // [02]-[보내는 유저 코드]-[암호화 안 된 메세지]
            // send to all
            else if(recv_type_flag=="02"){
                send_to_all_clients(np,recv_buffer);
            }
            

            //sprintf(send_buffer, "%s：%s from %s", np->name, recv_buffer, np->ip);
            //selective_message_send(np,send_buffer);
        } else if (receive == 0 || strcmp(recv_buffer, "exit") == 0) {
            printf("%s(%s)(%d) leave the chatroom.\n", np->name, np->ip, np->data);
            sprintf(send_buffer, "%s(%s) leave the chatroom.", np->name, np->ip);
            leave_flag = 1;
        } else {
            printf("Fatal Error: -1\n");
            leave_flag = 1;
        }
        
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
    root = newNode(server_sockfd, inet_ntoa(server_info.sin_addr),"000000");
    now = root;
    printf("\n\nRoot initialized:%s\n",root->user_code);

    // Database connection- MySQL :https://stackoverflow.com/questions/3453168/c-program-mysql-connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server = "localhost";
    char *user = "root";
    char *password = "Yuzuka601@"; /* set me first */
    char *database = "user_database";
    conn = mysql_init(NULL);
    /* Connect to database */
    if (!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    /* send SQL query */
    if (mysql_query(conn, "show tables")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn);

    /* output table name */
    printf("MySQL Tables in mysql database:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s \n", row[0]);



    while (1) {
        client_sockfd = accept(server_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);

        // Print Client IP
        getpeername(client_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
        printf("Client %s:%d come in.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

        // Append linked list for clients
        ClientList *c = newNode(client_sockfd, inet_ntoa(client_info.sin_addr),inc(now->user_code,now->user_code+5));
        c->prev = now;
        now->link = c;
        now = c;
        printf("ClientNode appended to the list:%s\n",c->user_code);

        // Prepare Argument for client handler
        struct client_handler_arg args;
        args.p_client = c;
        args.root_node = root;

        // Call client handler thread
        pthread_t id;
        if (pthread_create(&id, NULL, (void *)client_handler, (void *)&args) != 0) {
            perror("Create pthread error!\n");
            exit(EXIT_FAILURE);
        }
    }

    /* close connection */
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
