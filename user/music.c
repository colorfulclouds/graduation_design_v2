#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

//处理本地音乐
ssize_t SNDWAV_P_SaveRead(int fd, void *buf, size_t count) 
{
    ssize_t result = 0, res; 
    while (count > 0) 
	{
        if ((res = read(fd, buf, count)) == 0) 
            break; 
        if (res < 0) 
            return result > 0 ? result : res; 
        count -= res; 
        result += res; 
        buf = (char *)buf + res; 
    } 
    return result; 
}

void SNDWAV_Play(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav, int fd , int *stop_playing) 
{ 
    int load, ret; 
    off64_t written = 0; 
    off64_t c; 
    off64_t count = LE_INT(wav->chunk.length); 

    load = 0; 
	
    while (written < count) 
	{ 
        /* Must read [chunk_bytes] bytes data enough. */ 
        do 
		{ 
            c = count - written; 
            if (c > sndpcm->chunk_bytes) 
                c = sndpcm->chunk_bytes; 
            c -= load; 
 
            if (c == 0) 
                break; 
            ret = SNDWAV_P_SaveRead(fd, sndpcm->data_buf + load, c); 
            if (ret < 0) 
			{ 
                fprintf(stderr, "Error safe_read/n"); 
                exit(-1); 
            } 
            if (ret == 0) 
                break;
            load += ret; 
        } while ((size_t)load < sndpcm->chunk_bytes); 
 
        /* Transfer to size frame */ 
        load = load * 8 / sndpcm->bits_per_frame;
		
		if( (*stop_playing) == RECYCLE_YES)
		{
			while(1)
			{
				if( (*stop_playing) == RECYCLE_NO)
				{
					ret = SNDWAV_WritePcm(sndpcm, load);
					break;
				}
				else
					continue ;
			}
		}
		else
			ret = SNDWAV_WritePcm(sndpcm, load); 	//发出声音的关键 向声卡写数据
		
        if (ret != load) 
			break; 
         
        ret = ret * sndpcm->bits_per_frame / 8; 
        written += ret; 
        load = 0; 
    }
}

void SNDWAV_Play_s(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav, int fd , int *stop_playing) 
{
    int load, ret; 
    off64_t written = 0; 
    off64_t c; 
    off64_t count = LE_INT(wav->chunk.length); 

	off64_t data_counter = 0;
	
    load = 0; 
	
    while (written < count) 
	{ 
        /* Must read [chunk_bytes] bytes data enough. */ 
        do 
		{ 
            c = count - written; 
            if (c > sndpcm->chunk_bytes) 
                c = sndpcm->chunk_bytes; 
            c -= load; 
 
            if (c == 0) 
                break; 
			
			data_counter += c;
			
            ret = SNDWAV_P_SaveRead(fd, sndpcm->data_buf + load, c); 
            if (ret < 0) 
			{ 
                fprintf(stderr, "Error safe_read/n"); 
                exit(-1); 
            } 
            if (ret == 0) 
                break;
            load += ret; 
        } while ((size_t)load < sndpcm->chunk_bytes); 

        load = load * 8 / sndpcm->bits_per_frame;
		
		if( (*stop_playing) == RECYCLE_YES)
		{
			while(1)
			{
				if( (*stop_playing) == RECYCLE_NO)
				{
					ret = SNDWAV_WritePcm(sndpcm, load);
					break;
				}
				else
					continue ;
			}
		}
		else
			ret = SNDWAV_WritePcm(sndpcm, load); 	//发出声音的关键 向声卡写数据
		
        if (ret != load) 
        {
			if(data_counter != count)
			{
				sleep(MAX_F_WAIT_MUSIC);
				continue ;
			}
			break; 
		}
         
        ret = ret * sndpcm->bits_per_frame / 8; 
        written += ret; 
        load = 0; 
    }

}

