#ifndef PLAYS_GO
#define PLAYS_GO
#include "plays_go.h"
#endif

int plays_shell(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	if( order_struct->order_para == PLAYS_t)	//4
		return plays_shell_t(order_pthread , order_struct , user_music);
	
	if( order_struct->order_para == PLAYS_s)	//8
		return plays_shell_s(order_pthread , order_struct , user_music);
	
	if( order_struct->order_para == PLAYS_o)	//16
		return plays_shell_o(order_pthread , order_struct , user_music);
	
	if( order_struct->order_para == PLAYS_d)	//32
		return plays_shell_d(order_struct->queue_download);
	
	if( order_struct->order_para == PLAYS_u)	//64
		return plays_shell_u(order_pthread , order_struct);
	
	if( order_struct->order_para == PLAYS_k)	//128
		return plays_shell_k(order_pthread , order_struct);
	
	if( order_struct->order_para == PLAYS_l)	//256
		return plays_shell_l(order_pthread , order_struct);
		
	if( order_struct->order_para == PLAYS_c)	//4096
		return plays_shell_c(order_pthread , order_struct);
	
	if( order_struct->order_para == PLAYS_f)	//1024
		return plays_shell_f(order_pthread , order_struct);
		
	if( order_struct->order_para == PLAYS_r)	//2048
		return plays_shell_r(order_pthread , order_struct);
		
	if( order_struct->order_para == PLAYS_m)	//8192
		return plays_shell_m(order_pthread , order_struct);
		
	if( order_struct->order_para == PLAYS_i)	//16384
		return plays_shell_i(order_pthread , order_struct);
		
	if( order_struct->order_para == (PLAYS_u | PLAYS_d) )	//96
		return plays_shell_u_d(order_struct->queue_upload);
	
	if( order_struct->order_para == (PLAYS_l | PLAYS_d) )	//288
		return plays_shell_l_d(order_pthread , order_struct);
	
	if( order_struct->order_para == (PLAYS_l | PLAYS_x) )	//768
		return plays_shell_l_x(order_pthread , order_struct);
	
	if( order_struct->order_para == (PLAYS_l | PLAYS_u) )	//320
		return plays_shell_l_u(order_pthread , order_struct);
		
	if( order_struct->order_para == (PLAYS_u | PLAYS_x) )	//576
		return plays_shell_u_x(order_pthread , order_struct);
	
	if( order_struct->order_para == (PLAYS_u | PLAYS_s) )	//72
		return plays_shell_u_s(order_pthread , order_struct);
		
	if( order_struct->order_para == (PLAYS_c | PLAYS_l) )	//4352
		return plays_shell_c_l(order_pthread , order_struct);

	return 1;	//参数不存在 相关错误

	//if......
}

//TOPxx
int plays_shell_t(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int i;

printf("para:%d\n", order_struct->order_para);	
	if( send_user_request(order_struct->do_socket->client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[27001]check your net and re-login\n");
		return -1;
	}
	
	if( recv_top_music(order_struct->do_socket->client_sock_fd) == -1)
	{
		printf("[27002]recv topxx error\n");
		return -1;
	}
	
	return PLAYS_t;
}

//音乐缓冲(下载)
int plays_shell_s(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	
	int music_id_valid;
	char music_name[MUSIC_NAME_MAX_LENGTH];
	bzero(music_name , sizeof(music_name));
	
	char *music_id_c;
	int music_id = 0;

	if(order_struct->file_count == 0)
	{
		//在这里发送请求才是合理的 不占用server资源
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[27001]check your net and re-login\n");
			return -1;
		}
		//发送music_id = 0
		if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}
		//接收服务器传回的随机music_id
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}
	}
	else
	{
		music_id_c = strtok( order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		if(music_id == 0)
		{
			printf("check your para\n");
			return PLAYS_s;
		}
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[27001]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}
		if(music_id_valid == -1)
		{
			//用户输入的music_id不合法
			printf("wrong para\n");
			return PLAYS_s;
		}
		else
		{
			//用户输入的music_id合法
			if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			{
				printf("[27002]check your net and re-login\n");
				return -1;
			}
		}
	}
	//插入下载队列中 等待被下载
	insert_download_queue(order_struct->queue_download , music_id , music_name , "/home/download/" , NULL);	//下载目录 用户的家目录中

	int pthread_test;
	
	if(order_pthread->download_thread == 0)
		pthread_create( &(order_pthread->download_thread) , NULL , get_buff_music , (void *)(order_struct) );
	else
	{
		pthread_test = pthread_kill(order_pthread->download_thread , 0);
		if(pthread_test == ESRCH)
			pthread_create( &(order_pthread->download_thread) , NULL , get_buff_music , (void *)(order_struct) );
		else
			;
	}

	return PLAYS_s;
}

