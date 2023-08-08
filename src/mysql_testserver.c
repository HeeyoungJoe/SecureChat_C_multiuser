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

void client_handler(void *p_client) {
    int leave_flag = 0;
    char recv_buffer[100];
    char send_buffer[100];
    ClientList *np = (ClientList *)p_client;
    while (1) {
        int receive = recv(np->data, recv_buffer, sizeof(recv_buffer), 0);
        if (receive > 0) {
            sprintf(send_buffer, "%s from %s", recv_buffer, np->ip);
        } else if (receive == 0 || strcmp(recv_buffer, "exit\n") == 0) {
            sprintf(send_buffer, "%d leave the chatroom.", np->data);
            leave_flag = 1;
        } else {
            printf("Fatal Error: -1\n");
        }

        // Send to all client except self.
        ClientList *tmp = root->link;
        while (tmp != NULL) {
            if (np->data != tmp->data) {
                printf("Send to sockfd %d: \"%s\" \n", tmp->data, send_buffer);
                send(tmp->data, send_buffer, sizeof(send_buffer), 0);
            }
            tmp = tmp->link;
        }
        if (leave_flag) {
            break;
        }
    }

    // Remove Node
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