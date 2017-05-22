#ifndef PLAY_QUEUE
#define PLAY_QUEUE
#include "play_queue.h"
#endif

int init_play_queue(struct play_music_queue *queue)
{
	queue->front = queue->rear = 0;
	queue->music_count = 0;
	queue->playing_pointer = -1;	//当前没有播放
	queue->recycle_status = RECYCLE_NO;	//默认不循环播放
	queue->music_recycle_status = RECYCLE_NO;	//默认不单曲循环任何一首歌
	queue->random_status = RECYCLE_NO;	//默认不随机播放
	queue->stop_playing = RECYCLE_NO;	//默认不暂停	RECYCLE_NO在这里表示不暂停 就是正常播放
}

int find_dir_in_conf(struct music_dir *user_music_t , char *music_name , char *dir_name , int *dir_count)
{
	int count = 0;	//用来记录一共找到几个相同名字音乐在不同目录的个数
	int in_dir_count = user_music_t[0].music_number;
	int dir_in_music_count;
	
	int i;
	int j;
	
	for(i = 1 ; i<=in_dir_count ; i ++)
	{
		dir_in_music_count = atoi(user_music_t[i].music_name[0]);
		for(j = 1 ; j<=dir_in_music_count ; j++)
		{
			if( strcmp(music_name , user_music_t[i].music_name[j]) == 0)
			{
				count ++;
				strcat(dir_name , user_music_t[i].music_path );
				strcat(dir_name , "#");
			}			
		}			
	}
	
	dir_name[strlen(dir_name)] = '\0';
	*dir_count = count;
	
	return 1;
}

int insert_random_music(struct play_music_queue *queue , const char *dicectory , const char *music)
{
	strcpy(queue->play_music[queue->rear].music_name , music );
	queue->play_music[queue->rear].music_pos = 'm';	//暂时先看作是在用户机器上
	strcpy(queue->play_music[queue->rear].music_directory , dicectory);
	queue->play_music[queue->rear].flag = 1;
	
	queue->rear ++;
	queue->music_count ++;

	return queue->rear - 1;	//返回插入的位置	
}

int insert_play_queue(struct play_music_queue *queue , struct music_shell *shell , struct music_dir *user_music)
{
	char *file_name[SHELL_MAX_MUSIC_COUNT];
	int file_name_i = 0;
	
	char *one_dir_name;	//一个目录长度
	
	char *dir_name = (char *)malloc(sizeof(char) * SHELL_MAX_MUSIC_COUNT * MUSIC_PATH_MAX_LENGTH);	//shell所携音乐最大数目*音乐文件路径最长 所以放在堆中
	int dir_count;
	
	
	int i;
	
	if(shell->file_count != 0)
	{
		file_name[file_name_i ++] = strtok(shell->file_name , "#");
		
		while(1)
		{
			file_name[file_name_i] = strtok(NULL , "#");
			if(file_name[file_name_i] == NULL)
				break;
			else
				file_name_i ++;
		}
	}
	else
	{
		free(dir_name);
		return -1;
	}		
	
	for(i = 0 ; i<file_name_i ; i ++)
	{
		bzero(dir_name , sizeof(dir_name) );
		find_dir_in_conf(user_music , file_name[i] , dir_name , &dir_count);
		
		if(dir_count == 0)
		{
			printf("%s is not exist\n", file_name[i]);
			continue ;
		}
		else
		{
			one_dir_name = strtok(dir_name , "#");
			
			strcpy(queue->play_music[queue->rear].music_name , file_name[i] );
			queue->play_music[queue->rear].music_pos = 'm';	//暂时先看作是在用户机器上
			strcpy(queue->play_music[queue->rear].music_directory , one_dir_name);
			queue->play_music[queue->rear].flag = 1;
			
			queue->rear ++;
			queue->music_count ++;
			
			while(1)
			{
				one_dir_name = strtok(NULL , "#");
				if(one_dir_name == NULL)
					break;
				else
				{
					strcpy(queue->play_music[queue->rear].music_name , file_name[i] );
					queue->play_music[queue->rear].music_pos = 'm';	//暂时先看作是在用户机器上
					strcpy(queue->play_music[queue->rear].music_directory , one_dir_name);
					queue->play_music[queue->rear].flag = 1;
					
					queue->rear ++;
					queue->music_count ++;
				}
			}
		}
	}
	
	free(dir_name);
	return 1;
}

