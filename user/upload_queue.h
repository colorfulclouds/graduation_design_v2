//UPLOAD_QUEUE

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

struct upload_music
{
	char music_dir[MUSIC_PATH_MAX_LENGTH];
	char music_name[MUSIC_NAME_MAX_LENGTH];
	char music_dir_name[MUSIC_PATH_MAX_LENGTH + MUSIC_NAME_MAX_LENGTH];
	
	char upload_flag;	//n未上传 u正在上传中 y上传完毕 x音乐源文件出现问题
};

struct upload_queue
{
	struct upload_music music_upload[MAX_UPLOAD_COUNT];
	
	int front;
	int rear;
	int count;
	int upload_pointer;
};

void init_upload_queue(struct upload_queue *queue);

void insert_upload_queue(struct upload_queue *queue , char *music_dir_name);

void delete_upload_queue(struct upload_queue *queue);

int display_upload_queue(struct upload_queue *queue);

void divide_music_name(char *music_dir_name , char *music_dir , char *music_name);	//分离出音乐名称