//在线试听（伪试听）
int plays_shell_o(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	char *music_id_c;
	int music_id;
	int music_id_valid;
	char music_name[MUSIC_NAME_MAX_LENGTH];
	bzero(music_name , sizeof(music_name));
	
	if(order_struct->file_count == 0)
	{
		printf("lack of para\n");
		return PLAYS_o;
	}
	else
	{
		music_id_c = strtok( order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		if(music_id == 0)
		{
			printf("check your para\n");
			return PLAYS_o;
		}
		//在这里向服务器发送是正确的 不占用服务器资源
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[27001]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
		{
			printf("[27002]check your net and re-login\n");
			return -1;
		}	

		if( music_id_valid == -1)
		{
			printf("invalid para\n");
			return PLAYS_s;
		}
		else
		{		
			if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			{
				printf("[27002]check your net and re-login\n");
				return -1;
			}
			//只是插入 若要播放 使用play命令即可
			insert_play_queue_o(order_struct->play_queue , music_name , music_id);
		}
	}
	return PLAYS_s;
}

//查看下载队列
int plays_shell_d(struct download_queue *queue )
{
	display_download_queue(queue);
	return PLAYS_d;
}

//上传音乐
int plays_shell_u(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	char *music_path_c;
	//先插入上传队列
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_u;
	}
	else
	{
		music_path_c = strtok(order_struct->file_name , "#");	//这句多余了 #会自动删除
		insert_upload_queue(order_struct->queue_upload , music_path_c);
	}
	
	//再从上传队列进行上传
	int pthread_test;
	
	if(order_pthread->updownload_thread == 0)
		pthread_create( &(order_pthread->updownload_thread) , NULL , send_upload_music , (void *)order_struct );
	else
	{
		pthread_test = pthread_kill(order_pthread->updownload_thread , 0);
		if(pthread_test == ESRCH)
			pthread_create( &(order_pthread->updownload_thread) , NULL , send_upload_music , (void *)order_struct );
		else
			;
	}
	
	return PLAYS_u;
}

//查看上传队列
int plays_shell_u_d(struct upload_queue *queue)
{
	display_upload_queue(queue);
	
	return PLAYS_u | PLAYS_d;
}

//查看指定music_style的音乐
int plays_shell_k(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	
	char *music_style_c;
	int music_style;
	
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_k;
	}
	else
	{
		music_style_c = strtok(order_struct->file_name , "#");
		music_style = atoi(music_style_c);
		if( (music_style != PLAYS_k_pop) && (music_style != PLAYS_k_rock) && (music_style != PLAYS_k_emo) && (music_style != PLAYS_k_girl) && (music_style != PLAYS_k_boy) )
		{
			printf("invalid music style number\n");
			return PLAYS_k;
		}
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[27003]check your net and re-login\n");
			return -1;
		}
		//下面的这个函数 将music_style发至服务器 并接受音乐id和name
		if( recv_style_music(order_struct->do_socket->client_sock_fd , music_style) == -1)
		{
			printf("[27004]recv style error\n");
			return -1;
		}
	}
}

