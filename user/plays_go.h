//PLAYS_GO
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef SENDTO_SERVER
#define SENDTO_SERVER
#include "sendto_server.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

#ifndef PLAY_QUEUE
#define PLAY_QUEUE
#include "play_queue.h"
#endif

#ifndef RECVFROM_SERVER
#define RECVFROM_SERVER
#include "recvfrom_server.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

//指令参数分配参数
int plays_shell(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music );	

int plays_shell_t(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//获取TOPxx
int plays_shell_s(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//缓冲音乐（下载音乐）
int plays_shell_o(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//在线播放音乐
int plays_shell_d(struct download_queue *queue );														//显示下载队列中的音乐
int plays_shell_u(struct pthread_number *order_pthread , struct music_shell *order_struct);				//用户上传音乐
int plays_shell_u_d(struct upload_queue *queue);														//用户查看上传队列
int plays_shell_k(struct pthread_number *order_pthread , struct music_shell *order_struct);				//查看不同风格的音乐
int plays_shell_l(struct pthread_number *order_pthread , struct music_shell *order_struct);				//收藏音乐（by music_id）
int plays_shell_f(struct pthread_number *order_pthread , struct music_shell *order_struct);				//利用音乐名字搜索音乐
int plays_shell_l_d(struct pthread_number *order_pthread , struct music_shell *order_struct);			//查看收藏的音乐
int plays_shell_l_x(struct pthread_number *order_pthread , struct music_shell *order_struct);			//删除收藏的音乐
int plays_shell_l_u(struct pthread_number *order_pthread , struct music_shell *order_struct);			//查看曾经上传的音乐
int plays_shell_u_x(struct pthread_number *order_pthread , struct music_shell *order_struct);			//删除曾经上传的音乐
int plays_shell_u_s(struct pthread_number *order_pthread , struct music_shell *order_struct);			//用户下载自己上传的音乐
int plays_shell_r(struct pthread_number *order_pthread , struct music_shell *order_struct);				//猜你喜欢的前5个
int plays_shell_m(struct pthread_number *order_pthread , struct music_shell *order_struct);				//用户修改个人信息
int plays_shell_c(struct pthread_number *order_pthread , struct music_shell *order_struct);				//用户评论音乐
int plays_shell_i(struct pthread_number *order_pthread , struct music_shell *order_struct);				//用户查看自己的信息
int plays_shell_c_l(struct pthread_number *order_pthread , struct music_shell *order_struct);			//查看一首音乐的评论