//PLAY_QUEUE
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

struct play_music_t
{
	int flag;									// 1为本机音乐 3位在在线试听音乐 (2暂时没用)
	
	char music_name[MUSIC_NAME_MAX_LENGTH];		//音乐名称
	char music_pos;								//音乐位置（m:本机 s:服务器）
	char music_directory[MUSIC_PATH_MAX_LENGTH];//音乐所在目录
	
	int music_id;								//服务器使用的music_id
};

struct play_music_queue
{
	struct play_music_t play_music[PLAY_QUEUE_MAX_LENGTH];	//500
	
	int front;
	int rear;
	int music_count;			//播放队列中音乐数量
	int playing_pointer;		//指向当前正在播放的歌曲
	int recycle_status;			//是否循环播放（默认是不循环，到队尾就停止）
	int music_recycle_status;	//队列中音乐播放的循环性（单曲循环）
	int random_status;			//播放音乐的随机性（默认不随机）
	int stop_playing;			//暂停播放或者继续播放（默认是继续播放）
};

int find_dir_in_conf(struct music_dir *user_music_t , char *music_name, char *dir_name , int *dir_count);	//根据音乐的名称在用户的音乐中查找，凡是找到的都加到播放队列中

int insert_random_music(struct play_music_queue *queue , const char *dicectory , const char *music);
int insert_play_queue(struct play_music_queue *queue , struct music_shell *shell , struct music_dir *user_music);	//向播放队列中添加音乐
int remove_play_queue(struct play_music_queue *queue , struct music_shell *shell);	//将播放完毕的音乐去除
int init_play_queue(struct play_music_queue *queue);	//初始化播放队列
int print_queue(struct play_music_queue *queue);
int clear_play_queue(struct play_music_queue *queue , int flag);	//清除播放队列

int add_dir_to_play_queue(struct play_music_queue *queue , struct music_dir *user_music , int dir_no);	//将指定目录下的音乐全部添加到播放队列中
int add_out_dir_to_play_queue(struct play_music_queue *queue , char *dir_name);	//向播放队列中添加不在配置文件中的目录

//在线试听
int insert_play_queue_o(struct play_music_queue *queue , char *music_name , int music_id);