//收藏音乐
int plays_shell_l(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	
	char *music_id_c;
	int music_id;
	int music_id_valid;
	
	if( order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_l;
	}
	else
	{
		music_id_c = strtok(order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		if(music_id <= 0)
		{
			printf("invalid para\n");
		}
		else
		{
			if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
			{
				printf("[27006]check your net and re-login\n");
				return -1;
			}
			if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
			{
				printf("[27007]check your net and re-login\n");
				return -1;
			}
			if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
			{
				printf("[27008]check your net and re-login\n");
				return -1;
			}
			if( music_id_valid == -1)	//这个music_id是无效的 不存在的
			{
				printf("%d:invalid para\n", music_id);
			}
			else
				;
			
		}
		while(1)
		{
			music_id_c = strtok(NULL , "#");
			if(music_id_c == NULL)
				break;
			
			music_id = atoi(music_id_c);
			if(music_id <= 0)
				continue ;
			else
			{
				if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
				{
					printf("[27006]check your net and re-login\n");
					return -1;
				}
				if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[27007]check your net and re-login\n");
					return -1;
				}
				if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
				{
					printf("[27008]check your net and re-login\n");
					return -1;
				}
				if( music_id_valid == -1)	//这个music_id是无效的 不存在的
				{
					printf("%d:invalid para\n", music_id);
				}
				else
					;
			}
		}
	}
	
	return PLAYS_l;
}

//查看收藏的音乐
int plays_shell_l_d(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	int i;
	int collect_count;
	
	if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[27007]check your net and re-login\n");
		return -1;
	}
	
	usleep(100);	//服务器计算收藏数目
	if( _recv(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
	{
		printf("[27031]check your net and re-login\n");
		return -1;
	}

	if(collect_count == 0)
	{
		printf("no collect\n");
		return PLAYS_l | PLAYS_d;
	}
	
	int music_id;
	char music_name[MUSIC_NAME_MAX_LENGTH];
	
	printf("*********collect music********\n");
	printf("*******id************name*****\n");
	for(i = 0 ; i < collect_count ; i ++)
	{
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27032]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[27033]check your net and re-login\n");
			return -1;
		}
		printf("\t%d\t%s\n", music_id , music_name);
	}

	printf("******************************\n");
	return PLAYS_l | PLAYS_d;
}

