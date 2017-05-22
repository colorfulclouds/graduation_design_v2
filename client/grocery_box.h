//GROCERY_BOX	//process kinds of little questions

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

struct test_server_status
{
	int client_sock_fd;
	int *server_status;
};

void _flush();

void error_reason(int error_code , void * error_reason_buff);

void *test_server(void *arg);

void start_detect_server(int client_sock_fd , int *server_status);

void *monitor_yff_conf(void *arg);	//一个线程一直监视配置文件中的变化
void create_yff_conf();		//yff.conf不存在 创建一个
int update_user_music(struct music_dir *user_music );			//yff.conf已经发生变化，现在对user music进行更新

//5.15追加 命令选择 不必等待回车确认
void store_tty(int flag);	//保存终端模式
void set_tty();				//设置模式

