//SEND_RECV
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

int _recv(int client_sock_fd , void *buff , int length , int flag);
int _send(int client_sock_fd , void *buff , int length , int flag);
int send_music_block(int upload_sock_fd , void *buff , int length);