#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

void init_pthread_number(struct pthread_number *order_pthread)
{
	order_pthread->play_thread = 0;	//
	order_pthread->download_thread = 0;	//
	order_pthread->updownload_thread = 0;	//
	order_pthread->comment_thread = 0;
	order_pthread->online_thread = 0;
}

void destroy_pthread_number(struct pthread_number *order_pthread)
{
	int pthread_test;
	
	//结束下载线程
	if(order_pthread->download_thread != 0)
	{
		pthread_test = pthread_kill( order_pthread->download_thread , 0);
		if(pthread_test != ESRCH)
			pthread_cancel(order_pthread->download_thread );
	}
	
	//结束上传线程
	if(order_pthread->updownload_thread != 0)
	{
		pthread_test = pthread_kill( order_pthread->updownload_thread , 0);
		if(pthread_test != ESRCH)
			pthread_cancel(order_pthread->updownload_thread);
	}
	
	//结束播放线程
	if(order_pthread->play_thread != 0)
	{
		pthread_test = pthread_kill( order_pthread->play_thread , 0);
		if(pthread_test != ESRCH)
			pthread_cancel(order_pthread->play_thread);
	}
	
}

int op_shell(struct music_dir *user_music , struct socket_t *do_socket , int mode_flag)
{
	struct pthread_number order_pthread;
	init_pthread_number(&order_pthread);
	
	char order[SHELL_MAX_LENGTH * 2];	//load user's instruction(s) 50
	char order_name[SHELL_MAX_LENGTH];
	char order_para[SHELL_MAX_LENGTH];
	char file_name[SHELL_MAX_MUSIC_COUNT][SHELL_MAX_LENGTH];	//[10][100]

	int i = 0;
	int length;
	int explain_result;	//存储命令解释的返回值
		
	int order_name_i = 0;
	int file_name_i = 0;
	int file_name_j = 0;
	int order_para_i = 0;

	int explain_shell_result;	//命令执行完之后的结果
	char *temp_order = (char *)malloc(MAX_INPUT_BUFF * sizeof(char) );
	bzero(temp_order , sizeof(temp_order) );
	
	//命令行相关结构(主结构)
	struct music_shell *order_struct = (struct music_shell *)malloc(sizeof(struct music_shell) );
	
	//播放队列
	struct play_music_queue *play_queue = (struct play_music_queue *)malloc(sizeof(struct play_music_queue) ) ;
	init_play_queue(play_queue);

	//下载队列
	struct download_queue *queue_download = (struct download_queue *)malloc(sizeof(struct download_queue));
	init_download_queue(queue_download);
	//上传队列
	struct upload_queue *queue_upload = (struct upload_queue *)malloc(sizeof(struct upload_queue) );
	init_upload_queue(queue_upload);

	order_struct->do_socket = do_socket;
	order_struct->play_queue = play_queue;
	order_struct->queue_download = queue_download;
	order_struct->queue_upload = queue_upload;
	
	while(1)
	{
		i = 0;
		order_name_i = 0;
		file_name_i = 0;
		file_name_j = 0;
		order_para_i = 0;
		
		printf("~");
		fgets(temp_order , MAX_INPUT_BUFF , stdin);
		strncpy(order , temp_order , SHELL_MAX_LENGTH -1 );
		if(order[0] == '\n')
			continue ;
		
		if(strlen(order) < SHELL_MAX_LENGTH - 1)	//remove last enter
			order[strlen(order) - 1] = '\0';
			
		//****************
		//****************
		length = strlen(order);
		length --;
		while(order[length] == ' ')
			length --;
		
		length ++;
		order[length] = '\0';

		while(order[i] != '\0' && order[i] == ' ')
			i ++;
		
		while(order[i] != '\0' && order[i] != ' ')
		{
			order_name[order_name_i] = order[i];
			order_name_i ++;
			i ++;
		}
		order_name[order_name_i] = '\0';
		
		for(; i<length ; i ++)
		{
			if(order[i] == '-')
			{
				for(i =i +1 ; order[i] != '\0' && order[i] != ' ' ; i ++)
					order_para[order_para_i ++] = order[i];
			}
			else
				if(order[i] != ' ')	
				{
					for(; order[i] != '\0' && order[i] != ' ' ; i ++)
						file_name[file_name_i][file_name_j ++] = order[i];
					
					file_name[file_name_i][file_name_j] = '\0';
					file_name_i ++;
					file_name_j = 0;
				}
		}			
		order_para[order_para_i] = '\0';
		//****************
		//****************

		//将所有的音乐文件名拼接成一个
		temp_order[0] = '\0';
		for(i = 0 ; i<file_name_i ; i ++)
		{
			strcat(temp_order , file_name[i]);
			strcat(temp_order , "#");
		}
		temp_order[strlen(temp_order) - 1] = '\0';	//消除末尾的#

		if( strcmp(order_name , "logout") == 0)
		{
			explain_result = LOGOUT;
			system("clear");
			
			if( _send(do_socket->client_sock_fd , (void *)&explain_result , -1 , 2) == -1)
			{
				;	//空实现
			}
			break;
		}
		if( strcmp(order_name , "quit") == 0)
		{
			explain_result = QUIT;
			
			if( _send(do_socket->client_sock_fd , (void *)&explain_result , -1 , 2) == -1)
			{
				;	//空实现
			}
//			system("clear");	//退出清屏会引起怀疑
			break;
		}
		
		explain_shell_result = explain_shell(order_name , order_para , temp_order , file_name_i , &order_pthread , order_struct , user_music , mode_flag);
		if(explain_shell_result == -1)
		{
			printf("internet interrupt or server error ，outline!!!\n");	//网络中断或server问题
			continue ;	//这句话是废话 突出含义而已
		}
		if(explain_shell_result > 0)
		{
			;	//参数错误 或者 正常
			continue ;
		}
	}
	
	//销毁各个线程
	destroy_pthread_number(&order_pthread);
	
	free(temp_order);
	free(queue_download);
	free(queue_upload);
	free(play_queue);
	free(order_struct);

	return explain_result;
}//end op_shell

