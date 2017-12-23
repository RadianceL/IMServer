#include "Connection.h"

int initServer() {
    int server_fd,on=1;
    struct sockaddr_in *server_add;
    server_add = malloc(sizeof(struct sockaddr_in));


    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_add->sin_addr.s_addr = htonl(INADDR_ANY);
    server_add->sin_family = AF_INET;
    server_add->sin_port = htons(6667);

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

void thread(void* args) {

    char buffer[BUFFER_SIZE];
    int client_sock_fd = *(int *) args;

    if (client_sock_fd < 0) {
        perror("accept error:"); // 打印连接出错信息
    }
    memset(buffer,0,BUFFER_SIZE);

    int size = read(client_sock_fd,buffer,BUFFER_SIZE);
    write(1,buffer, size);

    //清理内存空间
    free(client_sock_fd);

    close(client_sock_fd);
    pthread_exit(0);
}

void connection() {
    int server_fd = initServer();
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    while (666){
        pthread_t thread_id;
        //阻塞，直到有用户连接
        int *client_sockfd = (int *) malloc(sizeof(int));
        *client_sockfd = accept(server_fd, &client_addr, &len);
        //开启线程

        //在顺序执行不循环的状态下，顺序执行，主线程执行完直接结束，可能不执行thread
        pthread_create(&thread_id,NULL,thread,client_sockfd);
    }
    close(server_fd);
}

void process_test() {
    printf("start...\n");
    fflush(stdout);
    int mFork = fork();
    //进程，复制内存（context），做轮寻 分配时间片
    if (mFork > 0) {
        printf("%d\n\n",mFork);
        for (int i = 0; i < 100; ++i) {
            printf("main process ...%d\n", i);
        }
    } else{
        printf("%d\n\n\n",mFork);
        for (int i = 0; i < 100; ++i) {
            printf("\tsub process ...%d\n", i);
        }
    }
}