//
// Created by Eddie Lee on 12/28/17.
//

#ifndef WEBTEST_STATIC_LIB_H
#define WEBTEST_STATIC_LIB_H

typedef struct {
    char *IP;
    int ACTION;
    char *account;
    char *passwd;
    char *t;
}LOGIN_INFO;

typedef struct {
    char *IP;
    int ACTION;
    char *account;
    char *t;
}LOGOUT_INFO;

typedef struct {
    char *IP;
    int ACTION;
    char *account;
    char *t;
}CHAT_INFO;

typedef struct {
    char *account;
    int ACTION;
}ADD_INFO;


typedef struct {
    char IP[32];
    char u_name[32];
}FRIEND;

typedef struct{
    int STATUS;
    char *t;
}RE_INFO;

//登陆信息
extern void send_client_login_info(LOGIN_INFO *login_info,char *json);
//登出信息
extern void send_client_logout_info(LOGOUT_INFO *logout_info,char *json);
//聊天连接信息
extern void send_client_chat_info(CHAT_INFO *chat_info,char *json);
//添加删除好友/群
extern ADD_INFO send_add_chat_info(ADD_INFO *chat_info,char *json);
//反回状态，是否成功
extern char* re_status_info(RE_INFO *re_info);
//反回朋友信息
extern char* re_friend_info(FRIEND *aFriend , int len);
//获取Action信息
void getAction(CHAT_INFO *info,char *json);

#endif