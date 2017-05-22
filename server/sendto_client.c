#ifndef SENDTO_CLIENT
#define SENDTO_CLIENT
#include "sendto_client.h"
#endif

int send_check_result(int client_sock_fd , char check_result)
{
	return _send(client_sock_fd , (void *)&check_result , -1 , 1);
}

int send_user_info(int client_sock_fd , struct user *one)
{
	if(_send(client_sock_fd , (void *)one->name , sizeof(one->name) , 3) == -1)
		return -1;
	if(_send(client_sock_fd , (void *)one->email , sizeof(one->email) , 3) == -1)
		return -1;
	if(_send(client_sock_fd , (void *)&(one->age) , -1 , 2) == -1)
		return -1;
	if(_send(client_sock_fd , (void *)&(one->sex) , -1 , 1) == -1)
		return -1;
	if(_send(client_sock_fd , (void *)one->reg_time , sizeof(one->reg_time) ,3) == -1)
		return -1;
	
	return 1;
}

int send_user_id_reg_time(int client_sock_fd , struct user *one)
{
	if( _send(client_sock_fd , (void *)one->id , sizeof(one->id) ,3) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)one->reg_time , sizeof(one->reg_time) , 3) == -1)
		return -1;
	
	return 1;
}

int send_music_id_name(int client_sock_fd , int music_id , char  *music_name)
{
	if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)music_name , MUSIC_NAME_MAX_LENGTH , 3) == -1)
		return -1;
	
	return 1;
}

//下载音乐主要数据传输
int send_music(struct pthread_number *affair_pthread , int download_sock_fd , int music_id , MYSQL *connection , int music_library_flag , char *user_id)
{
	char *music_path_name = (char *)malloc(sizeof(char) * (MUSIC_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH) );
	char *music_path = (char *)malloc(sizeof(char) * MUSIC_PATH_MAX_LENGTH);
	char *music_name = (char *)malloc(sizeof(char) * MUSIC_NAME_MAX_LENGTH);
	
	switch(music_library_flag)
	{
		case PLAYS_s_g:	//广库
			get_music_path_name(music_id , connection , music_path , music_name);
			break;
			
		case PLAYS_s_p:	//自己上传库
			get_music_path_name_upload(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , music_id , music_path , music_name);
			break;
		case PLAYS_s_f:	//好友上传库
			get_music_path_name_upload(affair_pthread->socket_kit->connection , user_id , music_id , music_path , music_name);
			break;
		
		//后续可追加多种下载任务
		default:
			//不会执行到这里
			break;
	}
	
	strcpy(music_path_name , music_path);
	strcat(music_path_name , music_name);
	
	int music_fd;
	music_fd = open(music_path_name , O_RDONLY);
	if(music_fd < 0)
	{
		printf("this music is not exist\n");
		free(music_path_name);
		free(music_path);
		free(music_name);
		return -2;	//2表示这个音乐文件不存在 可以考虑通知用户 
	}
	
	//*********下面获取文件大小
	int music_size;
	struct stat music_stat;
	stat(music_path_name , &music_stat);
	music_size = music_stat.st_size;
	if( _send(download_sock_fd , (void *)&music_size , -1 , 2) == -1)
	{
		free(music_path_name);
		free(music_path);
		free(music_name);
		close(music_fd);
		return -1;
	}
	//**************************
	void *buff = malloc(MAX_SEND_MUSIC_BUFF_LENGTH);
	int result;
	int finish_flag;

	while(1)
	{
		result = read(music_fd , buff , MAX_SEND_MUSIC_BUFF_LENGTH);
		usleep(1000);	//缓和用户接受压力
		
		if(result <= 0)
			break;
		
		if( send_music_block(download_sock_fd , buff , result) == -1)
		{
			free(music_path_name);
			free(music_path);
			free(music_name);
			free(buff);
			close(music_fd);
			return -1;	//-1表示网络突然不通
		}
	}
	
	free(music_path_name);
	free(music_path);
	free(music_name);
	free(buff);
	close(music_fd);

puts("finish music");	
	while(1)
	{
		if( _recv(download_sock_fd , (void *)&finish_flag , -1 , 2) == -1)
			return -1;
		if(finish_flag == 1)
			break;
	}
	
	return 1;
}

