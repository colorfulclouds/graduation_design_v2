#ifndef FRIEND_GO
#define FRIEND_GO
#include "friend_go.h"
#endif

int friend_shell(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if( order_struct->order_para == (FRIEND + FRIEND_A) )	//21
		return friend_shell_a(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_D) )	//23
		return friend_shell_d(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_F) )	//27
		return friend_shell_f(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_N) )	//35
		return friend_shell_n(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_L) )	//51
		return friend_shell_l(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_C) )	//83
		return friend_shell_c(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_U) )	//147
		return friend_shell_u(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_S) )	//275
		return friend_shell_s(order_pthread , order_struct );
		
	if( order_struct->order_para == (FRIEND + FRIEND_T) )
		return friend_shell_t(order_pthread , order_struct );
	
	return 1;
}

int friend_shell_a(struct pthread_number *order_pthread , struct music_shell *order_struct)
{	
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_A;
	}
	else
	{
		if(strcmp(order_struct->do_socket->person->id , order_struct->file_name) == 0)
		{
			printf("can not add your self\n");
			return FRIEND + FRIEND_A ;
		}
		
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		int user_id_exist;	//欲添加的用户存在与否
		int is_pal;			//这个用户已存在的前提下 是否已经是自己的好友
		char user_id[MAX_USER_ID_LENGTH];	//6
		bzero(user_id , sizeof(user_id));
		strcpy(user_id , order_struct->file_name);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28001]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28002]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)&user_id_exist , -1 , 2) == -1)
		{
			printf("[28101]check your net and re-login\n");
			return -1;
		}
		
		if( user_id_exist == -2)
		{
			//user_id无效
			printf("user is not exist\n");
			return FRIEND + FRIEND_A ;
		}
		else
		{
			//user_id有效
			if( _recv(client_sock_fd , (void *)&is_pal , -1 , 2) == -1)
			{
				printf("[29104]check your net and re-login\n");
				return -1;
			}
			
			if(is_pal == 1)
			{
				printf("already your pal\n");
				return FRIEND + FRIEND_A ;
			}
			else
			{			
				printf("request sent\n");
				return FRIEND + FRIEND_A ;
			}

			return FRIEND + FRIEND_A ;
		}
	}

	return FRIEND + FRIEND_A ;
}

int friend_shell_d(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_D;
	}
	else
	{
		if(strcmp(order_struct->do_socket->person->id , order_struct->file_name) == 0)
		{
			printf("is not your friend\n");
			return FRIEND + FRIEND_D ;
		}
		
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		int user_id_exist;
		char user_id[MAX_USER_ID_LENGTH];	//6
		bzero(user_id , sizeof(user_id));
		strcpy(user_id , order_struct->file_name);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28003]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28004]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)&user_id_exist , -1 , 2) == -1)
		{
			printf("[28201]check your net and re-login\n");
			return -1;
		}
		if( user_id_exist == -2)
		{
			printf("is not your friend\n");
			return FRIEND + FRIEND_D ;
		}
		else
		{
			printf("delete successfully\n");
			return FRIEND + FRIEND_D ;
		}
		
	}
	
	return FRIEND + FRIEND_D;
}

int friend_shell_f(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	int friend_count;
	int i;
	
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	char user_name[MAX_USER_NAME_LENGTH];	//20
	bzero(user_name ,sizeof(user_name));
	
	if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[28005]check your net and re-login\n");
		return -1;
	}
	
	if( _recv(client_sock_fd , (void *)&friend_count , -1 , 2) == -1)
	{
		printf("[28301]check your net and re-login\n");
		return -1;
	}
	
	printf("***********friend(s)**********\n");
	printf("*******id************name*****\n");
	for(i = 0 ; i<friend_count ; i ++)
	{
		if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28302]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
		{
			printf("[28303]check your net and re-login\n");
			return -1;
		}
		
		printf("\t%s\t%s\n", user_id , user_name);
	}
	printf("******************************\n");
	
	return FRIEND + FRIEND_F;
}

