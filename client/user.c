#ifndef USER
#define USER
#include "user.h"
#endif

void release_socket_t(struct socket_t *do_socket)
{
	close(do_socket->client_sock_fd);
	close(do_socket->download_sock_fd);
	close(do_socket->online_sock_fd);
}