//下载音乐线程（主控）
void *buff_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]sset cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]sset cancel type error\n");
	
	struct pthread_number *affair_pthread = (struct pthread_number *)arg;
	int download_sock_fd = affair_pthread->socket_kit->download_sock_fd;

	int music_id;
	int music_library_flag;
	
	char user_id[MAX_USER_ID_LENGTH];	//6
	bzero(user_id , sizeof(user_id));
	
	while(1)
	{
		if( _recv(download_sock_fd , (void *)&music_id , -1 , 2) == -1)
			break;
		
		if( _recv(download_sock_fd , (void *)&music_library_flag , -1 , 2) == -1)
			break;
		
		switch(music_library_flag)
		{
			case PLAYS_s_g:	//广库
				increase_download_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , music_id);	//增加下载次数
				break;
				
			case PLAYS_s_p:	//自己上传库
				;
				break;
				
			case PLAYS_s_f:	//好友上传库
				if( _recv(download_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
				{
					goto err;	//必须退出整个while循环
				}
				break;
		}
		printf("%d downloading music:%d\n", affair_pthread->socket_kit->client_sock_fd , music_id);
		
		if( send_music(affair_pthread , download_sock_fd , music_id , affair_pthread->socket_kit->connection , music_library_flag , user_id) == -1)
			break;
	}
err:
	pthread_exit(NULL);
}

//试听传送缓冲数据
int trans_music_stream(int online_sock_fd , int music_id , MYSQL *connection , struct pthread_number *affair_pthread)
{
	char *music_path_name = (char *)malloc(sizeof(char) * (MUSIC_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH) );
	char *music_path = (char *)malloc(sizeof(char) * MUSIC_PATH_MAX_LENGTH);
	char *music_name = (char *)malloc(sizeof(char) * MUSIC_NAME_MAX_LENGTH);
	
	//应该保证music_id 是正常的 不会出现非正值 bug还是会出现 干脆判断一下
	if( get_music_path_name(music_id , connection , music_path , music_name) == -1)
	{
		free(music_path_name);
		free(music_path);
		free(music_name);
		return -3;
	}
	
	strcpy(music_path_name , music_path);
	strcat(music_path_name , music_name);
	
	int music_fd;
	music_fd = open(music_path_name , O_RDONLY);
	if(music_fd < 0)
	{
		printf("this music is not exist\n");
		free(music_path_name);
		free(music_path);
		free(music_name);
		return -2;
	}

	//*********下面获取文件大小
	int music_size;
	struct stat music_stat;
	stat(music_path_name , &music_stat);
	music_size = music_stat.st_size;
	if( _send(online_sock_fd , (void *)&music_size , -1 , 2) == -1)
	{
		free(music_path_name);
		free(music_path);
		free(music_name);
		close(music_fd);
		return -1;
	}
printf("size:%d\n", music_size);
	//**************************
	void *buff = malloc(MAX_SEND_MUSIC_BUFF_LENGTH);
	int result;
	int finish_flag;
	
	while(1)
	{
		result = read(music_fd , buff , MAX_SEND_MUSIC_BUFF_LENGTH);

		if(result <= 0)
			break;
		
		if( send_music_block(online_sock_fd , buff , result) == -1)
		{
			free(music_path_name);
			free(music_path);
			free(music_name);
			free(buff);
			close(music_fd);
			return -1;
		}
	}
	
	free(music_path_name);
	free(music_path);
	free(music_name);
	free(buff);
	close(music_fd);

puts("finish online music");	
	while(1)
	{
		if( _recv(online_sock_fd , (void *)&finish_flag , -1 , 2) == -1)
			return -1;
		if(finish_flag == 1)
			break;
	}
	
	return 1;
}

//试听音乐线程（主控）
void *online_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]sset cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]sset cancel type error\n");
	
	struct pthread_number *affair_pthread = (struct pthread_number *)arg;
	int music_id;
	int online_sock_fd = affair_pthread->socket_kit->online_sock_fd;
	
	if( _recv(online_sock_fd , (void *)&music_id , -1 , 2) == -1)
		pthread_exit(NULL);
	//****
	//获取音乐的类型（广库）
	int music_style;
	music_style = get_music_style(affair_pthread->socket_kit->connection , music_id);
	//****
	increase_style_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , music_style);//增加用户听某种音乐的次数
	increase_listen_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id ,music_id);	//音乐和用户的listen_count加1
	printf("%d online music:%d\n", affair_pthread->socket_kit->client_sock_fd , music_id);

	trans_music_stream(online_sock_fd , music_id , affair_pthread->socket_kit->connection , affair_pthread);
	
	pthread_exit(NULL);
}

