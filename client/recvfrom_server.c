#ifndef RECVFROM_SERVER
#define RECVFROM_SERVER
#include "recvfrom_server.h"
#endif

int recv_check_result(int client_sock_fd , char *check_result)
{
	return _recv(client_sock_fd , check_result , -1 , 1);
}

int recv_user_info(int client_sock_fd , struct user *one)
{
	if(_recv(client_sock_fd , (void *)one->name , sizeof(one->name) , 3) == -1)
		return -1;
	if(_recv(client_sock_fd , (void *)one->email , sizeof(one->email) , 3) == -1)
		return -1;
	if(_recv(client_sock_fd , (void *)&(one->age) , -1 , 2) == -1)
		return -1;
	if(_recv(client_sock_fd , (void *)&(one->sex) , -1 , 1) == -1)
		return -1;
	if(_recv(client_sock_fd , (void *)one->reg_time , sizeof(one->reg_time) , 3) == -1)
		return -1;
	
	return 1;
}

int recv_check_email_result(int client_sock_fd , struct user *one , char *result)
{
	if(_send(client_sock_fd , (void *)one->email , sizeof(one->email) , 3) == -1)
		return -1;
	
	if( _recv(client_sock_fd , result , -1 , 1) == -1)
		return -1;
}

int recv_top_music(int client_sock_fd)
{
	int i;
	
	int music_id;
	char music_name[MUSIC_NAME_MAX_LENGTH];	//60
	
	printf("******TOP[%d] on server*******\n", TOP_NUMBER);
	printf("*******id************name*****\n");
	for(i = 0 ; i < TOP_NUMBER ; i ++)
	{
		usleep(1000);
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27010]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[27011]check your net and re-login\n");
			return -1;
		}
		printf("\t%d\t%s\n", music_id , music_name);
	}
	printf("******************************\n");
	
	return 1;
}

//下载传输音乐（真是音乐数据传输）
int recv_music_buff_from_s(int download_sock_fd , struct download_queue *queue_download , int pos)
{
	queue_download->music_download[pos].download_flag = 'd';	//置 正在下载 标志

	char music_download_path_name[MUSIC_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH];
	strcpy(music_download_path_name , queue_download->music_download[pos].music_dir);
	strcat(music_download_path_name , queue_download->music_download[pos].music_name);

	//*************接收服务器传来的文件大小
	int music_size;
	if( _recv(download_sock_fd , (void *)&music_size , -1 , 2) == -1)
	{
		return -1;
	}
	//*************************************
	
	int finish_flag = 1;	//缓冲结束标志 1时 表示此音乐缓冲完毕
	int music_buff_fd = open(music_download_path_name , O_RDWR | O_CREAT | O_TRUNC);

	if(music_buff_fd <= 0)
	{
		perror("file");
		
//******
//******
//******
//******
//******
//下载可能会在这里出问题
//******
//******
//向服务器报告
//******
	}

	int result;
	void *buff = malloc(MAX_SEND_MUSIC_BUFF_LENGTH);
	
	int recv_count = 0;
	//recv_music_buff in send_recv.h
	while(1)
	{
		result = recv(download_sock_fd , buff , MAX_SEND_MUSIC_BUFF_LENGTH , 0);
		write(music_buff_fd , buff , result);
		
		recv_count += result;
		if(recv_count >= music_size)
			break;	//传输完毕
	}
	free(buff);
	close(music_buff_fd);
	if( _send(download_sock_fd , (void *)&finish_flag , -1 , 2) == -1)
		return -1;
	
	return 1;
}

