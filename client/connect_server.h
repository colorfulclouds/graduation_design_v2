//CONNECT_SERVER

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef GROCERY_BOX
#define GROCERY_BOX
#include "grocery_box.h"
#endif


int init_client(struct sockaddr_in *client_info , int port);

int init_online_sock_fd(struct sockaddr_in *client_info , int port);