//RECVFROM_SERVER

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef SEND_RECV
#define SEND_RECV
#include "send_recv.h"
#endif

#ifndef PLAY_QUEUE
#define PLAY_QUEUE
#include "play_queue.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

int recv_check_result(int client_sock_fd , char *check_result);

int recv_user_info(int client_sock_fd , struct user *one);

int recv_check_email_result(int client_sock_fd , struct user *one , char *result);

int recv_top_music(int client_sock_fd);		//从服务器获取TOP10

void *get_buff_music(void *arg);			//从服务器接收缓冲音乐 与服务器同步相关音乐信息

int recv_music_buff_from_s(int download_sock_fd , struct download_queue *queue_download , int pos);	//开始接收音乐文件

void *recv_cache_music(void *arg);			//接收在线试听音乐文件

int recv_style_music(int client_sock_fd , int music_style);	//根据用户输入的style_id查看相关音乐
