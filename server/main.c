#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef SERVER
#define SERVER
#include "server.h"
#endif

#ifndef MODES
#define MODES
#include "modes.h"
#endif

#ifndef DATABASE
#define DATABASE
#include "database.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

int main(void)
{
	struct pthread v;	//struct in user.h
	
	MYSQL connection;
	init_database(&connection);
	v.connection = &connection;
	
	struct sockaddr_in server_info;
	int server_port = 32165;
	v.server_sock_fd = init_server(&server_info , server_port);
	
	struct sockaddr_in server_info_download;
	int server_port_download = 12345;
	v.server_sock_fd_download = init_server(&server_info_download , server_port_download);
	
	struct sockaddr_in server_info_online;
	int server_port_online = 54321;
	v.server_sock_fd_online = init_server(&server_info_online , server_port_online);
	
	struct sockaddr_in server_info_upload;
	int server_port_upload = 56123;
	v.server_sock_fd_upload = init_server(&server_info_upload , server_port_upload);

	while(1)
	{
		v.client_sock_fd = accept(v.server_sock_fd , NULL , NULL);	//业务连接

		v.download_sock_fd = accept(v.server_sock_fd_download , NULL , NULL);	//下载连接

		v.online_sock_fd = accept(v.server_sock_fd_online , NULL , NULL);	//试听连接
		
		v.upload_sock_fd = accept(v.server_sock_fd_upload , NULL , NULL);	//上传连接

		pthread_create( &(v.pid) , NULL , all_start , (void *)&v);	//in modes.h
	}

	close(v.server_sock_fd);	

	mysql_close(&connection);
	
	return 0;
}