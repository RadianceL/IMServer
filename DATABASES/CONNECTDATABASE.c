//
// Created by Eddie Lee on 1/14/18.
//

#include "CONNECTDATABASE.h"

MYSQL *conn;

int connect_database_init(){
    MYSQL_RES * result;     //保存结果集的
    MYSQL_ROW row;          //代表的是结果集中的一行

    conn = mysql_init(NULL);
    if (conn == NULL) { // 如果返回NULl说明初始化失败
        printf("mysql_init failed!\n");
        return EXIT_FAILURE;
    }
    conn = mysql_real_connect(conn, "47.93.50.62", "root", "776543", NULL, 3306, NULL, 0);

    if (conn) {
        printf("Connection success!\n");

    } else {
        printf("Connection failed!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void init_databases(){
    mysql_query(conn,"CREATE DATABASE test");
    mysql_query(conn,"USE test");
    mysql_query(conn,"CREATE TABLE rt(id INT PRIMARY KEY AUTO_INCREMENT,name TEXT NOT NULL );");
}

//添加用户（用户名，密码，昵称，头像地址）
void add_user(){
    mysql_query(conn,"USE examples");
    mysql_query(conn,"insert into test(id,name)values(1,\"bb\");");
    printf("ffffffff\n");
}

//修改密码（用户名，原始密码，新密码）
void change_user_passwd(){

}

//查询用户信息（用户名，密码）
void search_user(){

}

//假如黑名单（用户名，时间）
void make_user_black(){

}

//删除用户（用户名，超级用户密码，原因，时间）
void drop_user(){

}

//退出系统，断开连接
void close_connection(){
    mysql_close(conn);
}