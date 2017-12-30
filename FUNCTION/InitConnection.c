//
// Created by Eddie Lee on 12/24/17.
//

#include "InitConnection.h"

int init_login_dis_conn(int *port){
    int server_fd,on=1;
    struct sockaddr_in *server_add;
    server_add = malloc(sizeof(struct sockaddr_in));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_add->sin_addr.s_addr = htonl(INADDR_ANY);
    server_add->sin_family = AF_INET;
    server_add->sin_port = port;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int));

    if (bind(server_fd, server_add, sizeof(struct sockaddr_in)) == -1) {
        perror("bind error");
        exit(-1);
    }

    if (listen(server_fd,QUEUE) == -1) {
        perror("listen");
        exit(-1);
    }

    return server_fd;
}