//播放队列队首的音乐播放完毕
int remove_play_queue(struct play_music_queue *queue , struct music_shell *shell)
{
	queue->front ++;
	queue->music_count --;
}
//播放队列中的音乐播放完毕删除
int delete_play_queue(struct play_music_queue *queue)
{
	int i;
	
	for(i = queue->playing_pointer ; i<queue->rear - 1 ; i ++)
	{
		queue->play_music[i] = queue->play_music[i+1];
	}
	
	queue->rear --;
	queue->music_count --;
	
	return 1;
}

int print_queue(struct play_music_queue *queue)
{
	int counter = 1;
	int i;
	
	if(queue->music_count != 0)
		printf("\tdirectory\t\tposition\tmusic_name\n");
	
	for(i = queue->front ; i<queue->rear ; i ++)
	{
		if(queue->playing_pointer == i)
			printf("->");
		else
			printf("  ");
		printf("[%d] %s\t%c\t%s\n", counter ++ , queue->play_music[i].music_directory , queue->play_music[i].music_pos , queue->play_music[i].music_name );
	}
	
	return queue->music_count;
}

int clear_play_queue(struct play_music_queue *queue , int flag)
{
	int v;
	
	if(flag == -1)
	{
		//全部清除
		v = queue->music_count;
		queue->front = queue->rear = 0;
		queue->music_count = 0;
//		queue->playing_pointer = -1;	//这句是没有必要的
		return v;
	}
	else if(flag == -2)
		{
			//除过正在播放的全部清除
			v = queue->music_count - 1;
			queue->front = queue->playing_pointer;
			queue->rear = queue->playing_pointer + 1;
			queue->music_count = 1;
			return v;
		}
		else
		{
			//flag就是指定的要删除的音乐
			int i;
			for(i = flag ; i<queue->rear - 1 ; i ++)
			{
				queue->play_music[i] = queue->play_music[i+1];
			}
			
			if(flag < queue->playing_pointer)
				queue->playing_pointer --;
	
			queue->rear --;
			queue->music_count --;
			
			return flag;
		}
}

int add_dir_to_play_queue(struct play_music_queue *queue , struct music_dir *user_music , int dir_no)
{
	int i;
	int dir_in_music_number = atoi(user_music[dir_no].music_name[0]);
	char *dir_name = user_music[dir_no].music_path;
	
	for(i = 1 ; i<= dir_in_music_number ; i ++)
		insert_random_music(queue , dir_name , user_music[dir_no].music_name[i]);
	
	return dir_in_music_number;
}

int add_out_dir_to_play_queue(struct play_music_queue *queue , char *dir_name)
{
	DIR *dp;
	struct dirent *entry;
	struct stat file_stat;
	
	char file_format[MUSIC_FORMAT_MAX_LENGTH];	//5
	char temp_path[MUSIC_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH];	//用来存储音乐文件暂时的路径	//160
	int temp_path_length;
	int all_music = 0;
//puts(dir_name);	
	dp = opendir(dir_name);
	if(dp == NULL)
	{
		return -1;
	}
	
	strcpy(temp_path , dir_name);
	temp_path_length = strlen(temp_path);
	
	while(entry = readdir(dp))
	{
		if( strcmp(entry->d_name , ".") == 0 || strcmp(entry->d_name , "..") == 0)
			continue ;
		
		bzero(file_format , sizeof(file_format) );
		strcpy(temp_path + temp_path_length , entry->d_name);
		stat(temp_path , &file_stat);
		if( S_ISREG(file_stat.st_mode) )	//常规文件
		{
			if( get_format_position(entry->d_name , file_format) == -1)	//if-else
				continue ;
			else
				if( is_music(file_format) == 1)	//if-else
				{
					insert_random_music(queue , dir_name , entry->d_name);
					all_music ++;
				}
		}
		
	}
	
	closedir(dp);
	return all_music;
}

//下面的是在线试听的音乐所在队列中
int insert_play_queue_o(struct play_music_queue *queue , char *music_name , int music_id)
{
	strcpy(queue->play_music[queue->rear].music_name , music_name );
	queue->play_music[queue->rear].music_pos = 's';
	strcpy(queue->play_music[queue->rear].music_directory , "*");
	queue->play_music[queue->rear].flag = 3;
	
	queue->play_music[queue->rear].music_id = music_id;
	
	queue->rear ++;
	queue->music_count ++;

	return queue->rear - 1;	//返回插入的位置
}