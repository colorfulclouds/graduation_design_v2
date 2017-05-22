//CHECK
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef DATABASE
#define DATABASE
#include "database.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef SENDTO_CLIENT
#define SENDTO_CLIENT
#include "sendto_client.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef SEND_RECV
#define SEND_RECV
#include "send_recv.h"
#endif

char check_user(struct user *one , MYSQL *connection , void *_one);

void add_new_user(struct user *one , MYSQL *connection);

int check_new_user_email(int client_sock_fd , struct user *one , MYSQL *connection , char *result);

void get_user_id__reg_time(struct user *one , MYSQL *connection);

void add_user_upload_dir(struct user *one , MYSQL *connection);	//向数据库中添加用户上传音乐目录 供后面使用

void create_upload_table(struct user *one , MYSQL *connection);	//在数据库中添加用户上传音乐的数据表 为好友间功能做铺垫