int play_music(void *arg , int *stop_playing)
{
	char *file_name = (char *)arg;
    char *default_device = "default"; 
    int music_fd;
    WAVContainer_t wav; 	//音频文件的属性
    SNDPCMContainer_t playback; 	//声卡硬件的属性

    memset(&playback, 0x0, sizeof(playback)); 	//清空声卡变量

    music_fd = open(file_name, O_RDONLY); 	//打开音乐文件
	
    if (music_fd < 0) 
	{ 
        printf("[26001][%s] is not exist");
        return -1; 
    } 

    if (WAV_ReadHeader(music_fd, &wav) < 0) 
	{ 
        fprintf(stderr, "Error WAV_Parse [%s]/n", file_name); 
        goto Err; 
    } 

    if (snd_output_stdio_attach(&playback.log, stderr, 0) < 0) 
	{
        fprintf(stderr, "Error snd_output_stdio_attach/n"); 
        goto Err; 
    } 
 
    if (snd_pcm_open(&playback.handle, default_device, SND_PCM_STREAM_PLAYBACK, 0) < 0) //打开音频设备
	{ 
        fprintf(stderr, "Error snd_pcm_open [ %s]/n", default_device); 
        goto Err; 
    } 

    if (SNDWAV_SetParams(&playback, &wav) < 0) 	//设置音频设备参数
	{ 
        fprintf(stderr, "Error set_snd_pcm_params/n"); 
        goto Err; 
    }
//puts(file_name);
    SNDWAV_Play(&playback, &wav, music_fd , stop_playing); 
 
    snd_pcm_drain(playback.handle); 
 
    close(music_fd); 
    free(playback.data_buf); 
    snd_output_close(playback.log); 
    snd_pcm_close(playback.handle); 
    return 0; 
 
Err: 
    close(music_fd); 
    if (playback.data_buf) 
		free(playback.data_buf); 
    if (playback.log) 
		snd_output_close(playback.log); 
    if (playback.handle) 
		snd_pcm_close(playback.handle); 
	
    return -1; 
}

