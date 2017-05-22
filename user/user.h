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
	char id[MAX_LENGTH_TWENTY];			//账号
	char password[MAX_LENGTH_TWENTY];	//密码
	char name[MAX_LENGTH_TWENTY];		//昵称
	char email[MAX_LENGTH_THIRTY];		//邮箱
	
	int age;	//年龄
	char sex;	//性别
	char reg_time[MAX_LENGTH_TWENTY];	//注册时间
};

struct socket_t
{
	struct sockaddr_in client_info;
	int client_sock_fd;		//常规套接字
	
	struct sockaddr_in download_info;
	int download_sock_fd;	//下载音乐套接字
	
	struct sockaddr_in online_info;
	int online_sock_fd;		//在线试听套接字
	
	struct sockaddr_in upload_info;
	int upload_sock_fd;		//上传套接字
	
	struct user *person;	//挂入登录用户信息
};

void release_socket_t(struct socket_t *do_socket);	//释放套接字资源