//删除收藏的部分音乐
int plays_shell_l_x(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_l | PLAYS_x;
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		char *music_id_c;
		int music_id;
		int music_id_valid;	//music_id有效性
		int music_id_exist;	//music_id是否在自己的收藏中
		int collect_count;
		
		music_id_c = strtok(order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		if(music_id <= 0)
		{
			printf("invalid para\n");
		}
		else
		{
			if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
			{
				printf("[27009]check your net and re-login\n");
				return -1;
			}
			//****
			if( _recv(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
			{
				printf("[27042]check your net and re-login\n");
				return -1;
			}
			if(collect_count == 0)
			{
				printf("no collect music\n");
				return PLAYS_l | PLAYS_x;
			}
			//****
			if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
			{
				printf("[27100]check your net and re-login\n");
				return -1;
			}
			if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
			{
				printf("[27040]check your net and re-login\n");
				return -1;
			}
			if( music_id_valid == -1)	//这个music_id是无效的 不存在的
			{
				printf("%d:invalid para\n", music_id);
			}
			else
			{
				//合法的情况下 还要判断 是否在自己的收藏中
				if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
				{
					printf("[27041]check your net and re-login\n");
					return -1;
				}
				if(music_id_exist == -2)
				{
					printf("%d:not exist in your collect\n", music_id);
				}
				else
					;
			}
		}
		while(1)
		{
			music_id_c = strtok(NULL , "#");
			if(music_id_c == NULL)
				break;
			
			music_id = atoi(music_id_c);
			if(music_id <= 0)
				continue ;
			else
			{
				if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
				{
					printf("[27009]check your net and re-login\n");
					return -1;
				}
				//****
				if( _recv(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
				{
					printf("[27042]check your net and re-login\n");
					return -1;
				}
				if(collect_count == 0)
				{
					printf("no collect music\n");
					return PLAYS_l | PLAYS_x;
				}
				//****
				if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[27100]check your net and re-login\n");
					return -1;
				}
				if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
				{
					printf("[27040]check your net and re-login\n");
					return -1;
				}
				if( music_id_valid == -1)
				{
					printf("%d:invalid para\n", music_id);
				}
				else
				{
					if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
					{
						printf("[27041]check your net and re-login\n");
						return -1;
					}
					if(music_id_exist == -2)
					{
						printf("%d:not exist in your collect\n", music_id);
					}
					else
						;
				}
			}
		}
	}
	
	return PLAYS_l | PLAYS_x;
}

//查看曾经上传的音乐
int plays_shell_l_u(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	
	if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[27101]check your net and re-login\n");
		return -1;
	}
	
	int upload_count;
	if( _recv(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
	{
		printf("[27050]check your net and re-login\n");
		return -1;
	}
	if(upload_count == 0)
	{
		printf("no upload\n");
		return PLAYS_l | PLAYS_u;
	}
	
	int i;
	int music_id;
	char music_name[MUSIC_NAME_MAX_LENGTH];
	
	printf("*********upload music*********\n");
	printf("*******id************name*****\n");
	for(i = 0 ; i < upload_count ; i ++)
	{
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[27051]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[27052]check your net and re-login\n");
			return -1;
		}
		printf("\t%d\t%s\n", music_id , music_name);
	}
	
	printf("******************************\n");
	return PLAYS_l | PLAYS_u;
}

//删除上传的部分音乐
int plays_shell_u_x(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_u | PLAYS_x;
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		char *music_id_c;
		int music_id;
		int music_id_exist;	//music_id是否在自己的上传中
		int upload_count;
		
		music_id_c = strtok(order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		if(music_id <= 0)
		{
			printf("invalid para\n");
		}
		else
		{
			if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
			{
				printf("[27102]check your net and re-login\n");
				return -1;
			}
			//****
			if( _recv(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
			{
				printf("[27060]check your net and re-login\n");
				return -1;
			}
			if(upload_count  == 0)
			{
				printf("no upload music\n");
				return PLAYS_u | PLAYS_x;
			}
			//****
			if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
			{
				printf("[27103]check your net and re-login\n");
				return -1;
			}
			//****
			if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
			{
				printf("[27062]check your net and re-login\n");
				return -1;
			}
			if(music_id_exist == -2)
			{
				printf("%d:not exist in your upload\n", music_id);
			}
			else
				;
			//****
		}
		while(1)
		{
			music_id_c = strtok(NULL , "#");
			if(music_id_c == NULL)
				break;
			
			music_id = atoi(music_id_c);
			if(music_id <= 0)
				continue ;
			else
			{
				if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
				{
					printf("[27102]check your net and re-login\n");
					return -1;
				}
				//****
				if( _recv(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
				{
					printf("[270460]check your net and re-login\n");
					return -1;
				}
				if(upload_count == 0)
				{
					printf("no upload music\n");
					return PLAYS_u | PLAYS_x;
				}
				//****
				if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[27103]check your net and re-login\n");
					return -1;
				}
				//****
				if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
				{
					printf("[27062]check your net and re-login\n");
					return -1;
				}
				if(music_id_exist == -2)
				{
					//不曾上传
					printf("%d:not exist in your upload\n", music_id);
				}
				else
					;	//曾经上传
				//****
			}
		}
	}
	
	return PLAYS_u | PLAYS_x;
}

//用户下载自己上传的音乐
int plays_shell_u_s(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_u | PLAYS_s;
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		char *music_id_c;
		int music_id;
		int music_id_exist;
		char music_name[MUSIC_NAME_MAX_LENGTH];	//60
		bzero(music_name , sizeof(music_name));
		
		music_id_c = strtok( order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		if(music_id <= 0)
		{
			printf("check your para\n");
		}
		else
		{
			if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
			{
				printf("[27103]check your net and re-login\n");
				return -1;
			}
			if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
			{
				printf("[27104]check your net and re-login\n");
				return -1;
			}
			if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
			{
				printf("[27070]check your net and re-login\n");
				return -1;
			}
			if(music_id_exist == -2)
			{
				printf("%d:invalid para\n", music_id);
			}
			else
			{
				if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
				{
					printf("[27071]check your net and re-login\n");
					return -1;
				}
				//插入到下载队列中
				insert_download_queue(order_struct->queue_download , music_id , music_name , "/home/self_download/" , NULL);
			}
		}
		while(1)
		{
			//后续
			music_id_c = strtok(NULL , "#");
			if(music_id_c == NULL)
				break;
			
			music_id = atoi(music_id_c);
			if(music_id <= 0)
				continue ;
			else
			{
				if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
				{
					printf("[27103]check your net and re-login\n");
					return -1;
				}
				if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[27104]check your net and re-login\n");
					return -1;
				}
				if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
				{
					printf("[27070]check your net and re-login\n");
					return -1;
				}
				if(music_id_exist == -2)
				{
					printf("%d:invalid para\n", music_id);
				}
				else
				{
					if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
					{
						printf("[27071]check your net and re-login\n");
						return -1;
					}
					//插入到下载队列中
					insert_download_queue(order_struct->queue_download , music_id , music_name , "/home/self_download/" , NULL);
				}
			}
		}
	}
	int pthread_test;
	
	if(order_pthread->download_thread == 0)
		pthread_create( &(order_pthread->download_thread) , NULL , get_buff_music , (void *)(order_struct) );
	else
	{
		pthread_test = pthread_kill(order_pthread->download_thread , 0);
		if(pthread_test == ESRCH)
			pthread_create( &(order_pthread->download_thread) , NULL , get_buff_music , (void *)(order_struct) );
		else
			;	//下载线程正在运行
	}
	
	return PLAYS_u | PLAYS_s;
}

//利用音乐名字模糊搜索音乐
int plays_shell_f(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_f;
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		
		int i;
		int match_count;
		int music_id;
		char music_name[MUSIC_NAME_MAX_LENGTH];	//60
		bzero(music_name , sizeof(music_name));
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[27104]check your net and re-login\n");
			return -1;
		}
		
		strcpy(music_name , order_struct->file_name);
		if( _send(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[27105]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)&match_count , -1 , 2) == -1)
		{
			printf("[27081]check your net and re-login\n");
			return -1;
		}
		if(match_count == 0)
		{
			printf("this music is not exist\n");
			return PLAYS_f;
		}
		else
		{
			printf("**********match music*********\n");
			printf("*******id************name*****\n");
			for(i = 0 ; i<match_count ; i ++)
			{
				if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[27080]check your net and re-login\n");
					return -1;
				}
				if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
				{
					printf("[27082]check your net and re-login\n");
					return -1;
				}
				printf("\t%d\t%s\n", music_id , music_name);
			}
			printf("******************************\n");
		}

	}
	
	return PLAYS_f;
}

//猜你喜欢
int plays_shell_r(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	int like_count;
	int i;
	int like_music_style;	//“猜你喜欢”的音乐风格码
	int music_id;
	char music_name[MUSIC_NAME_MAX_LENGTH];	//60
	
	if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[28017]check your net and re-login\n");
		return -1;
	}
	
	if( _recv(client_sock_fd , (void *)&like_music_style , -1 , 2) == -1)
	{
		printf("[28904]check your net and re-login\n");
		return -1;
	}
	switch(like_music_style)
	{
		case PLAYS_k_pop:
			printf("guess you like [%s]\n", "pop music");
			break;
			
		case PLAYS_k_rock:
			printf("guess you like [%s]\n", "rock music");
			break;
			
		case PLAYS_k_emo:
			printf("guess you like [%s]\n", "emotional music");
			break;
			
		case PLAYS_k_girl:
			printf("guess you like [%s]\n", "girl voice music");
			break;
			
		case PLAYS_k_boy:
			printf("guess you like [%s]\n", "boy voice music");
			break;
			
		default:
			;	//impossible
			break;
	}
	
	if( _recv(client_sock_fd , (void *)&like_count , -1 , 2) == -1)
	{
		printf("[28901]check your net and re-login\n");
		return -1;
	}
//这里似乎多余 有服务器负责	
//	like_count = (like_count > LIKE_NUMBER ? LIKE_NUMBER : like_count) ;
	
	printf("**********guess like**********\n");
	printf("*******id************name*****\n");
	for(i = 0 ; i<like_count ; i ++)
	{
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[28902]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			printf("[28903]check your net and re-login\n");
			return -1;
		}
		
		printf("\t%d\t%s\n", music_id , music_name);
	}
	printf("******************************\n");
	
	return PLAYS_r;
}