int play_music_s(int online_sock_fd , int music_id ,int *stop_playing , struct music_shell *order_struct)
{
    char *default_device = "default"; 
 
    WAVContainer_t wav; 	//音频文件的属性
    SNDPCMContainer_t playback; 	//声卡硬件的属性

    memset(&playback, 0x0, sizeof(playback)); 	//清空声卡变量
		
	if( _send(online_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		printf("[send music_id]server is off\n");
		return -1;
	}
	pthread_t cache_t;
	pthread_create( &cache_t , NULL , recv_cache_music , (void *)order_struct);
	
	sleep(MAX_F_WAIT_MUSIC);	//缓冲时间10s
	
	int music_fd = open("/tmp/cache.temp" , O_RDONLY);
	
	if (WAV_ReadHeader(music_fd, &wav) < 0) 
	{ 
        fprintf(stderr, "Error WAV_Parse [%s]/n", "ds"); 
        goto Err; 
    } 
//暂时
    if (snd_output_stdio_attach(&playback.log, stderr, 0) < 0) {
        fprintf(stderr, "Error snd_output_stdio_attach/n"); 
        goto Err; 
    } 
 
    if (snd_pcm_open(&playback.handle, default_device, SND_PCM_STREAM_PLAYBACK, 0) < 0) //打开音频设备
	{ 
        fprintf(stderr, "Error snd_pcm_open [ %s]/n", default_device); 
        goto Err; 
    } 

    if (SNDWAV_SetParams(&playback, &wav) < 0) 	//设置音频设备参数
	{ 
        fprintf(stderr, "Error set_snd_pcm_params/n"); 
        goto Err; 
    }
	
	SNDWAV_Play_s(&playback, &wav, music_fd , stop_playing); 
 
    snd_pcm_drain(playback.handle); 
 
    free(playback.data_buf); 
    snd_output_close(playback.log); 
    snd_pcm_close(playback.handle); 
    return 0; 
 
Err: 
    if (playback.data_buf) 
		free(playback.data_buf); 
    if (playback.log) 
		snd_output_close(playback.log); 
    if (playback.handle) 
		snd_pcm_close(playback.handle); 
	
    return -1; 
}

void random_go_music(struct play_music_queue *play_queue , int online_sock_fd , int client_sock_fd , struct music_shell *order_struct)
{
	int random_play;	//供随机播放模式使用
	srand((unsigned) time(NULL));
	char music_file[CONF_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH];
	
	int new_online_music_flag = -PLAYS_o;
	int pthread_test;
	
	while(1)
	{
		if(play_queue->random_status ==  RECYCLE_YES)
		{
			play_queue->playing_pointer = rand() % play_queue->music_count + 1;
			strcpy(music_file , play_queue->play_music[play_queue->playing_pointer].music_directory);
			strcat(music_file , play_queue->play_music[play_queue->playing_pointer].music_name);
			
			if(play_queue->play_music[play_queue->playing_pointer].flag == 1)
			{
				switch(get_music_format(play_queue->play_music[play_queue->playing_pointer].music_name))
				{
					case FORMAT_WAV:
						;	//空实现
						break;
						
					case FORMAT_MP3:
						mp3_to_wav(music_file);
						break;
						
					default:
						;	//impossible
						break;
				}
				play_music(music_file , &(play_queue->stop_playing));
			}
			else
			{
				if( _send(client_sock_fd , (void *)&new_online_music_flag , -1 , 2) == -1)
				{
					break;
				}
				play_music_s(online_sock_fd , play_queue->play_music[play_queue->playing_pointer].music_id , &(play_queue->stop_playing) , order_struct);	//在线试听的音乐 order_struct->do_socket->online_sock_fd 需要用这个套接字 进行在线试听
			}
		}
		else
			break;
	}
}

void *go_music(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001]set cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002]set cancel type error\n");

	struct music_shell *order_struct = (struct music_shell *)arg;
	
	struct play_music_queue *play_queue = order_struct->play_queue;

	char music_file[CONF_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH];

	int pthread_test;
	int new_online_music_flag = -PLAYS_o;
	
	if( play_queue->music_count != 0)
	{		
		for( ; play_queue->playing_pointer < play_queue->rear ; play_queue->playing_pointer ++)
		{
			strcpy(music_file , play_queue->play_music[play_queue->playing_pointer].music_directory);
			strcat(music_file , play_queue->play_music[play_queue->playing_pointer].music_name);
			do
			{
				if(play_queue->play_music[play_queue->playing_pointer].flag == 1)
				{
					switch(get_music_format(play_queue->play_music[play_queue->playing_pointer].music_name))
					{
						case FORMAT_WAV:
							;	//空实现
							break;
							
						case FORMAT_MP3:
							mp3_to_wav(music_file);
							break;
							
						default:
							;	//impossible
							break;
					}
					play_music(music_file , &(play_queue->stop_playing));
				}
				else
				{
					if( _send(order_struct->do_socket->client_sock_fd , (void *)&new_online_music_flag , -1 , 2) == -1)
						continue ;
					play_music_s(order_struct->do_socket->online_sock_fd , play_queue->play_music[play_queue->playing_pointer].music_id , &(play_queue->stop_playing) ,  order_struct);	//在线试听的音乐
				}
				if(play_queue->random_status == RECYCLE_YES)
					random_go_music(play_queue , order_struct->do_socket->online_sock_fd , order_struct->do_socket->client_sock_fd , order_struct);
								
			}while(play_queue->music_recycle_status == RECYCLE_YES);
		}
		
		while(1)
		{
			if(play_queue->recycle_status == RECYCLE_YES)
			{
				for(play_queue->playing_pointer = play_queue->front ; play_queue->playing_pointer < play_queue->rear ; play_queue->playing_pointer ++)
				{
					strcpy(music_file , play_queue->play_music[play_queue->playing_pointer].music_directory);
					strcat(music_file , play_queue->play_music[play_queue->playing_pointer].music_name);
					do
					{
						if(play_queue->play_music[play_queue->playing_pointer].flag == 1)
						{
							switch(get_music_format(play_queue->play_music[play_queue->playing_pointer].music_name))
							{
								case FORMAT_WAV:
									;	//空实现
									break;
									
								case FORMAT_MP3:
									mp3_to_wav(music_file);
									break;
									
								default:
									;	//impossible
									break;
							}
							play_music(music_file , &(play_queue->stop_playing));
						}
						else
						{
							if( _send(order_struct->do_socket->client_sock_fd , (void *)&new_online_music_flag , -1 , 2) == -1)
								continue ;
							play_music_s(order_struct->do_socket->online_sock_fd , play_queue->play_music[play_queue->playing_pointer].music_id , &(play_queue->stop_playing) , order_struct );	//在线试听的音乐
						}
						if(play_queue->random_status ==  RECYCLE_YES)
							random_go_music(play_queue , order_struct->do_socket->online_sock_fd , order_struct->do_socket->client_sock_fd , order_struct );
						
					}while(play_queue->music_recycle_status == RECYCLE_YES);
				}
			}
			else
				break;
		}
		
		play_queue->playing_pointer = -1;	//队列中的音乐播放到队列尾，恢复 正在播放指针
	}	
	
	pthread_exit(NULL);	//音乐播放完毕，结束掉播放线程
}

