//RECVFROM_CLIENT
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

int recv_mode(int client_sock_fd , char *choice);

int recv_id_password(int client_sock_fd , struct user *one);

int recv_user_info(int client_sock_fd , struct user *one);

int recv_user_request(int client_sock_fd , int *user_request);	//接收用户的服务请求

void *recv_upload_music(void *arg);	//接收用户上传的音乐

int recv_upload_music_true(int upload_sock_fd , char *user_upload_dir , char *music_name);	//真正接收用户上传音乐数据