int get_order_name_file(char *order , char *order_name , char *file_name[] , int *file_count)
{
	int i = 0;
	
	int length = strlen(order);
	
	int order_name_i = 0;
	int file_name_i = 0;
	int file_name_j = 0;
	
	while(order[i] == ' ')	//将命令前面的空格全部无视掉
		i ++;
	
	while(order[i] != ' ')
		order_name[order_name_i ++] = order[i ++];
	
	order_name[order_name_i] = '\0';
	
	for(; i<length ; i ++)
	{
		if(order[i] != ' ')	
		{
			for(; order[i] != ' ' ; i ++)
				file_name[file_name_i][file_name_j ++] = order[i];
			
			file_name[file_name_i][file_name_j] = '\0';
			file_name_i ++;
			file_name_j = 0;
		}
	}
	
	return 1;
	
}

int order_para_str_to_int(char *order_para)
{
	int number = PLAY_m;	//1

	//a n 参数同时出现 那后面的数字就需要一对对的出现
	if(strstr(order_para , "n") && strstr(order_para , "a") )
	{
		number = 0;
		number |= PLAY_n;
		number |= PLAY_a;
		return number;
	}
	
	//a d参数同时出现 后面可以多带几个数字 每一个数字的意思就是d参数显示的一个目录id
	if(strstr(order_para , "d") && strstr(order_para , "a") )
	{
		number = 0;
		number |= PLAY_d;
		number |= PLAY_a;
		return number;
	}
	
	if(strstr(order_para , "q"))
	{
		number = 0;
		number |= PLAY_q;	//有了q参数，其余所有参数失效
		return number;
	}
	
	if(strstr(order_para , "c"))
	{
		number = 0;
		number |= PLAY_c;
		return number;
	}
	
	if(strstr(order_para , "R"))
	{
		number = 0;
		number |= PLAY_R;
		return number;
	}
	
	if(strstr(order_para , "r"))
	{
		number = 0;
		number |= PLAY_r;
		return number;
	}
	
	if(strstr(order_para , "t"))
	{
		number = 0;
		number |= PLAY_t;
		return number;
	}
	
	if(strstr(order_para , "p"))
	{
		number = 0;
		number |= PLAY_p;
		return number;
	}
	
	if(strstr(order_para , "d"))
	{
		number = 0;
		number |= PLAY_d;
		return number;
	}
	
	if(strstr(order_para , "n"))
	{
		number = 0;
		number |= PLAY_n;
		return number;
	}
	
	if(strstr(order_para , "a"))
	{
		number = 0;
		number |= PLAY_a;
		return number;
	}
	
	if(strstr(order_para , "f"))
	{
		number = 0;
		number |= PLAY_f;
		return number;
	}
	
	if(strstr(order_para , "l"))
	{
		number = 0;
		number |= PLAY_l;
		return number;
	}
	return number;
}