int friend_shell_n(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_D;
	}
	else
	{
		int i;
		int match_count;
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		char user_id[MAX_USER_ID_LENGTH];		//6
		bzero(user_id , sizeof(user_id));
		char user_name[MAX_USER_NAME_LENGTH];	//20
		bzero(user_name ,sizeof(user_name));
		strcpy(user_name , order_struct->file_name);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28006]check your net and re-login\n");
			return -1;
		}
		
		if( _send(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
		{
			printf("[28007]check your net and re-login\n");
			return -1;
		}
		
		if( _recv(client_sock_fd , (void *)&match_count , -1 , 2) == -1)
		{
			printf("[28401]check your net and re-login\n");
			return -1;
		}
		
		printf("***********friend(s)**********\n");
		printf("*******id************name*****\n");
		for(i = 0 ; i<match_count ; i ++)
		{
			if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
			{
				printf("[28402]check your net and re-login\n");
				return -1;
			}
			if( _recv(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
			{
				printf("[28403]check your net and re-login\n");
				return -1;
			}
			
			printf("\t%s\t%s\n", user_id , user_name);
		}
		printf("******************************\n");
	}
	
	return FRIEND + FRIEND_N;
}

int friend_shell_l(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_L;
	}
	else
	{
		if(strcmp(order_struct->do_socket->person->id , order_struct->file_name) == 0)
		{
			printf("can not look yourself\n");
			return FRIEND + FRIEND_L ;
		}
		
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		int user_id_exist;
		char user_id[MAX_USER_ID_LENGTH];	//6
		bzero(user_id , sizeof(user_id));
		strcpy(user_id , order_struct->file_name);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28008]check your net and re-login\n");
			return -1;
		}
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28009]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&user_id_exist , -1 , 2) == -1)
		{
			printf("[28501]check your net and re-login\n");
			return -1;
		}
		if( user_id_exist == -2)
		{
			printf("is not your friend\n");
			return FRIEND + FRIEND_L ;
		}
		else
		{
			//这里开始接收好友的收藏
			int i;
			int collect_count;
			int music_id;
			char music_name[MUSIC_NAME_MAX_LENGTH];	//60
			bzero(music_name , sizeof(music_name));
			
			if( _recv(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
			{
				printf("[28502]check your net and re-login\n");
				return -1;
			}
			
			printf("********%s:collect music******\n", user_id);
			printf("*******id************name*****\n");
			for(i = 0 ; i<collect_count ; i ++)
			{
				if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[28503]check your net and re-login\n");
					return -1;
				}
				if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
				{
					printf("[28504]check your net and re-login\n");
					return -1;
				}
				
				printf("\t%d\t%s\n", music_id , music_name);
			}
			printf("******************************\n");
			
			return FRIEND + FRIEND_L ;
		}
	}
	
	return FRIEND + FRIEND_L;
}

int friend_shell_c(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_C;
	}
	else
	{
		if(strcmp(order_struct->do_socket->person->id , order_struct->file_name) == 0)
		{
			printf("can not look yourself\n");
			return FRIEND + FRIEND_C ;
		}
		
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		int user_id_exist;
		char user_id[MAX_USER_ID_LENGTH];	//6
		bzero(user_id , sizeof(user_id));
		strcpy(user_id , order_struct->file_name);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28010]check your net and re-login\n");
			return -1;
		}
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28011]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&user_id_exist , -1 , 2) == -1)
		{
			printf("[28601]check your net and re-login\n");
			return -1;
		}
		if( user_id_exist == -2)
		{
			printf("is not your friend\n");
			return FRIEND + FRIEND_C;
		}
		else
		{
			printf("copy successfully\n");
			return FRIEND + FRIEND_C;
		}
	}
	
	return FRIEND + FRIEND_C;
}

int friend_shell_u(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count == 0)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_U;
	}
	else
	{
		if(strcmp(order_struct->do_socket->person->id , order_struct->file_name) == 0)
		{
			printf("can not look yourself\n");
			return FRIEND + FRIEND_U ;
		}
		
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		int user_id_exist;
		char user_id[MAX_USER_ID_LENGTH];	//6
		bzero(user_id , sizeof(user_id));
		strcpy(user_id , order_struct->file_name);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28012]check your net and re-login\n");
			return -1;
		}
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28013]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&user_id_exist , -1 , 2) == -1)
		{
			printf("[28701]check your net and re-login\n");
			return -1;
		}
		if( user_id_exist == -2)
		{
			printf("is not your friend\n");
			return FRIEND + FRIEND_U;
		}
		else
		{
			//接下来接收好友曾经上传的音乐
			int i;
			int music_id;
			char music_name[MUSIC_NAME_MAX_LENGTH];	//60
			bzero(music_name , sizeof(music_name));
			int upload_count;
			
			if( _recv(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
			{
				printf("[28702]check your net and re-login\n");
				return -1;
			}
			printf("********%s:upload music*******\n", user_id);
			printf("*******id************name*****\n");
			for(i = 0 ; i<upload_count ; i ++)
			{
				if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
				{
					printf("[28703]check your net and re-login\n");
					return -1;
				}
				if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
				{
					printf("[28704]check your net and re-login\n");
					return -1;
				}
				
				printf("\t%d\t%s\n", music_id , music_name);
			}
			printf("******************************\n");
			
			return FRIEND + FRIEND_U;
		}
	}

	return FRIEND + FRIEND_U;
}