int is_music(char *file_format)
{
	int i;
	char music_format[MUSIC_FORMAT_MAX_COUNT][MUSIC_FORMAT_MAX_LENGTH]={"wav","mp3","flac","ape"};
	
	for(i=0 ; i < MUSIC_FORMAT_MAX_COUNT ; i++)
	{
		if( strcmp(file_format , music_format[i]) == 0)
			return 1;
	}
	
	return -1;
}

int get_format_position(char *file_name , char *file_format)	//从字符串的后面向前定位出音乐文件格式前面的小数点. 以从后向前第一次出现的小数点为准，如果一直没有小数点，那就认为这不是音乐文件
{
	int length = strlen(file_name);
	int length_format;
	
	length --;
	
	while(length >= 0 && file_name[length] != '.' )
	{
		length --;
	}
	
	if(length == strlen(file_name))
		return -1;
	
	if(length == -1)
		return -1;
	
	length_format = strlen(file_name + length + 1);
	strcpy(file_format , file_name + length + 1);
	
	file_format[length_format] = '\0';
	
	return length;	//返回音乐格式的限位点位置
}

int display_conf_dir_music(char *dir_name)
{
	DIR *dp;
	struct dirent *entry;
	struct stat file_stat;
	char file_format[MUSIC_FORMAT_MAX_LENGTH];
	char temp_path[MUSIC_PATH_MAX_LENGTH];	//用来存储音乐文件暂时的路径
	int temp_path_length;

	strcpy(temp_path , dir_name);
	temp_path_length = strlen(temp_path);
	
	dp = opendir(dir_name);
	if(dp == NULL)
	{
		perror("error : ");
		return -1;
	}
	
	while(entry = readdir(dp))
	{
		if( strcmp(entry->d_name , ".") == 0 || strcmp(entry->d_name , "..") == 0)
			continue ;
		
		bzero(file_format , sizeof(file_format) );
		
		strcpy(temp_path + temp_path_length , entry->d_name);	
		stat(temp_path , &file_stat);
		if( S_ISREG(file_stat.st_mode) )	//这个文件是个常规文件，音乐文件也只可能是常规文件
		{
			if( get_format_position(entry->d_name , file_format) == -1)	//if-else
				continue ;
			else
				if( is_music(file_format) == 1)	//if-else
					printf("\t%s\n", entry->d_name);
		}
		//递归判断
//		else if( S_ISDIR(file_stat.st_mode) ) 
//				display_conf_dir_music(temp_path);
		
	}
	closedir(dp);
	return 1;
}

void get_indicator_position(char *dir_name)	//不管从conf取出来的目录是否具有最终的\ 都加上一个\ 统一化处理
{
	int length = strlen(dir_name);
	length --;
	
	while(isalpha(dir_name[length]) == 0)
		length --;
	
	if(dir_name[length + 1] == '/')
		dir_name[length + 2] = '\0';
	else
	{
		dir_name[length + 1] = '/';
		dir_name[length + 2] = '\0';
	}
}

//将用户配置文件中的音乐目录中的音乐显示出来
int display_music_menu(struct music_dir * user_music)
{
	int i;
	int j;
	int all = 0;
	
	int dir_counter;
	int music_counter;
	
	dir_counter = user_music[0].music_number;
	if(dir_counter == 0)
		return 0;
	
	for(i = 1 ; i<=dir_counter ; i ++)
	{
		printf("%d : %s\n", i , user_music[i].music_path );
		
		music_counter = atoi(user_music[i].music_name[0]);
		all += music_counter;
		
		for(j = 1 ; j<=music_counter ; j ++)
			printf("\t\t %d : %s\n", j , user_music[i].music_name[j]);
	}
	
	return all;
}

