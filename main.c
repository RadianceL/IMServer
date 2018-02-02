#include <static_lib.h>
#include <stdbool.h>
#include "../SETCONFIG/CONNECTIONSET.h"

pSOCKET_INFORMATION pHead;

//发送好友信息到客户端
void send_friend_list_to_client(int server_fd);
//获取登陆信息
LOGIN_INFO* get_login_info(char json[]);
//获取登出信息
LOGOUT_INFO* get_logout_info(char json[]);

CHAT_INFO* get_chat_info(char json[]);

ADD_INFO* get_Add_INFO(char json[]);

//记录登陆信息
void creater_SOCKER_ARRAY_index(pSOCKET_INFORMATION thisHead,int socket,LOGIN_INFO name);
//登出，并关闭socket
void delete_SOCKER_ARRA_index(pSOCKET_INFORMATION thisHead,char* index);

void search_chat_with(pSOCKET_INFORMATION pINFORMATION, char* param);

bool search_is_already_create(pSOCKET_INFORMATION thisHead,char* account);

void send_status_info(int client_sock_fd, int status, char *time);

//初始化服务器连接
int initServer() {
    //声明server句柄，setsockopt的开关量
    int server_fd,on=1;
    //声明结构体用于初始化网络连接
    struct sockaddr_in *server_add;
    //给server_add分配内存空间**全程使用，不需要free掉
    server_add = malloc(sizeof(struct sockaddr_in));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_add->sin_addr.s_addr = htonl(INADDR_ANY);
    server_add->sin_family = AF_INET;
    server_add->sin_port = htons(6666);

    //设置端口复用
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int));

    //绑定端口，如果成功返回1，失败返回0
    if (bind(server_fd, server_add, sizeof(struct sockaddr_in)) == -1) {
        perror("bind error");
        exit(-1);
    }
    //监听端口，如果成功返回1，失败返回0
    if (listen(server_fd,QUEUE) == -1) {
        perror("listen");
        exit(-1);
    }

    //返回配置好的句柄
    return server_fd;
}


void thread(void* args) {

    if (pHead == NULL){
        pHead = malloc(sizeof(SOCKET_INFORMATION));
    }

    //客户机传上来的数据
    char buffer_client[BUFFER_SIZE];

    //客户机的连接句柄
    int client_sock_fd = *(int *) args;

    //判定连接是否可用
    if (client_sock_fd < 0) {
        perror("accept error:"); // 打印连接出错信息
    }

    //清理buffer空间
    memset(buffer_client, 0, BUFFER_SIZE);
    //获取可用字符串
    int size = read(client_sock_fd, buffer_client, BUFFER_SIZE);

    if(size < 0){
        perror("read STDIN_FILENO");
        exit(1);
    }

    //初次判断接收的请求Action是什么
    CHAT_INFO* info = malloc(sizeof(CHAT_INFO));
    //为info赋值
    getAction(info,buffer_client);

    //内存占用8个字节,static,whitout malloc
    LOGIN_INFO* login_info;
    LOGOUT_INFO* logout_info;
    CHAT_INFO* chat_info;

    switch (info->ACTION) {
        case 0:
            login_info = get_login_info(buffer_client);
            if (search_is_already_create(pHead,login_info->account)) {
                creater_SOCKER_ARRAY_index(pHead,client_sock_fd, *login_info);
                send_status_info(client_sock_fd, 777, login_info->t);
                send_friend_list_to_client(client_sock_fd);
            } else {
                send_status_info(client_sock_fd, 444, login_info->t);
            }
            break;
        case 886:
            logout_info = get_logout_info(buffer_client);
            if (!search_is_already_create(pHead,logout_info->account)) {
                delete_SOCKER_ARRA_index(pHead,logout_info->account);
                send_status_info(client_sock_fd, 777, logout_info->t);
            } else {
                send_status_info(client_sock_fd, 444, logout_info->t);
            }
            close(client_sock_fd);
            break;

        case 11:
            chat_info = get_chat_info(buffer_client);
            printf("%s\n",chat_info->message);
            if (!search_is_already_create(pHead,chat_info->account)){
                search_chat_with(pHead,chat_info->account);
                send_status_info(client_sock_fd, 777, chat_info->t);
            } else {
                send_status_info(client_sock_fd, 444, chat_info->t);
            }
            close(client_sock_fd);
            break;

        case 111:
            //add friend
            break;
    }
    printf("ready out pthread");
    pthread_exit(0);
}

void send_status_info(int client_sock_fd, int status, char *time) {
    RE_INFO r;
    r.STATUS = status;
    r.t = malloc(sizeof(16));
    r.t = time;
    char *pJ = re_status_info(&r);
    int len = strlen(pJ);
    send(client_sock_fd, pJ, len,0);
    send(client_sock_fd, "\n", sizeof("\n"),0);
    free(r.t);
}

