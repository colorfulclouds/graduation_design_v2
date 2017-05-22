//SENDTO_SERVER
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

#ifndef UPLOAD_QUEUE
#define UPLOAD_QUEUE
#include "upload_queue.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

int send_order(int client_sock_fd , char choice);

int send_id_password(int client_sock_fd , struct user *one);

int send_user_info(int client_sock_fd , struct user *one);

int send_user_request(int client_sock_fd , int user_request);	//向服务器请求服务（TOP10）

void *send_upload_music(void *arg);	//向服务器上传音乐线程

int send_upload_music_to_s(int upload_sock_fd , struct upload_queue *queue_upload , int pos);	//向服务器上传音乐的真实过程