int get_music_from_dir(struct music_dir * user_music)
{
	int i = 1; 	//留下来的[0] 用来保存，这个直接目录中实际存了多少首音乐
	char *dir_name = user_music->music_path;
	
	DIR *dp;
	struct dirent *entry;
	struct stat file_stat;
	char file_format[MUSIC_FORMAT_MAX_LENGTH];
	char temp_path[MUSIC_PATH_MAX_LENGTH];	//用来存储音乐文件暂时的路径
	int temp_path_length;

	strcpy(temp_path , dir_name);
	temp_path_length = strlen(temp_path);
	
	dp = opendir(dir_name);
//printf("dir name :%s\n", dir_name);	
	if(dp == NULL)
	{
		perror("error");
		return -1;
	}

	while(entry = readdir(dp))
	{
		if( strcmp(entry->d_name , ".") == 0 || strcmp(entry->d_name , "..") == 0)
			continue ;
		
		bzero(file_format , sizeof(file_format) );
		
		strcpy(temp_path + temp_path_length , entry->d_name);
		stat(temp_path , &file_stat);
		if( S_ISREG(file_stat.st_mode) )	//这个文件是个常规文件，音乐文件也只可能是常规文件
		{
			if( get_format_position(entry->d_name , file_format) == -1)	//if-else
				continue ;
			else
				if( is_music(file_format) == 1)	//if-else
				{
					strcpy(user_music->music_name[i++] , entry->d_name);
//					printf("\t%s\n", entry->d_name);
				}
		}
		//递归判断
//		else if( S_ISDIR(file_stat.st_mode) ) 
//				display_conf_dir_music(temp_path);
		
	}
	closedir(dp);
	//类似实现了一个itoa()
	char real_music_count[MAX_MUSIC_COUNT_BIT];
	bzero(real_music_count , sizeof(real_music_count) );
	sprintf(real_music_count , "%d" , i-1);
	
	strcpy(user_music->music_name[0] , real_music_count);	//将实际的一个直接目录中的歌曲数量保存到哨兵位置
	
	return 1;
}

//这个函数 暂时没有用
//已经用一个线程在监视了
void init_user_music(struct music_dir * user_music)
{
	FILE *fp = fopen("yff.conf" , "r");
	if(fp == NULL)
	{
		system("touch yff.conf");
		fp = fopen("yff.conf" , "w");
		fputc('\#' , fp);
		fclose(fp);
		fp = fopen("yff.conf" , "r");
	}
	
	int i;
	
	//i=0 留下来存储用户机器上一共多少个音乐直接目录
	//user_music[0].music_number 还没有使用，那就用来存储用户的机器上有多少个直接的音乐目录 哨兵起来
	for(i = 1 ; i<MUSIC_DIRECT_DIR_MAX_COUNT ; i ++)
		user_music[i].music_number = i;

	i = 1;
	while(fgets(user_music[i].music_path , CONF_PATH_MAX_LENGTH , fp))
	{
		if(user_music[i].music_path[0] != '#')
		{
			get_indicator_position(user_music[i].music_path);
			
			if( get_music_from_dir( &(user_music[i]) ) == -1)	//get_music_from_dir( (user_music + sizeof(struct music_dir) * i);
				continue ;	//未知错误
			else
				i ++;
		}
		else
		{
			user_music[0].music_number = i-1 ;	//最后一个i肯定是‘#’
			break ;	//#为用户指定音乐目录的终结符
		}
	}
	
	fclose(fp);
}

//**************************
//**************************
int SNDWAV_P_GetFormat(WAVContainer_t *wav, snd_pcm_format_t *snd_format) 
{    
    if (LE_SHORT(wav->format.format) != WAV_FMT_PCM) 
        return -1; 
     
    switch (LE_SHORT(wav->format.sample_length)) 
	{ 
		case 16: 
			*snd_format = SND_PCM_FORMAT_S16_LE; 
			break; 
		case 8: 
			*snd_format = SND_PCM_FORMAT_U8; 
			break; 
		default: 
			*snd_format = SND_PCM_FORMAT_UNKNOWN; 
			break; 
    } 
 
    return 0; 
} 