int main() {
    int server_fd = initServer();
    printf("server init over\n");
    struct sockaddr_in *client_addr = malloc(sizeof(struct sockaddr_in));
    int len = sizeof(client_addr);

    while (666){
        pthread_t thread_id;
        //阻塞，直到有用户连接
        int *client_sockfd = (int *) malloc(sizeof(int));
        printf("waiting for connectiong...\n");
        *client_sockfd = accept(server_fd, client_addr, &len);

        //在顺序执行不循环的状态下，顺序执行，主线程执行完直接结束，可能不执行thread
        pthread_create(&thread_id,NULL,thread,client_sockfd);
    }

    close(server_fd);

    return EXIT_SUCCESS;
}

void send_friend_list_to_client(int server_fd) {
    FRIEND aFriend[3];
    strcpy(aFriend[0].u_name, "eddie");
    strcpy(aFriend[1].u_name, "alice");
    strcpy(aFriend[2].u_name, "jim");

    strcpy(aFriend[0].IP, "127.0.0.1");
    strcpy(aFriend[1].IP, "127.0.0.1");
    strcpy(aFriend[2].IP, "127.0.0.1");
    char *pJson = re_friend_info(&aFriend, 3);
    write(server_fd, pJson, strlen(pJson));
    write(server_fd, "\n", sizeof("\n"));
}

LOGIN_INFO * get_login_info(char json[]) {
    LOGIN_INFO *login_info = malloc(sizeof(LOGIN_INFO));
    send_client_login_info(login_info,json);

    return login_info;
}


LOGOUT_INFO * get_logout_info(char json[]) {
    LOGOUT_INFO *logout_info = malloc(sizeof(LOGOUT_INFO));
    send_client_logout_info(logout_info,json);

    return logout_info;
}

CHAT_INFO* get_chat_info(char json[]) {
    CHAT_INFO * chat_info = malloc(sizeof(CHAT_INFO));
    send_client_chat_info(chat_info,json);

    return chat_info;
}


ADD_INFO* get_Add_INFO(char json[]) {
    ADD_INFO *add_info = malloc(sizeof(ADD_INFO));
    send_add_chat_info(add_info,json);

    return add_info;
}

void creater_SOCKER_ARRAY_index(pSOCKET_INFORMATION thisHead,int socket,LOGIN_INFO ID){
    pSOCKET_INFORMATION  pCopy = thisHead;
    pSOCKET_INFORMATION  pNew = malloc(sizeof(SOCKET_INFORMATION));

    while (pCopy->next != NULL) {
        pCopy = pCopy->next;
    }

    pNew->info = ID;
    pNew->Socket = socket;
    pNew->next = NULL;

    pCopy->next = pNew;
    show(thisHead);
}

void delete_SOCKER_ARRA_index(pSOCKET_INFORMATION thisHead,char* account){
    pSOCKET_INFORMATION  old = thisHead;
    pSOCKET_INFORMATION  p = thisHead->next;
    while (old->next != NULL){
        if (!strcmp(p->info.account , account)){
            if(p->next!=NULL) {
                old->next = p->next;
                close(p->Socket);
                //waiting replace zhan
                free(p->info.account);
                free(p->info.passwd);
                free(p);
            } else {
                old->next = NULL;
            }
            break;
        }
        old= p;
        p = p->next;
    }
    show(thisHead);
}


void search_chat_with(pSOCKET_INFORMATION thisHead, char* param){
    pSOCKET_INFORMATION  pCopy = thisHead->next;

    while (pCopy!= NULL){
        if(!strcmp(pCopy->info.account,param)){
            printf("%d\n",pCopy->Socket);
            send(pCopy->Socket, "test",sizeof("test"),0);
            send(pCopy->Socket, "\n", sizeof("\n"),0);
        }
        pCopy = pCopy->next;
    }
}

void show(pSOCKET_INFORMATION thisHead){
    pSOCKET_INFORMATION  pCopy = thisHead->next;
    while (pCopy!= NULL){
        printf("account:%s.IP:%s.passwd:%s.%d\n",pCopy->info.account,pCopy->info.IP,pCopy->info.passwd,pCopy->Socket);
        pCopy = pCopy->next;
    }
}

bool search_is_already_create(pSOCKET_INFORMATION thisHead,char* account) {
    pSOCKET_INFORMATION  pCopy = thisHead->next;
    while (pCopy!= NULL){
        if (!strcmp(pCopy->info.account,account)){
            return 0;
        }
        pCopy = pCopy->next;
    }

    return 1;
}