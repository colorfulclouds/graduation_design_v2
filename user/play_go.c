#ifndef PLAY_GO
#define PLAY_GO
#include "play_go.h"
#endif

int play_shell(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	//查看播放队列
	if( order_struct->order_para == PLAY_q)
		return play_go_q(order_pthread , order_struct , user_music);
	
	//直接play命令 随便选一首播放
	if( order_struct->order_para == PLAY_m )
		return play_go_m(order_pthread , order_struct , user_music);
	
	//下一首（在播放队列中切换）
	if( order_struct->order_para == PLAY_f)
		return play_go_f(order_pthread , order_struct , user_music);
	
	//上一首（在播放队列中切换）
	if( order_struct->order_para == PLAY_l )
		return play_go_l(order_pthread , order_struct , user_music);
	
	//显示本机中配置文件制定目录中的音乐
	if( order_struct->order_para == PLAY_d)
		return play_go_d(order_pthread , order_struct , user_music);
		
	//用户给出两个数字或一个数字的时候，进行播放音乐或播放队列中的音乐
	if( order_struct->order_para == PLAY_n)
		return play_go_n(order_pthread , order_struct , user_music);
	
	//在播放队列中增加音乐（根据文件名增加）
	if( order_struct->order_para == PLAY_a)
		return play_go_a(order_pthread , order_struct , user_music);
	
	//在播放队列中增加音乐（根据-d参数中的两个数字进行增加）
	if( order_struct->order_para == (PLAY_a | PLAY_n) )
		return play_go_a_n(order_pthread , order_struct , user_music);
	
	//清空播放队列	如果后面带一些数字 删除队列中指定的音乐 但是不能删除正在播放的音乐
	if(	order_struct->order_para == PLAY_c)
		return play_go_c(order_pthread , order_struct , user_music);
	
	//改变循环状态和指定的单曲循环
	if( order_struct->order_para == PLAY_r)
		return play_go_r(order_pthread , order_struct , user_music);
	
	//改变播放的随机状态
	if( order_struct->order_para == PLAY_R)
		return play_go_R(order_pthread , order_struct , user_music);
	
	//添加一个目录（配置文件中的目录）中的全部音乐
	if(order_struct->order_para == (PLAY_a | PLAY_d))
		return play_go_a_d(order_pthread , order_struct , user_music);
	
	//添加一个目录（不是配置文件中的目录）中的全部音乐 需要指定特定的目录了
	if(order_struct->order_para == PLAY_p)
		return play_go_p(order_pthread , order_struct , user_music);
	
	//暂停或者开始播放
	if(order_struct->order_para == PLAY_t)
		return play_go_t(order_pthread , order_struct , user_music);
	
	//配置文件中增加新的目录
	if(order_struct->order_para == PLAY_x)
		return play_go_x(order_pthread , order_struct , user_music);
		
	return 3;
}

//查看播放队列
int play_go_q(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int music_is_playing_count;
	music_is_playing_count = print_queue(order_struct->play_queue);
	printf("there is %d music(s) is playing\n", music_is_playing_count);
	return PLAY_q;
}

//本机上的操作 随机播放
int play_go_m(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	char random_music_dir[CONF_PATH_MAX_LENGTH];
	char random_music_music[MUSIC_NAME_MAX_LENGTH];
	
	int pthread_test;
	pthread_t play_thread;	//播放音乐的线程
	
	if(order_struct->file_count == 0)
	{
		//本机播放，而且没有指定音乐
		bzero(random_music_dir , sizeof(random_music_dir) );
		bzero(random_music_music , sizeof(random_music_music) );
		get_random_music(user_music ,  random_music_dir , random_music_music);
		
		if(order_pthread->play_thread != 0)
		{
			pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
			if( pthread_test == ESRCH )
			{
				insert_random_music(order_struct->play_queue , random_music_dir , random_music_music);
				order_struct->play_queue->playing_pointer = order_struct->play_queue->front;	//播放指针指向播放队列中的第一首音乐
				pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
				order_pthread->play_thread = play_thread;
			}
			else
			{
				insert_random_music(order_struct->play_queue , random_music_dir , random_music_music);
			}
		}
		else
		{
			insert_random_music(order_struct->play_queue , random_music_dir , random_music_music);
			order_struct->play_queue->playing_pointer = order_struct->play_queue->front;	//播放指针指向播放队列中的第一首音乐
			pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
			order_pthread->play_thread = play_thread;
		}
		
		return PLAY_m;	
	}
	return PLAY_m;
}