//修改个人信息
int plays_shell_m(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	int choice;
	
	if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[30001]check your net and re-login\n");
		return -1;
	}
	
	printf("1.password\n");
	printf("2.name\n");
	printf("3.age\n");
	printf("4.sex\n");
	printf("5.quit\n");
	
	printf("which:");
	scanf("%d", &choice);
	_flush();
	
	choice = (int)pow(2 , choice);

	if( _send(client_sock_fd , (void *)&choice , -1 , 2) == -1)
	{
		printf("[30005]check your net and re-login\n");
		return -1;
	}
	
	switch(choice)
	{
		case PLAYS_m_p:
			log_display_register_password(order_struct->do_socket->person);
			break;
			
		case PLAYS_m_n:
			log_display_register_name(order_struct->do_socket->person);
			break;
		
		case PLAYS_m_a:
			log_display_register_age(order_struct->do_socket->person);
			break;
		
		case PLAYS_m_s:
			log_display_register_sex(order_struct->do_socket->person);
			break;
		
		case PLAYS_m_q:
			return PLAYS_m;
//			break;
			
		default:
			printf("error\n");
			return PLAYS_m;
//			break;
	}
	send_user_info(client_sock_fd , order_struct->do_socket->person);
	
	return PLAYS_m;
}

//查看个人信息
int plays_shell_i(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	printf("id:%s\n", order_struct->do_socket->person->id);
	printf("name:%s\n", order_struct->do_socket->person->name);
	printf("email:%s\n", order_struct->do_socket->person->email);
	printf("age:%d\n", order_struct->do_socket->person->age);
	printf("sex:%c\n", order_struct->do_socket->person->sex);
	printf("reg_time:%s\n", order_struct->do_socket->person->reg_time);
	
	return PLAYS_i;
}