ssize_t SNDWAV_WritePcm(SNDPCMContainer_t *sndpcm, size_t wcount) 
{ 
    ssize_t r; 
    ssize_t result = 0; 
    uint8_t *data = sndpcm->data_buf; 
 
    if (wcount < sndpcm->chunk_size) { 
        snd_pcm_format_set_silence(sndpcm->format,  
            data + wcount * sndpcm->bits_per_frame / 8,  
            (sndpcm->chunk_size - wcount) * sndpcm->channels); 
        wcount = sndpcm->chunk_size; 
    } 
    while (wcount > 0) 
	{ 
        r = snd_pcm_writei(sndpcm->handle, data, wcount); 
        if (r == -EAGAIN || (r >= 0 && (size_t)r < wcount)) 
		{ 
			snd_pcm_wait(sndpcm->handle, 1000); 
        } else if (r == -EPIPE) 
		{ 
			snd_pcm_prepare(sndpcm->handle); 
//			fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>/n"); 
        } else if (r == -ESTRPIPE) 
		{             
//			fprintf(stderr, "<<<<<<<<<<<<<<< Need suspend >>>>>>>>>>>>>>>/n");       
        } else if (r < 0) 
		{ 
//			fprintf(stderr, "Error snd_pcm_writei: [%s]", snd_strerror(r)); 
			exit(EXIT_FAILURE); 
        } 
        if (r > 0) 
		{ 
            result += r; 
            wcount -= r; 
            data += r * sndpcm->bits_per_frame / 8; 
        } 
    } 
    return result; 
} 
 
