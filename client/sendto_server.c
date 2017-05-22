#ifndef SENDTO_SERVER
#define SENDTO_SERVER
#include "sendto_server.h"
#endif

int send_order(int client_sock_fd , char choice)
{
	return _send(client_sock_fd , (void *)&choice , -1 ,  1);
}

int send_id_password(int client_sock_fd , struct user *one)
{
	if( _send(client_sock_fd , (void *)one->id , sizeof(one->id) , 3) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)one->password , sizeof(one->password) , 3) == -1)
		return -1;
	
	return 1;
}

int send_user_info(int client_sock_fd , struct user *one)
{
	if( _send(client_sock_fd , (void *)one->name , sizeof(one->name) , 3) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)one->email , sizeof(one->email) , 3) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)one->password , sizeof(one->password) , 3) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)&(one->age) , -1 , 2) == -1)
		return -1;
	if( _send(client_sock_fd , (void *)&(one->sex) , -1 , 1) == -1)
		return -1;
	
	return 1;
}

int send_user_request(int client_sock_fd , int user_request)
{
	return _send(client_sock_fd , (void *)&user_request , -1 , 2);
}

//上传音乐（真实传送音乐数据）
int send_upload_music_to_s(int upload_sock_fd , struct upload_queue *queue_upload , int pos)
{
	queue_upload->music_upload[pos].upload_flag = 'u';
	
	int music_fd;
	
	music_fd = open(queue_upload->music_upload[pos].music_dir_name , O_RDONLY);
	if(music_fd < 0)
	{
		printf("this music is not exist\n");
		queue_upload->music_upload[pos].upload_flag = 'x';	//置音乐标志位x 表示音乐源文件出现问题（例如 不存在）
		return -2;	//这个音乐不存在 继续上传余下的音乐文件
	}
	int music_size;
	struct stat music_stat;
	stat(queue_upload->music_upload[pos].music_dir_name , &music_stat);
	music_size = music_stat.st_size;
	
	//发送音乐名字
	if( _send(upload_sock_fd , (void *)(queue_upload->music_upload[pos].music_name) , sizeof(queue_upload->music_upload[pos].music_name) , 3) == -1)
	{
		close(music_fd);
		return -1;
	}
	//发送音乐文件大小
	if( _send(upload_sock_fd , (void *)&music_size , -1 , 2) == -1)
	{
		close(music_fd);
		return -1;
	}
	
	void *buff = malloc(MAX_SEND_MUSIC_BUFF_LENGTH);	//申请发送音乐文件缓冲区
	int result;
	int finish_flag;
	
	while(1)
	{
		result = read(music_fd , buff , MAX_SEND_MUSIC_BUFF_LENGTH);
		usleep(1000);
		
		if(result <= 0)
			break;	//文件发送完毕
		
		if( send_music_block(upload_sock_fd , buff , result ) == -1)
		{
			free(buff);
			close(music_fd);
			return -1;
		}
	}
	
	free(buff);
	close(music_fd);
	
	while(1)
	{
		if( _recv(upload_sock_fd , (void *)&finish_flag , -1 , 2) == -1)
			return -1;
		if( finish_flag == 1)
			break;
	}
//******************
//丢失置queue_upload->music_upload[pos].upload_flag的语句  到时补上
//******************
//******************
//******************
	return 1;
}

//上传音乐（主控）
void *send_upload_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]sset cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]sset cancel type error\n");
	
	struct music_shell *order_struct = (struct music_shell *)arg;
	struct upload_queue *queue_upload = order_struct->queue_upload;
	int upload_sock_fd = order_struct->do_socket->upload_sock_fd;
	
	int i;
	
	for(i = queue_upload->upload_pointer ; i < queue_upload->rear ; i ++)
	{
		if( send_upload_music_to_s(upload_sock_fd , queue_upload , i) == -1 )
			break;
		
		delete_upload_queue(queue_upload);
	}
	
	pthread_exit(NULL);
}