//下载音乐
void *get_buff_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]sset cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]sset cancel type error\n");
	
	int music_library_flag;	//记录下载音乐的库标志
	
	struct music_shell *order_struct = (struct music_shell *)arg;
	struct download_queue *queue_download = order_struct->queue_download;
	int download_sock_fd = order_struct->do_socket->download_sock_fd;	//下载套接字
	
	int i;
	
	for(i = queue_download->download_pointer ; i < queue_download->rear ; i ++)
	{
		if( _send(download_sock_fd , &(queue_download->music_download[i].music_id) , -1 ,2 ) == -1)
			break;
		//****
		//发送音乐来源音乐库
		if( strcmp(queue_download->music_download[i].music_dir , "/home/download/") == 0)
		{
			//广库音乐
			music_library_flag = PLAYS_s_g;
		}
		else if(strcmp(queue_download->music_download[i].music_dir , "/home/self_download/") == 0)
			 {
				//用户上传库音乐
				music_library_flag = PLAYS_s_p;
			 }
			 else if( strcmp(queue_download->music_download[i].music_dir , "/home/other_download/") == 0 )
				  {
						//好友上传库音乐
						music_library_flag = PLAYS_s_f;
			      }
			 
		if( _send(download_sock_fd , (void *)&music_library_flag , -1 , 2) == -1)
			break;
		//如果从好友库上传库中下载 需要将好友id告诉服务器
		if( music_library_flag == PLAYS_s_f)
		{
			char user_id[MAX_USER_ID_LENGTH];	//6
			bzero(user_id , sizeof(user_id));
			strcpy(user_id , queue_download->music_download[i].user_id);
			if( _send(download_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
				break;
		}
		//****
		if( recv_music_buff_from_s(download_sock_fd , queue_download , i) == -1)
			break;
		//下载完了 从队列中移除
		delete_download_queue(queue_download);
	}
	
	pthread_exit(NULL);
}

//试听模块
void *recv_cache_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]sset cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]sset cancel type error\n");
	
	struct music_shell *order_struct = (struct music_shell *)arg;
	int online_sock_fd = order_struct->do_socket->online_sock_fd;
	
	//*************接收服务器传来的文件大小
	int music_size;
	if( _recv(online_sock_fd , (void *)&music_size , -1 , 2) == -1)
	{
		pthread_exit(NULL);
	}
	//*************************************
	
	int finish_flag = 1;
	int music_buff_fd = open("/tmp/cache.temp" , O_RDWR | O_CREAT | O_TRUNC);
	int result;
	void *buff = malloc(MAX_SEND_MUSIC_BUFF_LENGTH);
	
	int recv_count = 0;
	while(1)
	{
		result = recv(online_sock_fd , buff , MAX_SEND_MUSIC_BUFF_LENGTH , 0);
		write(music_buff_fd , buff , result);
		
		recv_count += result;
		if(recv_count >= music_size)
			break;	//传输完毕
	}
	free(buff);
	close(music_buff_fd);
	if( _send(online_sock_fd , (void *)&finish_flag , -1 , 2) == -1)
	{
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

int recv_style_music(int client_sock_fd , int music_style)
{
	int i;
	
	int music_style_count;
	int music_id;
	char music_name[MUSIC_NAME_MAX_LENGTH];				//60
	char music_style_str[MUSIC_STYLE_NAME_MAX_LENGTH];	//10
	
	if( _send(client_sock_fd , (void *)&music_style , -1 , 2) == -1)
	{
		printf("[27005]send music_style error\n");
		return -1;
	}
	
	if( _recv(client_sock_fd , (void *)&music_style_count , -1 , 2) == -1)
	{
		printf("[27020]check your net and re-login\n");
		return -1;
	}
	
	switch(music_style)
	{
		case PLAYS_k_pop:
			strcpy(music_style_str , "pop");
			break;
			
		case PLAYS_k_rock:
			strcpy(music_style_str , "rock");
			break;
			
		case PLAYS_k_emo:
			strcpy(music_style_str , "emotion");
			break;
			
		case PLAYS_k_girl:
			strcpy(music_style_str , "girl_s");
			break;
			
		case PLAYS_k_boy:
			strcpy(music_style_str , "boy_s");
			break;
	}
	
	printf("there are(is) [%d] [%s] music(s) on server\n", music_style_count , music_style_str);
	printf("*******id************name*****\n");
	
	for(i = 0 ; i < music_style_count ; i ++)
	{
		usleep(1000);
		
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27021]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[27022]check your net and re-login\n");
			return -1;
		}
		
		printf("\t%d\t%s\n", music_id , music_name);
	}
	
	printf("******************************\n");
	
	return 1;
}
