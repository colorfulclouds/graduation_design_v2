//USER

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

struct user
{
	char id[MAX_LENGTH_TWENTY];			//账号（不可修改）
	char password[MAX_LENGTH_TWENTY];	//密码
	char email[MAX_LENGTH_THIRTY];		//邮箱
	char name[MAX_LENGTH_TWENTY];		//昵称
	
	int age;	//年龄
	char sex;	//性别
	char reg_time[MAX_LENGTH_TWENTY];	//注册时间（不可修改）
};

struct pthread
{
	//与用户连接产生connect()返回值
	int client_sock_fd;		//普通套接字
	int download_sock_fd;	//下载音乐套接字
	int online_sock_fd;		//在线试听套接字
	int upload_sock_fd;		//上传音乐套接字
	
	MYSQL *connection;
	//上面4个经常使用的
	pthread_t pid;
	//服务器自身创建的套接字
	int server_sock_fd;
	int server_sock_fd_download;
	int server_sock_fd_online;
	int server_sock_fd_upload;
	
	//用户的信息
	struct user *person;
};

void release_socket_kit(struct pthread *socket_kit);