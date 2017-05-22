//MODES
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef RECVFROM_CLIENT
#define RECVFROM_CLIENT
#include "recvfrom_client.h"
#endif


#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef CHECK
#define CHECK
#include "check.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef SERVER
#define SERVER
#include "server.h"
#endif

#ifndef HANDLE
#define HANDLE
#include "handle.h"
#endif

void *all_start(void *arg);
void start(struct pthread *socket_kit);

int modes_user_in(int client_sock_fd , struct user *one ,MYSQL *connection);
int modes_user_register(int client_sock_fd , struct user *one ,MYSQL *connection);