//下一首（在播放队列中切换）
int play_go_f(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int pthread_test;
	pthread_t play_thread;	//播放音乐的线程
	
	if(order_pthread->play_thread == 0)
	{
		printf("there is no music is playing\n");
		return PLAY_f;
	}
	else
	{
		pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
		if(pthread_test == ESRCH)
		{
			//播放线程没有工作，说明没有音乐在播放
			printf("there is no music is playing\n");
			return PLAY_f;
		}
		else
		{
			if(order_struct->play_queue->playing_pointer + 1 >= order_struct->play_queue->rear)
			{
				//正在播放的已经是最后一首了，再也下不去了
				printf("already last music\n");
				return -PLAY_f;
			}
			else
			{
				pthread_cancel(order_pthread->play_thread);
				order_struct->play_queue->playing_pointer += 1;
			}
			
			pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
			order_pthread->play_thread = play_thread;
			
			return PLAY_f;
		}
	}
}

//上一首（在播放队列中切换）
int play_go_l(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int pthread_test;
	pthread_t play_thread;	//播放音乐的线程
	
	if(order_pthread->play_thread == 0)
	{
		printf("there is no music is playing\n");
		return PLAY_l;
	}
	else
	{
		pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
		if(pthread_test == ESRCH)
		{
			printf("there is no music is playing\n");
			return PLAY_l;
		}
		else
		{
			if(order_struct->play_queue->playing_pointer - 1 < order_struct->play_queue->front)
			{
				//正在播放的已经是第一首了，再也上不去了
				printf("already first music\n");
				return -PLAY_l;
			}
			else
			{
				pthread_cancel(order_pthread->play_thread);
				order_struct->play_queue->playing_pointer -= 1;
			}
			
			pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
			order_pthread->play_thread = play_thread;
			
			return PLAY_l;
		}
	}
}

//显示本机中配置文件制定目录中的音乐
int play_go_d(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int all;
	all = display_music_menu(user_music);
	printf("there are(is) %d music(s) on your machine\n", all);
	
	return PLAY_d;
}

//用户给出两个数字或一个数字的时候，进行播放音乐或播放队列中的音乐
int play_go_n(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int pthread_test;
	pthread_t play_thread;	//播放音乐的线程
	
	if(order_struct->file_count == 0)
	{
		printf("nlack of para\n");
		return -PLAY_n;
	}
	
	//指定播放队列中指定的音乐，进行播放
	if(order_struct->file_count == 1)
	{
		//***************
		int jump_position ;	//用户指定的最新的播放队列中的音乐
		jump_position = atoi( strtok(order_struct->file_name , "#") );
		if(jump_position == 0)
		{
			printf("wrong para\n");
			return -PLAY_n;
		}
		if(jump_position < 1 || jump_position > order_struct->play_queue->music_count)
		{						
			printf("not in play queue\n");	//越界访问了
			return -PLAY_n;
		}
		
		if( jump_position + order_struct->play_queue->front - 1 == order_struct->play_queue->playing_pointer)
		{
			printf("this music is playing\n");
			return -PLAY_n;
		}
		//****************
		
		if(order_pthread->play_thread != 0)
		{
			pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
			if( pthread_test == ESRCH )
			{
				//播放进程没有在工作
				;
			}
			else
			{
				//播放进程在播放
				pthread_cancel(order_pthread->play_thread);
			}
		}
		else
		{
			//第一次执行到这里
			;
		}
		//调整播放指针
		//jump_position为用户指定的序号
		order_struct->play_queue->playing_pointer = jump_position + order_struct->play_queue->front - 1;
		pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
		order_pthread->play_thread = play_thread;
		
		return PLAY_n;
	}
	
	//两个数字
	//只需要前两个数字 后面的任何东西都忽略掉
	int dir_no;
	int music_no;
	
	char *dir_no_c = strtok(order_struct->file_name , "#");
	char *music_no_c = strtok(NULL , "#");
	
	dir_no = atoi(dir_no_c);
	music_no = atoi(music_no_c);
	
	if(dir_no == 0 || music_no == 0)
	{
		printf("wrong para\n");
		return -PLAY_n;
	}
	
	if(order_pthread->play_thread != 0)
	{
		pthread_test = pthread_kill(order_pthread->play_thread , 0);
		if( pthread_test == ESRCH )	//播放线程不存在
		{
			insert_random_music(order_struct->play_queue , user_music[dir_no].music_path , user_music[dir_no].music_name[music_no]);
			order_struct->play_queue->playing_pointer = order_struct->play_queue->rear - 1;
			pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
			order_pthread->play_thread = play_thread;
			
			return PLAY_n;
		}
		else
		{
			//播放线程正在播放音乐
			pthread_cancel(order_pthread->play_thread);
			insert_random_music(order_struct->play_queue , user_music[dir_no].music_path , user_music[dir_no].music_name[music_no]);
			order_struct->play_queue->playing_pointer = order_struct->play_queue->rear - 1;
			pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
			order_pthread->play_thread = play_thread;
			
			return PLAY_n;
		}
	}
	else
	{
		//第一次会执行到这里
		insert_random_music(order_struct->play_queue , user_music[dir_no].music_path , user_music[dir_no].music_name[music_no]);
		order_struct->play_queue->playing_pointer = order_struct->play_queue->rear - 1;
		pthread_create(&play_thread , NULL , go_music , (void *)order_struct );
		order_pthread->play_thread = play_thread;
// printf("play_thread:%d\n", play_thread);	
// pthread_detach(play_thread);
		
		return PLAY_n;
	}
	
}