int friend_shell_s(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	if(order_struct->file_count <= 1)
	{
		printf("lack of paras\n");
		return FRIEND + FRIEND_S; 
	}
	else
	{
		int client_sock_fd = order_struct->do_socket->client_sock_fd;
		int user_id_exist;	//好友存在与否
		int music_id_exist;	//好友上传音乐与否
		char user_id[MAX_USER_ID_LENGTH];	//6
		bzero(user_id , sizeof(user_id));
		int music_id;

		char *p;

		p = strtok(order_struct->file_name , "#");
		strcpy(user_id , p);
		if( strcmp(order_struct->do_socket->person->id , user_id) == 0)
		{
			printf("can not op yourself\n");
			return FRIEND + FRIEND_S;
		}
		p = strtok(NULL , "#");
		music_id = atoi(p);
		
		if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
		{
			printf("[28014]check your net and re-login\n");
			return -1;
		}
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[28015]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)&user_id_exist , -1 , 2) == -1)
		{
			printf("[28801]check your net and re-login\n");
			return -1;
		}

		if( user_id_exist == -2)
		{
			//不是自己的好友
			printf("is no your friend\n");
			return FRIEND + FRIEND_S;
		}
		else
		{
			//是自己的好友 发送music_id
			if( _send(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
			{
				printf("[28016]check your net and re-login\n");
				return -1;
			}
			//接收music_id的有效性
			if( _recv(client_sock_fd , (void *)&music_id_exist , -1 , 2) == -1)
			{
				printf("[28802]check your net and re-login\n");
				return -1;
			}

			if( music_id_exist == -2)
			{
				printf("your pal no have this music\n");
				return FRIEND + FRIEND_S;
			}
			else
			{
				char music_name[MUSIC_NAME_MAX_LENGTH];	//60
				bzero(music_name , sizeof(music_name));
				
				if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
				{
					printf("[28803]check your net and re-login\n");
					return -1;
				}
				
				insert_download_queue(order_struct->queue_download , music_id , music_name , "/home/other_download/" , user_id);
				//执行下载线程
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
			}
		}
	}
	
	return FRIEND + FRIEND_S;
}

int friend_shell_t(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;

	if( send_user_request(client_sock_fd , order_struct->order_para) == -1)
	{
		printf("[28018]check your net and re-login\n");
		return -1;
	}
	
	if(order_struct->file_count == 0)
	{
		//查看请求添加列表
		return friend_shell_t_l(order_pthread , order_struct);
	}
	else
	{
		//同意添加
		return friend_shell_t_a(order_pthread , order_struct);
	}
	
	return FRIEND + FRIEND_T;
}

int friend_shell_t_l(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	int mode;	//t参数的功能模式（两个功能之一）
	
	mode = FRIEND_T_l;
	if( _send(client_sock_fd , (void *)&mode , -1 , 2) == -1)
	{
		printf("[28018]check your net and re-login\n");
		return -1;
	}

	int i;
	int request_count;
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	char user_name[MAX_USER_NAME_LENGTH];	//20
	bzero(user_name , sizeof(user_name));
	
	if( _recv(client_sock_fd , (void *)&request_count , -1 , 2) == -1)
	{
		printf("[29101]check your net and re-login\n");
		return -1;
	}
	
	printf("**********request add*********\n");
	printf("*******id************name*****\n");
	for(i = 0 ; i<request_count ; i ++)
	{
		if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			printf("[29102]check your net and re-login\n");
			return -1;
		}
		if( _recv(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
		{
			printf("[29103]check your net and re-login\n");
			return -1;
		}
		
		printf("\t%s\t%s\n", user_id , user_name);
	}
	printf("******************************\n");
	
	return FRIEND + FRIEND_T;
}

int friend_shell_t_a(struct pthread_number *order_pthread , struct music_shell *order_struct)
{
	int client_sock_fd = order_struct->do_socket->client_sock_fd;
	int mode;	//t参数的功能模式（两个功能之一）

	mode = FRIEND_T_a;
	if( _send(client_sock_fd , (void *)&mode , -1 , 2) == -1)
	{
		printf("[28018]check your net and re-login\n");
		return -1;
	}
	
	int is_request;
	
	char user_id[MAX_USER_ID_LENGTH];	//
	bzero(user_id , sizeof(user_id));
	
	char *p = strtok(order_struct->file_name , "#");
	
	strcpy(user_id , p);
	
	if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		printf("[28021]check your net and re-login\n");
		return -1;
	}
	
	if( _recv(client_sock_fd , (void *)&is_request , -1 , 2) == -1)
	{
		printf("[29105]check your net and re-login\n");
		return -1;
	}
	if( is_request == 1)
	{
		//请求列表中有
		printf("now is your pal!\n");
		return FRIEND + FRIEND_T;
	}
	else
	{
		//请求列表中没有
		printf("no this pal request\n");
		return FRIEND + FRIEND_T;
	}
	
	
	return FRIEND + FRIEND_T;
}