int plays_order_para_str_to_int(char *order_para , int mode_flag)
{
	int number = 0;

	if(mode_flag == MODE_LOAD)
	{	
		//正常登录才能使用
		if(strstr(order_para , "u") && strstr(order_para , "d") )
		{
			number |= PLAYS_d;
			number |= PLAYS_u;
			return number;
		}
		
		if(strstr(order_para , "l") && strstr(order_para , "d"))
		{
			number |= PLAYS_l;
			number |= PLAYS_d;
			return number;
		}
		
		if(strstr(order_para , "l") && strstr(order_para , "x"))
		{
			number |= PLAYS_l;
			number |= PLAYS_x;
			return number;
		}
		
		if(strstr(order_para , "l") && strstr(order_para , "u"))
		{
			number |= PLAYS_l;
			number |= PLAYS_u;
			return number;
		}
		
		if(strstr(order_para , "u") && strstr(order_para , "x"))
		{
			number |= PLAYS_u;
			number |= PLAYS_x;
			return number;
		}
		
		if(strstr(order_para , "u") && strstr(order_para , "s"))
		{
			number |= PLAYS_u;
			number |= PLAYS_s;
			return number;
		}
		
		if(strstr(order_para , "c") && strstr(order_para , "l"))
		{
			number |= PLAYS_c;
			number |= PLAYS_l;
			return number;
		}
		
		if(strstr(order_para , "u"))
		{
			number |= PLAYS_u;
			return number;
		}
		
		if(strstr(order_para , "l"))
		{
			number |= PLAYS_l;
			return number;
		}
		
		if(strstr(order_para , "c"))
		{
			number |= PLAYS_c;
			return number;
		}
		
		if(strstr(order_para , "r"))
		{
			number |= PLAYS_r;
			return number;
		}
		
		if(strstr(order_para , "m"))
		{
			number |= PLAYS_m;
			return number;
		}
		
		if(strstr(order_para , "i"))
		{
			number |= PLAYS_i;
			return number;
		}
	}
	
	//匿名用户使用
	if(strstr(order_para , "t"))
	{
		number |= PLAYS_t;
		return number;
	}
	
	if(strstr(order_para , "s"))
	{
		number |= PLAYS_s;
		return number;
	}
	
	if(strstr(order_para , "o"))
	{
		number |= PLAYS_o;
		return number;
	}
	
	if(strstr(order_para , "d"))
	{
		number |= PLAYS_d;
		return number;
	}

	if(strstr(order_para , "k"))
	{
		number |= PLAYS_k;
		return number;
	}
	
	if(strstr(order_para , "f"))
	{
		number |= PLAYS_f;
		return number;
	}

	return -1;	//暂时用-1代替
}

int friend_order_para_str_to_int(char *order_para)
{
	int number = FRIEND;
	
	if(strstr(order_para , "a"))
	{
		number += FRIEND_A;
		return number;
	}
	
	if(strstr(order_para , "d"))
	{
		number += FRIEND_D;
		return number;
	}
	
	if(strstr(order_para , "f"))
	{
		number += FRIEND_F;
		return number;
	}
	
	if(strstr(order_para , "n"))
	{
		number += FRIEND_N;
		return number;
	}
	
	if(strstr(order_para , "l"))
	{
		number += FRIEND_L;
		return number;
	}
	
	if(strstr(order_para , "c"))
	{
		number += FRIEND_C;
		return number;
	}
	
	if(strstr(order_para , "u"))
	{
		number += FRIEND_U;
		return number;
	}
	
	if(strstr(order_para , "s"))
	{
		number += FRIEND_S;
		return number;
	}
	
	if(strstr(order_para , "t"))
	{
		number += FRIEND_T;
		return number;
	}
	
	return -1;
}

void get_random_music(struct music_dir *user_music , char *random_music_dir , char *random_music_music)
{
	int directory;
	int music;
	
	srand((unsigned) time(NULL));
	
	int dir_counter = user_music[0].music_number;
	directory = rand() % dir_counter + 1;
	int music_counter = atoi(user_music[directory].music_name[0]);
	music = rand() % music_counter + 1;
	
	strcpy(random_music_dir , user_music[directory].music_path);
	strcpy(random_music_music , user_music[directory].music_name[music]);
}

int explain_shell(char *order_name , char *order_para , char *file_name , int file_count , struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music , int mode_flag)
{
	strcpy(order_struct->order_name , order_name);
	
	if(strcmp(order_struct->order_name , "play") == 0)
		order_struct->order_para = order_para_str_to_int(order_para);	//参数由字符数字化为数字，利用位运算 更便捷
	else
	{
		if(strcmp(order_struct->order_name , "plays") == 0)
			order_struct->order_para = plays_order_para_str_to_int(order_para , mode_flag);
		else
		{
			if(strcmp(order_struct->order_name , "friend") == 0)
				order_struct->order_para = friend_order_para_str_to_int(order_para);
		}
	}		
	strcpy(order_struct->file_name , file_name);
	order_struct->file_count = file_count;
	
printf("para :%s %d\n", order_para , order_struct->order_para);

	if(strcmp(order_struct->order_name , "play") == 0)
	{
		play_shell(order_pthread , order_struct , user_music);
		return PLAY;
	}
	
	if(strcmp(order_struct->order_name , "cls") == 0)
	{
		system("clear");
		return CLS;
	}
	
	//单机不能使用与网络相关
	if( mode_flag != MODE_ALONE)
	{
		if(strcmp(order_struct->order_name , "plays") == 0)
		{
			return plays_shell(order_pthread , order_struct , user_music );
		}
	}
	
	if( mode_flag == MODE_LOAD)
	{
		//登录用户才能够使用好友功能（匿名和未登录不能使用）
		if(strcmp(order_struct->order_name , "friend") == 0)
		{
			return friend_shell(order_pthread , order_struct);
		}
	}
	printf("unknown order,check it\n");
	return -2;
}