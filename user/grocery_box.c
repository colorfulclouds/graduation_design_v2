#ifndef GROCERY_BOX
#define GROCERY_BOX
#include "grocery_box.h"
#endif

void _flush()
{
	char ch;
	while((ch=getchar())!='\n'&&ch!=EOF);
}

void error_reason(int error_code , void * error_reason_buff)
{
	FILE *fp = fopen("reason.txt" , "r");
	
	char buff[MAX_LENGTH_ERROR_REASON_BUFF];
	char code[MAX_LENGTH_ERROR_CODE_BUFF];
	
	if(fp == NULL)
	{
		perror("*");
		exit(EXIT_FAILURE);
	}
	
	while( fgets(buff , MAX_LENGTH_ERROR_REASON_BUFF , fp) )
	{
		strncpy(code , buff , MAX_LENGTH_ERROR_CODE_BUFF - 1);
		if( atoi(code) == error_code)
		{
			strcpy(error_reason_buff , buff + MAX_LENGTH_ERROR_CODE_BUFF);
			break;
		}
	}
	
	fclose(fp);
}

void *test_server(void *arg)
{
	struct test_server_status temp;
	temp = *(struct test_server_status *)arg ;
	
	int client_sock_fd = temp.client_sock_fd;
	int result;
	char test_send_char = 'y';
	char test_recv_char;
	
	while(1)
	{
		send(client_sock_fd , (void *)&test_send_char , sizeof(char) , 0);
		result = recv(client_sock_fd , (void *)&test_recv_char , sizeof(char) , 0);
		if(result == 0)
		{
			*(temp.server_status) = SERVER_IS_DOWNTIME;
			break ;
		}
	}
	pthread_exit(NULL);
}

void start_detect_server(int client_sock_fd , int *server_status)
{
	struct test_server_status arg;
	pthread_t test_server_thread;
	
	arg.client_sock_fd = client_sock_fd;
	arg.server_status = server_status;
	
	pthread_create(&test_server_thread , NULL , test_server , (void *)&arg);
}

void create_yff_conf()
{
	FILE *fp;
	system("touch yff.conf");
	fp = fopen("yff.conf" , "w");
	fputc('\#' , fp);
	fclose(fp);
}

int update_user_music(struct music_dir *user_music)
{
	int i;
	FILE *fp = fopen("yff.conf" , "r");
	
	if(fp == NULL)
	{
		system("touch yff.conf");
		fp = fopen("yff.conf" , "w");
		fputc('\#' , fp);
		fclose(fp);
		fp = fopen("yff.conf" , "r");
	}
	
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
	return 1;
}

//刷新yff.conf文件 15s刷新一次
void *monitor_yff_conf(void *arg)
{
	if( pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL) != 0)
		printf("[25001][monitor] set cancel state error\n");
	
	if( pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL) != 0)
		printf("[25002][monitor] set cancel type error\n");
	
	struct music_dir *user_music = (struct music_dir *)arg;
	update_user_music(user_music);
	
	struct stat yff_conf;
	time_t original_modify_time;
	
	if( access("./yff.conf" , F_OK) != 0)
	{
		//yff.conf不存在 创建一个
		create_yff_conf();
	}
	stat("./yff.conf" , &yff_conf);
	original_modify_time = yff_conf.st_mtime;	//第一次获取文件的修改时间
	
	while(1)
	{
		sleep(UPDATE_YFF_CONF_F);	//15秒刷新一次
		
		if( access("./yff.conf" , F_OK) != 0)
		{
			//yff.conf不存在 创建一个
			create_yff_conf();
//			continue ;	//等下一次检测
		}
		
		stat("./yff.conf" , &yff_conf);
		
		if(yff_conf.st_mtime != original_modify_time)
		{
			original_modify_time = yff_conf.st_mtime;
			update_user_music(user_music);
		}
		
	}
	
	pthread_exit(NULL);
}

void store_tty(int flag)
{
	static struct termios tty_state;
	
	if( flag == 0)
		tcgetattr(0 , &tty_state);
	else
		tcsetattr(0 , TCSANOW , &tty_state);
	
}

void set_tty()
{
	struct termios tty_state;
	
	tcgetattr(0 , &tty_state);
	tty_state.c_lflag &= ~ICANON;
	tty_state.c_cc[VMIN] = 1;
	
	tcsetattr(0 , TCSANOW , &tty_state);
}