int SNDWAV_SetParams(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav) 
{ 
    snd_pcm_hw_params_t *hwparams; 
    snd_pcm_format_t format; 
    uint32_t exact_rate; 
    uint32_t buffer_time, period_time; 
 
    /* Allocate the snd_pcm_hw_params_t structure on the stack. */ 
    snd_pcm_hw_params_alloca(&hwparams); 
     
    /* Init hwparams with full configuration space */ 
    if (snd_pcm_hw_params_any(sndpcm->handle, hwparams) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_any/n"); 
        goto ERR_SET_PARAMS; 
    } 
 
    if (snd_pcm_hw_params_set_access(sndpcm->handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_set_access/n"); 
        goto ERR_SET_PARAMS; 
    } 
 
    /* Set sample format */ 
    if (SNDWAV_P_GetFormat(wav, &format) < 0) 
	{ 
        fprintf(stderr, "Error get_snd_pcm_format/n"); 
        goto ERR_SET_PARAMS; 
    } 
    if (snd_pcm_hw_params_set_format(sndpcm->handle, hwparams, format) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_set_format/n"); 
        goto ERR_SET_PARAMS; 
    } 
    sndpcm->format = format; 
 
    /* Set number of channels */ 
    if (snd_pcm_hw_params_set_channels(sndpcm->handle, hwparams, LE_SHORT(wav->format.channels)) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_set_channels/n"); 
        goto ERR_SET_PARAMS; 
    } 
    sndpcm->channels = LE_SHORT(wav->format.channels); 
 
    /* Set sample rate. If the exact rate is not supported */ 
    /* by the hardware, use nearest possible rate.         */  
    exact_rate = LE_INT(wav->format.sample_rate); 
	if (snd_pcm_hw_params_set_rate_near(sndpcm->handle, hwparams, &exact_rate, 0) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_set_rate_near/n"); 
        goto ERR_SET_PARAMS; 
    } 
    if (LE_INT(wav->format.sample_rate) != exact_rate) 
	{ 
        fprintf(stderr, "The rate %d Hz is not supported by your hardware./n ==> Using %d Hz instead./n",LE_INT(wav->format.sample_rate), exact_rate); 
    } 
 
    if (snd_pcm_hw_params_get_buffer_time_max(hwparams, &buffer_time, 0) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_get_buffer_time_max/n"); 
        goto ERR_SET_PARAMS; 
    } 
    if (buffer_time > 500000) 
		buffer_time = 500000; 
	
    period_time = buffer_time / 4; 
 
    if (snd_pcm_hw_params_set_buffer_time_near(sndpcm->handle, hwparams, &buffer_time, 0) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_set_buffer_time_near/n"); 
        goto ERR_SET_PARAMS; 
    } 
 
    if (snd_pcm_hw_params_set_period_time_near(sndpcm->handle, hwparams, &period_time, 0) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params_set_period_time_near/n"); 
        goto ERR_SET_PARAMS; 
    } 
 
    /* Set hw params */ 
    if (snd_pcm_hw_params(sndpcm->handle, hwparams) < 0) 
	{ 
        fprintf(stderr, "Error snd_pcm_hw_params(handle, params)/n"); 
        goto ERR_SET_PARAMS; 
    } 
 
    snd_pcm_hw_params_get_period_size(hwparams, &sndpcm->chunk_size, 0);     
    snd_pcm_hw_params_get_buffer_size(hwparams, &sndpcm->buffer_size); 
    if (sndpcm->chunk_size == sndpcm->buffer_size) 
	{         
        fprintf(stderr, ("Can't use period equal to buffer size (%lu == %lu)/n"), sndpcm->chunk_size, sndpcm->buffer_size);      
        goto ERR_SET_PARAMS; 
    } 
 
    sndpcm->bits_per_sample = snd_pcm_format_physical_width(format); 
    sndpcm->bits_per_frame = sndpcm->bits_per_sample * LE_SHORT(wav->format.channels); 
     
    sndpcm->chunk_bytes = sndpcm->chunk_size * sndpcm->bits_per_frame / 8; 
 
    /* Allocate audio data buffer */ 
    sndpcm->data_buf = (uint8_t *)malloc(sndpcm->chunk_bytes); 
    if (!sndpcm->data_buf) 
	{ 
        fprintf(stderr, "Error malloc: [data_buf]/n"); 
        goto ERR_SET_PARAMS; 
    } 
 
    return 0; 
 
ERR_SET_PARAMS: 
    return -1; 
} 

//**********************
//**********************
int WAV_P_CheckValid(WAVContainer_t *container) 
{ 
    if (container->header.magic != WAV_RIFF || container->header.type != WAV_WAVE || 
        container->format.magic != WAV_FMT  || container->format.fmt_size != LE_INT(16) || 
		(container->format.channels != LE_SHORT(1) && container->format.channels != LE_SHORT(2)) || container->chunk.type != WAV_DATA) 
	{
 //       fprintf(stderr, "non standard wav file./n"); 
 //       return -1; 
    } 
 
    return 0; 
} 
 
int WAV_ReadHeader(int fd, WAVContainer_t *container) 
{ 
    assert((fd >=0) && container); 
 
	if( read(fd , &container->header , sizeof(container->header)) != sizeof(container->header) || read(fd,&container->format,sizeof(container->format))!=sizeof(container->format) || read(fd,&container->chunk,sizeof(container->chunk))!=sizeof(container->chunk))
	{
        fprintf(stderr, "Error WAV_ReadHeader/n"); 
        return -1; 
    } 
 
    if (WAV_P_CheckValid(container) < 0) 
        return -1; 
    return 0; 
}
//**********************
//**********************

int get_music_format(char *music_name)
{
	char file_format[MUSIC_FORMAT_MAX_LENGTH];	//5
	bzero(file_format , sizeof(file_format));
	
	get_format_position(music_name , file_format);
	
	int i;
	char music_format[MUSIC_FORMAT_MAX_COUNT][MUSIC_FORMAT_MAX_LENGTH]={"wav","mp3","flac","ape"};
	
	for(i=0 ; i < MUSIC_FORMAT_MAX_COUNT ; i ++)
	{
		if( strcmp(file_format , music_format[i]) == 0)
			return i;
	}
	
	return -1;	//impossible
	
}

void mp3_to_wav(char *music_file)
{
	char ffmpeg[MAX_FFMPEG_MAX_LENGTH];	//100
	bzero(ffmpeg , sizeof(ffmpeg));
	strcpy(ffmpeg , "../ffmpeg/mp3towav ");
	strcat(ffmpeg , music_file);
	strcat(ffmpeg , " 1 > /dev/null 2>&1");

	
//puts(ffmpeg);	
	system(ffmpeg);
	
	bzero(music_file , sizeof(music_file));
	strcpy(music_file , "/tmp/mp3towav.mp3.wav");
}