//评论音乐
int plays_shell_c(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_c;
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		char *music_id_c;
		int music_id;
		int music_id_valid;
		
		music_id_c = strtok(order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[30002]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[30003]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
		{
			printf("[31001]check your net and re-login\n");
			return -1;
		}
		
		if( music_id_valid == -1)
		{
			//music id不合法
			printf("invalid music_id\n");
			return PLAYS_c;
		}
		else
		{
			//music id合法
			char comment[MAX_LENGTH_COMMENT];	//363
			bzero(comment , sizeof(comment));
			
			char *temp;
			temp = (char *)malloc(sizeof(char) * MAX_INPUT_BUFF );
			
			printf("comment[enter to finish]:");
			fgets(temp , MAX_INPUT_BUFF , stdin);
			strncpy(comment , temp , MAX_LENGTH_COMMENT - 1);
			
			if(strlen(comment) < MAX_LENGTH_COMMENT - 1)	//remove last enter
				comment[strlen(comment) - 1] = '\0';
			
			free(temp);
			
			if( _send(client_sock_fd , (void *)comment , sizeof(comment) , 3) == -1)
			{
				printf("[30004]check your net and re-login\n");
				return -1;
			}
			
			printf("comment successfully!\n");
			return PLAYS_c;
		}
	}

	return PLAYS_c;
}

//查看音乐的评论
int plays_shell_c_l(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return PLAYS_c | PLAYS_l;
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		char *music_id_c;
		int music_id;
		int music_id_valid;
		
		music_id_c = strtok(order_struct->file_name , "#");
		music_id = atoi(music_id_c);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[30006]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			printf("[30007]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&music_id_valid , -1 , 2) == -1)
		{
			printf("[32001]check your net and re-login\n");
			return -1;
		}
		
		if(music_id_valid == -1)
		{
			//music id无效
			printf("invalid music_id\n");
			return PLAYS_c | PLAYS_l;
		}
		else
		{
			//music id有效
			int i;
			int comment_count;
			char music_name[MUSIC_NAME_MAX_LENGTH];	//60
			bzero(music_name , sizeof(music_name));
			
			if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			{
				printf("[32003]check your net and re-login\n");
				return -1;
			}
			
			if( _recv(client_sock_fd , (void *)&comment_count , -1 , 2) == -1)
			{
				printf("[32002]check your net and re-login\n");
				return -1;
			}
			printf("******************************\n");
			printf("id:%d\n", music_id);
			printf("name:%s\n", music_name);
			printf("************comment***********\n");
			
			char comment[MAX_LENGTH_COMMENT];	//363
			
			for(i = 0 ; i<comment_count ; i ++)
			{
				bzero(comment , sizeof(comment));
				if( _recv(client_sock_fd , (void *)comment , sizeof(comment) , 3) == -1)
				{
					printf("[32004]check your net and re-login\n");
					return -1;
				}
				printf("%d.%s\n", i+1 , comment);
			}
			printf("******************************\n");
		}
	}
	
	return PLAYS_c | PLAYS_l;
}

