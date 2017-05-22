//HANDLE
//专门处理客户端向服务器请求的关于音乐的事务
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef SENDTO_CLIENT
#define SENDTO_CLIENT
#include "sendto_client.h"
#endif

#ifndef RECVFROM_CLIENT
#define RECVFROM_CLIENT
#include "recvfrom_client.h"
#endif

#ifndef DATABASE
#define DATABASE
#include "database.h"
#endif

#ifndef WAV_PARSER
#define WAV_PARSER
#include "wav_parser.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef FRIEND_HANDLE
#define FRIEND_HANDLE
#include "friend_handle.h"
#endif

struct pthread_number
{
	pthread_t topxx_thread;		//向用户发用TOPxx的线程（未使用）
	pthread_t online_thread;	//向用户发送缓冲音乐的线程
	pthread_t download_thread;	//用户下载音乐的线程
	pthread_t upload_thread;	//用户上传音乐的线程
	
	struct pthread *socket_kit;	//需要使用的套接字集合
};

void init_affair_pthread(struct pthread_number *affair_pthread , struct pthread *socket_kit);

void cancel_thread_release_affair_pthread(struct pthread_number *affair_pthread);	//关闭线程 释放资源

//处理全部用户请求的分配函数
int handle_work(struct pthread *socket_kit);

//用户请求TOPxx
int handle_work_t(struct pthread_number *affair_pthread);
//用户请求播放音乐
int handle_work_s(struct pthread_number *affair_pthread);
//用户在线试听音乐（伪试听）
int handle_work_o(struct pthread_number *affair_pthread);
//用户在线试听音乐（真试听）
int handle_work_o_c(struct pthread_number *affair_pthread);
//用户请求以音乐风格查看
int handle_work_k(struct pthread_number *affair_pthread);
//用户收藏音乐
int handle_work_l(struct pthread_number *affair_pthread);
//用户利用音乐名字查找音乐
int handle_work_f(struct pthread_number *affair_pthread);
//用户查看收藏的音乐
int handle_work_l_d(struct pthread_number *affair_pthread);
//用户删除收藏的部分音乐
int handle_work_l_x(struct pthread_number *affair_pthread);
//用户查看曾经上传的音乐
int handle_work_l_u(struct pthread_number *affair_pthread);
//用户删除曾经上传的音乐
int handle_work_u_x(struct pthread_number *affair_pthread);
//用户下载曾经上传的音乐
int handle_work_u_s(struct pthread_number *affair_pthread);
//用户获取猜你喜欢列表
int handle_work_r(struct pthread_number *affair_pthread);
//用户修改个人信息
int handle_work_m(struct pthread_number *affair_pthread);
//用户评论广库音乐
int handle_work_c(struct pthread_number *affair_pthread);
//用户查看音乐评论
int handle_work_c_l(struct pthread_number *affair_pthread);