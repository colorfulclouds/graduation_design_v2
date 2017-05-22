//FRIEND_GO
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

#ifndef DOWNLOAD_QUEUE
#define DOWNLOAD_QUEUE
#include "download_queue.h"
#endif

#ifndef RECVFROM_SERVER
#define RECVFROM_SERVER
#include "recvfrom_server.h"
#endif

int friend_shell(struct pthread_number *order_pthread , struct music_shell *order_struct);

int friend_shell_a(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户添加好友

int friend_shell_d(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户删除好友

int friend_shell_n(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户用name查找用户

int friend_shell_l(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户查看好友收藏的音乐

int friend_shell_c(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户拷贝好友的收藏列表

int friend_shell_u(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户查看好友上传的音乐

int friend_shell_s(struct pthread_number *order_pthread , struct music_shell *order_struct);	//用户下载好友上传的音乐

int friend_shell_t(struct pthread_number *order_pthread , struct music_shell *order_struct);	//查看添加自己的用户或同意添加

int friend_shell_t_l(struct pthread_number *order_pthread , struct music_shell *order_struct);	//查看添加自己的用户

int friend_shell_t_a(struct pthread_number *order_pthread , struct music_shell *order_struct);	//同意添加