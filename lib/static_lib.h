//
// Created by Eddie Lee on 12/28/17.
//

#ifndef WEBTEST_STATIC_LIB_H
#define WEBTEST_STATIC_LIB_H

typedef struct{
    char *IP;
    int TPYE;
    char *account;
    char *passwd;
    char *t;
}LOGIN_INFO;

typedef struct {
    char u_name[16];
}FRIEND;

typedef struct{
    int STATUS;
    char *t;
}RE_INFO;

extern void send_client_login_info(LOGIN_INFO *login_info,char *json);
extern char* re_json_login_info(RE_INFO *re_info,FRIEND *aFriend , int len);

#endif