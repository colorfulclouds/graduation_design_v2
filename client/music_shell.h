//MUSIC_SHELL
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

#ifndef PLAY_GO
#define PLAY_GO
#include "play_go.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef PLAY_QUEUE
#define PLAY_QUEUE
#include "play_queue.h"
#endif

#ifndef DOWNLOAD_QUEUE
#define DOWNLOAD_QUEUE
#include "download_queue.h"
#endif

#ifndef UPLOAD_QUEUE
#define UPLOAD_QUEUE
#include "upload_queue.h"
#endif

#ifndef FRIEND_GO
#define FRIEND_GO
#include "friend_go.h"
#endif

//完成一个简易的命令解释器，对用户输入的相关命令进行解释执行
//用户一旦登陆完毕，就立刻投入到这个命令解释器中

struct music_shell
{
	char order_name[SHELL_MAX_MUSIC_COUNT];
	int order_para;
	char file_name[SHELL_MAX_MUSIC_COUNT * SHELL_MAX_LENGTH];
	int file_count;							//命令后面跟的文件数量 有时也不是 是两个数字
	
	struct play_music_queue *play_queue;	//增加播放队列成员
	struct download_queue *queue_download;	//增加下载音乐队列成员
	struct upload_queue *queue_upload;		//增加上传音乐队列成员
	
	struct socket_t *do_socket;				//增加下载和试听套接字成员
};

struct pthread_number
{
	pthread_t play_thread;		//播放线程(used)
	pthread_t download_thread;	//下载线程(used)
	pthread_t online_thread;	//在线试听

	pthread_t updownload_thread;//上传线程(used)
	
	pthread_t comment_thread;	//评论线程（很可能没用）
};

void init_pthread_number(struct pthread_number *order_pthread);	//初始化各种应用的线程
void destroy_pthread_number(struct pthread_number *order_pthread);//销毁各种应用的线程

int op_shell(struct music_dir *user_music , struct socket_t *do_socket , int mode_flag);

int get_order_name_file(char *order , char *order_name , char *file_name[] , int *file_count);

void get_random_music(struct music_dir *user_music , char *random_music_dir , char *random_music_music);	//用户未指定音乐文件的时候，使用来产生随机音乐

int explain_shell(char *order_name , char *order_para , char *file_name , int file_count , struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music , int mode_flag);	//对命令进行解释执行

int order_para_str_to_int(char *order_para);		//play命令参数数字化

int plays_order_para_str_to_int(char *order_para , int mode_flag);	//plays命令参数数字化

int friend_order_para_str_to_int(char *order_para);	//friend命令参数数字化