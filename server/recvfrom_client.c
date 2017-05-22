#ifndef RECVFROM_CLIENT
#define RECVFROM_CLIENT
#include "recvfrom_client.h"
#endif

int recv_mode(int client_sock_fd , char *choice)
{
	return _recv(client_sock_fd , (void *)choice , -1 , 1);
}

int recv_id_password(int client_sock_fd , struct user *one)
{
	if( _recv(client_sock_fd , (void *)one->id , sizeof(one->id) , 3) == -1)
		return -1;
	if( _recv(client_sock_fd , (void *)one->password , sizeof(one->password) , 3) == -1)
		return -1;
	
	return 1;
}

int recv_user_info(int client_sock_fd , struct user *one)
{
	if( _recv(client_sock_fd , (void *)one->name  , sizeof(one->name) , 3) == -1)
		return -1;
	if( _recv(client_sock_fd , (void *)one->email , sizeof(one->email) , 3) == -1)
		return -1;
	if( _recv(client_sock_fd , (void *)one->password , sizeof(one->password) , 3) == -1)
		return -1;
	if( _recv(client_sock_fd , (void *)&(one->age) , -1 , 2) == -1)
		return -1;
	if( _recv(client_sock_fd , (void *)&(one->sex) , -1 , 1) == -1)
		return -1;
	
	return 1;
}

int recv_user_request(int client_sock_fd , int *user_request)
{
	return _recv(client_sock_fd , (void *)user_request , -1 , 2);
}

//真实接收上传音乐数据
int recv_upload_music_true(int upload_sock_fd , char *user_upload_dir , char *music_name)
{
	strcat(user_upload_dir , music_name);
	
	int music_size;
	if( _recv(upload_sock_fd , (void *)&music_size , -1 , 2) == -1)
		return -1;
	
	int music_fd;
	music_fd = open(user_upload_dir , O_RDWR | O_CREAT | O_TRUNC);
	if(music_fd < 0)
	{
		perror("gg");
		return -2;	//文件打开出错
	}
	
	int finish_flag = 1;
	int result;
	void *buff = malloc(MAX_SEND_MUSIC_BUFF_LENGTH);
	
	int recv_count;
	
	while(1)
	{
		result = recv(upload_sock_fd , buff , MAX_SEND_MUSIC_BUFF_LENGTH , 0);
		write(music_fd , buff , result);
		
		recv_count += result;
		if(recv_count >= music_size)
			break;
	}
	free(buff);
	close(music_fd);
	
	if( _send(upload_sock_fd , (void *)&finish_flag , -1 , 2) == -1)
		return -1;

	puts("one music uploaded");
	return 1;
}

//接收上传音乐
void *recv_upload_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]sset cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]sset cancel type error\n");

	struct pthread_number *affair_pthread = (struct pthread_number *)arg;

	int upload_sock_fd = affair_pthread->socket_kit->upload_sock_fd;
	
	char user_upload_dir[MAX_USER_UPLOAD_DIR_LENGTH];
	get_user_upload_dir(affair_pthread , user_upload_dir);	//从数据库获取该登录用户的上传目录
	int length = strlen(user_upload_dir);
	
	char music_name[MUSIC_NAME_MAX_LENGTH];
	
	while(1)
	{
		user_upload_dir[length] = '\0';	//连续上传文件时候 保持路径不变
		//接收上传的音乐的名字
		if( _recv(upload_sock_fd , (void *)music_name , MUSIC_NAME_MAX_LENGTH , 3) == -1)
			break;

		if( recv_upload_music_true(upload_sock_fd , user_upload_dir , music_name) == -1)
			break;
		
		//上传音乐 在数据库中进行记录
		add_user_upload(affair_pthread , music_name);
	}
	
	pthread_exit(NULL);
}