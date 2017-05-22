//SERVER
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

int init_server(struct sockaddr_in *server_info , int port);

int init_server_online(struct sockaddr_in *server_info , int port);