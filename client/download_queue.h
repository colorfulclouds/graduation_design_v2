//DOWNLOAD_QUEUE
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

struct download_music
{
	int music_id;							//服务器音乐id
	char music_name[MUSIC_NAME_MAX_LENGTH];	//音乐名称
	char music_dir[MUSIC_PATH_MAX_LENGTH];	//下载音乐所在目录
	char download_flag;						//未下载为n 下载中为d 下载完为y
	
	char user_id[MAX_USER_ID_LENGTH];		//6	//为了完成 从好友上传库中下载追加的成员 存储的是这个上传库音乐所归属的好友
};

struct download_queue
{
	struct download_music music_download[MAX_DOWNLOAD_COUNT];	//100
	int front;
	int rear;
	int count;				//即将下载的音乐数量
	int download_pointer;	//下载指针
};

void init_download_queue(struct download_queue *queue);

void insert_download_queue(struct download_queue *queue , int music_id , char *music_name , char *music_dir , char *user_id);

void delete_download_queue(struct download_queue *queue);

int display_download_queue(struct download_queue *queue);