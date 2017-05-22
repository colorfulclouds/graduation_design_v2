//LOG_DISPLAY

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef SENDTO_SERVER
#define SENDTO_SERVER
#include "sendto_server.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef RECVFROM_SERVER
#define RECVFROM_SERVER
#include "recvfrom_server.h"
#endif

#ifndef GROCERY_BOX
#define GROCERY_BOX
#include "grocery_box.h"
#endif

#ifndef CHECK_INFO
#define CHECK_INFO
#include "check_info.h"
#endif

#ifndef MUSIC
#define MUSIC
#include "music.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

//void user_init(struct user *one);

int log_display(struct user * one , struct music_dir * user_music);
//different log mode
int log_display_in(struct user * one , int client_sock_fd , int mode_flag);
void log_display_in_id(struct user * one , int mode_flag);
void log_display_in_password(struct user * one , int mode_flag);

int  log_display_register(struct user * one , int client_sock_fd);
int  log_display_register_id(struct user * one , int client_sock_fd);
int  log_display_register_reg_time(struct user * one , int client_sock_fd);
void log_display_register_name(struct user * one);
int  log_display_register_email(struct user * one , int client_sock_fd);
void log_display_register_password(struct user * one);
void log_display_register_age(struct user * one);
void log_display_register_sex(struct user * one);


void log_display_manual();
