#include <static_lib.h>
#include "FUNCTION/Connection.h"

void test1();

void re_test();

void test2();

int main() {
    test2();

    return EXIT_SUCCESS;
}


void test1() {
    char json[] = "{\"ACTION\":100,\"account\":\"931305033\"}";
    ADD_INFO *add_info;
    add_info = malloc(sizeof(ADD_INFO));
    send_add_chat_info(add_info,json);
    printf("%s\n",add_info->account);
    printf("%d\n",add_info->ACTION);
    free(add_info);
}

void test2() {
    LOGIN_INFO *login_info = malloc(sizeof(LOGIN_INFO));
    char json[] = "{\"IP\":\"127.0.0.1\",\"ACTION\":0,\"account\":\"931305033\",\"password\":\"776543\",\"time\":\"2018-01-01\"}";
    send_client_login_info(login_info,json);

    printf("%s\n",login_info->IP);
    printf("%d\n",login_info->ACTION);
    printf("%s\n",login_info->account);
    printf("%s\n",login_info->passwd);
    printf("%s\n",login_info->t);
    free(login_info);
}

void re_test() {

    RE_INFO r;
    r.STATUS = 7;
    r.t = "fff";

    FRIEND aFriend[3];
    strcpy(aFriend[0].u_name, "eddie");
    strcpy(aFriend[1].u_name, "alice");
    strcpy(aFriend[2].u_name, "jim");

    strcpy(aFriend[0].IP, "127.0.0.1");
    strcpy(aFriend[1].IP, "127.0.0.1");
    strcpy(aFriend[2].IP, "127.0.0.1");

    char *pJson = re_friend_info(&aFriend, 3);
    printf("JSON:\n%s\n", pJson);

    char *pJ = re_status_info(&r);
    printf("JSON:\n%s\n", pJ);


}