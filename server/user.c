#ifndef USER
#define USER
#include "user.h"
#endif


void release_socket_kit(struct pthread *socket_kit)
{
	close(socket_kit->client_sock_fd);
	close(socket_kit->download_sock_fd);
	close(socket_kit->online_sock_fd);
}