//在播放队列中增加音乐（根据文件名增加）
int play_go_a(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	if(order_struct->file_count == 0)
	{
		printf("there is no music(s) you will add\n");
		return -PLAY_a;
	}

	insert_play_queue(order_struct->play_queue , order_struct , user_music);	//向播放队列中增加音乐
	
	//
	//if(PLAY_a PLAY_n)
	//后面跟上一对一对的数字 就可以添加了 还是根据play的d参数给出的数字进行添加
	
	return PLAY_a;

}

//在播放队列中增加音乐（根据-d参数中的两个数字进行增加）
int play_go_a_n(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	if(order_struct->file_count < 2)
	{
		printf("anlack of para\n");
		return -(PLAY_a | PLAY_n);
	}
	
	char *no_dir_c;
	char *no_music_c;
	int no_dir;
	int no_music;
	
	no_dir = atoi(strtok(order_struct->file_name , "#"));
	no_music = atoi(strtok(NULL , "#") );
	
	if(no_dir < 1 || no_dir > user_music[0].music_number || no_music < 1 || no_music > atoi(user_music[no_dir].music_name[0]))
	{
		printf("wrong para\n");			
	}
	else
	{
		insert_random_music(order_struct->play_queue , user_music[no_dir].music_path , user_music[no_dir].music_name[no_music] );
	}
	
	while(1)
	{
		no_dir_c = strtok(NULL , "#");
		if(no_dir_c == NULL)
			break;
		no_dir = atoi(no_dir_c);
		
		no_music_c = strtok(NULL , "#");
		if(no_music_c == NULL)
		{
			printf("anlack of para\n");
			break;
		}
		no_music = atoi(no_music_c);
		
		if(no_dir < 1 || no_dir > user_music[0].music_number || no_music < 1 || no_music > atoi(user_music[no_dir].music_name[0]))
		{
			printf("wrong para\n");			
		}
		else
		{
			insert_random_music(order_struct->play_queue , user_music[no_dir].music_path , user_music[no_dir].music_name[no_music] );
		}
	}
	
	return PLAY_a | PLAY_n;
}

//清空播放队列	如果后面带一些数字 删除队列中指定的音乐 但是不能删除正在播放的音乐
int play_go_c(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int pthread_test;
	pthread_t play_thread;	//播放音乐的线程
	
	if(order_struct->file_count == 0)
	{
		//没有携带数字，对整个播放队列进行操作
		if(order_pthread->play_thread == 0)
		{
			//全部清除
			clear_play_queue(order_struct->play_queue , -1);
		}
		else
		{
			pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
			if(pthread_test == ESRCH)
			{
				//全部清除
				clear_play_queue(order_struct->play_queue , -1);
			}
			else
			{
				//正在播放，不能清除正在播放的音乐
				clear_play_queue(order_struct->play_queue , -2);
			}
		}
	}
	else
	{
		//至少携带数字，删除指定的播放队列中的音乐
		int will_delete;
		char *will_delete_c;
		
		will_delete_c = strtok(order_struct->file_name , "#");
		will_delete = atoi(will_delete_c);
		if(will_delete < 1 || will_delete > order_struct->play_queue->music_count)
		{
			printf("wrong para\n");
		}
		else
		{
			if(order_pthread->play_thread == 0)
				clear_play_queue(order_struct->play_queue , will_delete + order_struct->play_queue->front - 1);
			else
			{
				pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
				if(pthread_test == ESRCH)
					clear_play_queue(order_struct->play_queue , will_delete + order_struct->play_queue->front - 1);
				else
				{
					//不能清除正在播放的音乐
					if(will_delete + order_struct->play_queue->front - 1 == order_struct->play_queue->playing_pointer)
						printf("[%d] is playing\n", will_delete);
					else
						clear_play_queue(order_struct->play_queue , will_delete + order_struct->play_queue->front - 1);
				}
			}
		}
		
		while(1)
		{
			will_delete_c = strtok(NULL , "#");
			if(will_delete_c == NULL)
				break;
			
			will_delete = atoi(will_delete_c);
			if(will_delete < 1 || will_delete > order_struct->play_queue->music_count)
			{
				printf("wrong para\n");
			}
			else
			{
				if(order_pthread->play_thread == 0)
					clear_play_queue(order_struct->play_queue , will_delete + order_struct->play_queue->front - 1);
				else
				{
					pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
					if(pthread_test == ESRCH)
						clear_play_queue(order_struct->play_queue , will_delete + order_struct->play_queue->front - 1);
					else
					{
						//不能清除正在播放的音乐
						if(will_delete + order_struct->play_queue->front - 1 == order_struct->play_queue->playing_pointer)
							printf("[%d] is playing\n");
						else
							clear_play_queue(order_struct->play_queue , will_delete + order_struct->play_queue->front - 1);
					}
				}
			}
		}//end-while
		
	}
	
	return PLAY_c;
}

