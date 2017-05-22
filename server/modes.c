#ifndef MODES
#define MODES
#include "modes.h"
#endif

void *all_start(void *arg)
{
	struct pthread *socket_kit = (struct pthread *)arg;

	printf("<log>client [%d] is connect\n", socket_kit->client_sock_fd);
	start(socket_kit);

	pthread_exit(NULL);
}

void start(struct pthread *socket_kit)
{
	int client_sock_fd = socket_kit->client_sock_fd;
	MYSQL *connection = socket_kit->connection;
	
	struct user *person;	//defined in user.h
	person = (struct user *)malloc(sizeof(struct user));
	
	char choice;
	
	if( recv_mode(client_sock_fd , &choice) == -1)
	{
		release_socket_kit(socket_kit);
		free(person);
		
		return ;
	}
	
printf("[<test>\\user mode : %c %d]\n" , choice , client_sock_fd);

	switch(choice)
	{
		case 'a':
			if( modes_user_in(client_sock_fd , person , connection) == -1)
			{
				printf("<log>client [%d] is closed[login]\n", client_sock_fd);
				break;	//千万要释放客户端占用的所有资源
			}
			else
			{
				//这里的流程最后检查 因为是最主要的地方
				//这里就是登陆成功了 下面开始了
				socket_kit->person = person;	//将已登录用户的信息引入
				
				if( handle_work(socket_kit) == -1)	//这里可以对返回值进行判断
					printf("<normal affair>client [%d] is closed[internet]\n", client_sock_fd);
				else
					printf("<normal affair>client [%d] is closed[quit_logout]\n", client_sock_fd);
				
				break;
			}			
		case 'b':
			if( modes_user_register(client_sock_fd , person , connection) == -1)
			{
				printf("<register>client [%d] is closed[register 1]\n", client_sock_fd);
				break;	//千万要释放客户端占用的所有资源
			}
			else
			{
				//用户注册完了之后 到底应该怎么样 后面添加
				;
				printf("<register>client [%d] is closed[register 2]\n", client_sock_fd);;	//这里表明注册成功了，接下来用户就可以进行登录了
				break;
			}
		case 'c':
			if( modes_user_in(client_sock_fd , person , connection) == -1)
			{
				printf("<log>client [%d] is closed[login]\n", client_sock_fd);
				break;
			}
			else
			{
				socket_kit->person = person;	//将已登录用户的信息引入
				
				if( handle_work(socket_kit) == -1)	//这里可以对返回值进行判断
					printf("<anonymous>client [%d] is closed[internet]\n", client_sock_fd);
				else
					printf("<anonymous>client [%d] is closed[quit_logout]\n", client_sock_fd);
				
				break;
			}
			
		default:
			//这里就是客户端意外关闭会执行到的地方	choice == 'z'
			//这里就是一个用户线程结束的地方，服务器释放一切分给客户端的资源
			printf("client close\n");
			break;
	}
	
	release_socket_kit(socket_kit);
	free(person);
	
	return ;
}

int modes_user_in(int client_sock_fd , struct user *one , MYSQL *connection)
{
	char check_result;
	
	char id[MAX_LENGTH_TWENTY];
	char password[MAX_LENGTH_TWENTY];
	
	if( recv_id_password(client_sock_fd , one ) == -1)
		return -1;

	check_result = check_user(one , connection , NULL);	//在数据库中校验用户登录输入的信息 和网络无关

	if( send_check_result(client_sock_fd , check_result) == -1)	//send the check result to the user
		return -1;
	
	if(check_result == 'y')
	{
		check_user(one , connection , (void *)one );	//从数据库中取出所有关于这个新登录用户的全部信息，返回给客户
		
		if( send_user_info(client_sock_fd , one ) == -1)	//send the user information with the style of struct
			return -1;
		else
			return 1;
	}
	else
		return -1;
}

int modes_user_register(int client_sock_fd , struct user *one , MYSQL *connection)
{
	char result;
	while(1)
	{
		if( check_new_user_email(client_sock_fd , one , connection , &result) == -1)
			 return -1;	
		 
		if(result == 'n')	//说明新用户注册的邮箱在数据库是不存在的，是可以进行注册的
		{		
			break ;
		}
	}
	if( recv_user_info(client_sock_fd , one) == -1)	//recive the new user's information , and insert into database
		return -1;
	
	add_new_user(one , connection);	//insert the new user's information into the database

	//in check.c
	get_user_id__reg_time(one , connection);
	
	if( send_user_id_reg_time(client_sock_fd , one) == -1)
		return -1;
	
	//在数据库总添加用户上传音乐的目录
	add_user_upload_dir(one , connection);
	//在数据库中创建用户上传音乐数据表
	create_upload_table(one , connection);
	
	//注册成功 发一封邮件
	char register_success[MAX_LENGTH_REGISTER_SUCCESS];	//80
	strcpy(register_success , "mail  -s \"register\" ");
	strcat(register_success , one->email);
	strcat(register_success , " < /home/leaf/graduation_design/server/register");
	system(register_success);	
	//注册成功 创建用户上传音乐的目录
	char mkdir_user_upload_dir[MAX_LENGTH_REGISTER_SUCCESS * 2];	//160
	strcpy(mkdir_user_upload_dir , "mkdir /home/leaf/graduation_design/upload/");
	strcat(mkdir_user_upload_dir , one->id);
	system(mkdir_user_upload_dir);

	return 1;
}

