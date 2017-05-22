//PLAY_GO
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

#ifndef PLAY_QUEUE
#define PLAY_QUEUE
#include "play_queue.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

//指令参数分配函数
int play_shell(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);

int play_go_q(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//查看播放队列
int play_go_m(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//随便选一个播放
int play_go_f(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//下一首
int play_go_l(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//上一首
int play_go_d(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//显示配置文件中的音乐
int play_go_n(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//指定和跳
int play_go_a(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//添加
int play_go_a_n(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);//添加
int play_go_c(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//清除
int play_go_r(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//循环与否
int play_go_R(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//随机与否
int play_go_a_d(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);//添加
int play_go_p(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//添加
int play_go_t(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//暂停与否
int play_go_x(struct pthread_number *order_pthread , struct music_shell *order_struct , struct music_dir *user_music);	//yff.conf增加新目录