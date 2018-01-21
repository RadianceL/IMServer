#include <static_lib.h>
#include "../SETCONFIG/CONNECTIONSET.h"
#include "DATABASES/CONNECTDATABASE.h"

LOGIN_INFO * get_login_info(char json[]);
void test2();
void test3();
void test4();
void re_test();

void test();

void send_friend_list_to_client();

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
    //客户机传上来的数据
    char buffer_client[BUFFER_SIZE];

    //服务器将要发出的数据
    //char buffer_server[BUFFER_SIZE];

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

//  判定是否断开连接
//  if (buffer_client[0] == 'e' && buffer_client[1] == 'x' && buffer_client[2] == 'i' && buffer_client[3] == 't'){
//      break;
//  }
    buffer_client[size] = "\0";

    //初次判断接收的请求Action是什么
    CHAT_INFO* info;
    //为info赋值
    getAction(info,buffer_client);

    //内存占用8个字节
    LOGIN_INFO* login_info;

    switch (info->ACTION){
        case 0:
            login_info = get_login_info(buffer_client);

            //解析后查数据库，登陆成功反回状态信息
            if (1){
                RE_INFO r;
                r.STATUS = 7;
                r.t = "2018-01-01";
                char *pJ = re_status_info(&r);
                write(client_sock_fd, pJ, strlen(pJ));

                free(login_info);

                //主动发出信息
                //send_friend_list_to_client(login_info->IP);
            } else{
                RE_INFO r;
                r.STATUS = 444;//登陆失败，账号或密码错误
                r.t = "2018-01-01";
                char *pJ = re_status_info(&r);
                write(client_sock_fd, pJ, strlen(pJ));
            }

            break;

        case 886:

            break;

        case 11:

            break;

        case 111:
            connect_database_init();
            init_databases();
            add_user();
            break;
    }

    close(client_sock_fd);


    pthread_exit(0);
}

void send_friend_list_to_client() {
    int server_fd,on=1;
    struct sockaddr_in *server_add;
    server_add = malloc(sizeof(struct sockaddr_in));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_add->sin_addr.s_addr = htonl(INADDR_ANY);
    server_add->sin_family = AF_INET;
    server_add->sin_port = htons(6666);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(int));

    if (connect(server_fd, server_add, sizeof(struct sockaddr_in)) == -1) {
        perror("connect error");
        exit(-1);
    }


    FRIEND aFriend[3];
    strcpy(aFriend[0].u_name, "eddie");
    strcpy(aFriend[1].u_name, "alice");
    strcpy(aFriend[2].u_name, "jim");

    strcpy(aFriend[0].IP, "127.0.0.1");
    strcpy(aFriend[1].IP, "127.0.0.1");
    strcpy(aFriend[2].IP, "127.0.0.1");
    char *pJson = re_friend_info(&aFriend, 3);
    write(server_fd, pJson, strlen(pJson));

}

void connect_client() {
    int server_fd = initServer();
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    while (666){
        pthread_t thread_id;
        //阻塞，直到有用户连接
        int *client_sockfd = (int *) malloc(sizeof(int));
        *client_sockfd = accept(server_fd, &client_addr, &len);

        //在顺序执行不循环的状态下，顺序执行，主线程执行完直接结束，可能不执行thread
        pthread_create(&thread_id,NULL,thread,client_sockfd);
        free(client_sockfd);
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

int main() {
    connect_client();

    return EXIT_SUCCESS;
}

void test() {
    printf("----------------------------------\n");
    //test1();
    test2();
    test3();
    test4();
    re_test();
}

LOGIN_INFO * get_login_info(char json[]) {
    printf("Login_info : \n");
    LOGIN_INFO *login_info = malloc(sizeof(LOGIN_INFO));
    send_client_login_info(login_info,json);

    free(login_info);

    return login_info;
}


void test2() {
    printf("Logout_info : \n");
    LOGOUT_INFO *logout_info = malloc(sizeof(LOGOUT_INFO));
    char json[] = "{\"IP\":\"127.0.0.1\",\"ACTION\":886,\"account\":\"931305033\",\"time\":\"2018-01-01\"}";
    send_client_logout_info(logout_info,json);

    printf("IP : %s\n",logout_info->IP);
    printf("ACTION : %d\n",logout_info->ACTION);
    printf("account : %s\n",logout_info->account);
    printf("TIME : %s\n",logout_info->t);
    free(logout_info);
    printf("----------------------------------\n");
}

void test3() {
    printf("Chat_info : \n");
    CHAT_INFO * chat_info = malloc(sizeof(CHAT_INFO));
    char json[] = "{\"IP\":\"127.0.0.1\",\"ACTION\":11,\"account\":\"931305033\",\"time\":\"2018-01-01\"}";
    send_client_chat_info(chat_info,json);
    printf("IP : %s\n",chat_info->IP);
    printf("ACTION : %d\n",chat_info->ACTION);
    printf("account : %s\n",chat_info->account);
    printf("TIME : %s\n",chat_info->t);
    free(chat_info);
    printf("----------------------------------\n");
}


void test4() {
    printf("Add_INFO : \n");
    char json[] = "{\"ACTION\":1111,\"account\":\"931305033\"}";
    ADD_INFO *add_info;
    add_info = malloc(sizeof(ADD_INFO));
    send_add_chat_info(add_info,json);
    printf("%s\n",add_info->account);
    printf("%d\n",add_info->ACTION);
    free(add_info);
    printf("----------------------------------\n");
}

void re_test() {

    RE_INFO r;
    r.STATUS = 7;
    r.t = "2018-01-01";

    FRIEND aFriend[3];
    strcpy(aFriend[0].u_name, "eddie");
    strcpy(aFriend[1].u_name, "alice");
    strcpy(aFriend[2].u_name, "jim");

    strcpy(aFriend[0].IP, "127.0.0.1");
    strcpy(aFriend[1].IP, "127.0.0.1");
    strcpy(aFriend[2].IP, "127.0.0.1");

    char *pJson = re_friend_info(&aFriend, 3);
    printf("%s\n", pJson);

    printf("----------------------------------\n");

    char *pJ = re_status_info(&r);
    printf("%s\n", pJ);

    printf("----------------------------------\n");
}


