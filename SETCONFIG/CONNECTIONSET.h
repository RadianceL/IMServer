#ifndef WEBTEST_SET_H
#define WEBTEST_SET_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct _SOCKET_INFORMATION   {
    char *name;        // 发送和接收数据的缓冲区
    int Socket;                      // 与客户端进行通信的套接字
} SOCKET_INFORMATION;


#define BUFFER_SIZE 256 // 这是接收缓冲区的大小， 发送缓冲区动态指定，动态分配
#define QUEUE 10        // 设置队列
#define INDEX 1000

#endif