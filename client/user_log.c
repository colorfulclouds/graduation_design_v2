#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef LOG_DISPLAY
#define LOG_DISPLAY
#include "log_display.h"
#endif

#ifndef CONNECT_SERVER
#define CONNECT_SERVER
#include "connect_server.h"
#endif

#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

void ctrl_z(int sig)
{
	//待定
}

int main(void)
{
	signal(SIGPIPE , SIG_IGN);	//防止程序退出 忽略此信号
	signal(SIGINT , SIG_IGN);	//忽略Ctrl-C信号
	signal(SIGQUIT , SIG_IGN);	//忽略Ctrl-\信号
	
//	signal(SIGTSTP , ctrl_z);	//忽略Ctrl-Z信号

	
	struct user *person;			//为用户申请资源s
	person = (struct user *)malloc(sizeof(struct user));
	
	struct music_dir *user_music;	//为用户机器上的音乐申请资源
	user_music = (struct music_dir *)malloc( MUSIC_DIRECT_DIR_MAX_COUNT * sizeof(struct music_dir));	//用户机器上最多有MUSIC_DIRECT_DIR_MAX_COUNT个音乐目录（直接的目录）

	log_display(person , user_music);	//用户的界面开始入口

	free(person);
	free(user_music);
	
	return 0;
}