//改变循环状态和指定的单曲循环
int play_go_r(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{	
	if(order_struct->file_count == 0)
	{
		order_struct->play_queue->recycle_status *= -1;	//使用一次不带数字的r参数，改变一下循环与否
		if(order_struct->play_queue->recycle_status == RECYCLE_NO)
			printf("---------->now,no recycle\n");
		else
			if(order_struct->play_queue->recycle_status == RECYCLE_YES)
				printf("---------->now,recycle\n");
	}
	else
	{
		//设置指定的音乐是否循环播放
		order_struct->play_queue->music_recycle_status *= -1;
		if(order_struct->play_queue->music_recycle_status ==  RECYCLE_NO)
			printf("---------->one song recycle mode is off\n");
		else
			if(order_struct->play_queue->music_recycle_status ==  RECYCLE_YES)
				printf("---------->one song recycle mode is on\n");
	}
	
	return PLAY_r;
}

//改变播放的随机状态
int play_go_R(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	order_struct->play_queue->random_status *= -1;
	if(order_struct->play_queue->random_status ==  RECYCLE_NO)
		printf("---------->random playing mode is off\n");
	else
		if(order_struct->play_queue->random_status ==  RECYCLE_YES)
			printf("---------->random playing mode is on\n");
	
	return PLAY_R;
}

//添加一个目录（配置文件中的目录）中的全部音乐
int play_go_a_d(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	if(order_struct->file_count == 0)
	{
		printf("lack para\n");
		return -(PLAY_a | PLAY_d);
	}
	else
	{
		int no_dir_add;
		char *no_dir_add_c;
		
		no_dir_add_c = strtok(order_struct->file_name , "#");
		no_dir_add = atoi(no_dir_add_c);
		if(no_dir_add < 1 || no_dir_add > user_music[0].music_number)
			printf("wrong para \n");
		else
			add_dir_to_play_queue(order_struct->play_queue , user_music , no_dir_add);
		
		while(1)
		{
			no_dir_add_c = strtok(NULL , "#");
			if(no_dir_add_c == NULL)
				break;
			
			no_dir_add = atoi(no_dir_add_c);
			if(no_dir_add < 1 || no_dir_add > user_music[0].music_number)
				printf("wrong para \n");
			else
				add_dir_to_play_queue(order_struct->play_queue , user_music , no_dir_add);
			
		}
	}
	
	return PLAY_a | PLAY_d;
}

//添加一个目录（不是配置文件中的目录）中的全部音乐 需要指定特定的目录了
int play_go_p(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int all_music = 0;
	int v ;
	char *dir_name;
	
	if(order_struct->file_count == 0)
	{
		printf("lack of directory\n");
		return -PLAY_p;
	}
	
	dir_name = strtok(order_struct->file_name , "#");
	get_indicator_position(dir_name);	//统一 在目录名后面添加/
	v = add_out_dir_to_play_queue(order_struct->play_queue , dir_name);
	if(v == -1)
		printf("this directory is invalid\n");
	else
		all_music += v;
	
	//这里有问题 也不知为什么 后面的目录不能识别
	while(1)
	{
		dir_name = strtok(NULL , "#");
		if(dir_name == NULL)
			break;
		
		get_indicator_position(dir_name);
		v = add_out_dir_to_play_queue(order_struct->play_queue , dir_name);
		if(v == -1)
			printf("this directory is invalid\n");
		else
			all_music += v;
		
	}
	
	printf("you add %d music(s) to the play queue\n", all_music);
	
	return PLAY_p;
}

//暂停或者开始播放
int play_go_t(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	int pthread_test;
	pthread_t play_thread;	//播放音乐的线程
	
	if(order_pthread->play_thread == 0)
	{
		printf("there is no music is playing\n");
		return -PLAY_t;
	}
	else
	{
		pthread_test = pthread_kill(order_pthread->play_thread , 0) ;
		if(pthread_test == ESRCH)
		{
			//播放线程没有工作，说明没有音乐在播放
			printf("there is no music is playing\n");
			return -PLAY_t;
		}
		else
		{
			order_struct->play_queue->stop_playing *= -1;	//改变播放状态 暂停或者开始
		}
	}
	
	return PLAY_t;
}

//空实现
int play_go_x(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music)
{
	printf("you can modify the yff.conf on your machine,bye\n");
	return PLAY_x;
}