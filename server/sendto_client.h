//SENDTO_CLIENT

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

#ifndef WAV_PARSER
#define WAV_PARSER
#include "wav_parser.h"
#endif

#ifndef HANDLE
#define HANDLE
#include "handle.h"
#endif

#ifndef DATABASE
#define DATABASE
#include "database.h"
#endif

#ifndef SERVER
#define SERVER
#include "server.h"
#endif

int send_check_result(int client_sock_fd , char check_result);

int send_user_info(int client_sock_fd , struct user *one);

int send_user_id_reg_time(int client_sock_fd , struct user *one);

int send_music_id_name(int client_sock_fd , int music_id , char  *music_name);	//向用户发送TOPxx的音乐id和名字

//向用户下载音乐
void *buff_music(void *arg);	
int send_music(struct pthread_number *affair_pthread , int download_sock_fd , int music_id , MYSQL *connection , int music_library_flag , char *user_id);	//发送音乐文件

//用户在线试听音乐
void *online_music(void *arg);	
int trans_music_stream(int online_sock_fd , int music_id , MYSQL *connection , struct pthread_number *affair_pthread);	//发送音乐流
