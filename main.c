#include <static_lib.h>
#include "FUNCTION/Connection.h"
#include "FUNCTION/InitConnection.h"


void re_test(){
    RE_INFO re_info;
    FRIEND aFriend[3];
    strcpy(aFriend[0].u_name,"eddie");
    strcpy(aFriend[1].u_name,"alice");
    strcpy(aFriend[2].u_name,"jim");

    re_info.STATUS = 1;
    re_info.t = "2017-01-01";
    char * pJson = re_json_login_info(&re_info,&aFriend,3);
    printf("JSON:\n%s\n", pJson);
}

int main() {

    char json[] = "{\"IP\":\"127.0.0.1\",\"TYPE\":2,\"account\":\"931305033\",\"password\":\"776543a\",\"time\":\"2017-01-01\"}";
    LOGIN_INFO login_info;
    send_client_login_info(&login_info,json);

    printf("%s\n",login_info.IP);
    printf("%s\n",login_info.account);
    printf("%s\n",login_info.passwd);
    printf("%s\n",login_info.t);
    printf("%d\n",login_info.TPYE);

    re_test();

    return EXIT_SUCCESS;
}