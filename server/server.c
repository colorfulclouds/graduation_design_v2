#ifndef SERVER
#define SERVER
#include "server.h"
#endif

int init_server(struct sockaddr_in *server_info , int port)
{
	int no_use = 1;
	int server_sock_fd;

	server_sock_fd = socket(AF_INET,SOCK_STREAM,0);
	
	if(server_sock_fd == -1)
	{
		perror("10001");
		exit(EXIT_FAILURE);
	}

	bzero(server_info,sizeof(struct sockaddr_in));
	
	server_info->sin_family = AF_INET;
	server_info->sin_port = htons(port);	//32165
	server_info->sin_addr.s_addr = inet_addr("10.135.188.26");
	
	//quick restart
	if( ( setsockopt(server_sock_fd , SOL_SOCKET , SO_REUSEADDR , &no_use , sizeof no_use ) ) != 0)  
    {
        perror("10003");  
        exit(EXIT_FAILURE);  
    }
	//pthread
	if( ( setsockopt(server_sock_fd , SOL_SOCKET , SO_REUSEPORT , &no_use , sizeof no_use ) ) != 0)
	{
		perror("10004");
		exit(EXIT_FAILURE);
	}

	if ( bind(server_sock_fd , (struct sockaddr *)server_info,sizeof(struct sockaddr)) == -1 )
	{
		perror("10002");
		exit(EXIT_FAILURE);
	}

	listen(server_sock_fd,5);

	return server_sock_fd;
}

int init_server_online(struct sockaddr_in *server_info , int port)
{
	int no_use = 1;
	
	int server_online_sock_fd;
	server_online_sock_fd = socket(AF_INET , SOCK_DGRAM , 0);
	
	if (server_online_sock_fd == -1)
    {
        perror("-10001");
        exit(EXIT_FAILURE);
    }
	
	bzero(server_info , sizeof(struct sockaddr_in));
	
	server_info->sin_family = AF_INET;
	server_info->sin_port = htons(port);
	server_info->sin_addr.s_addr = htonl(INADDR_ANY);	//inet_addr("10.135.188.26");
	
	if( ( setsockopt(server_online_sock_fd , SOL_SOCKET , SO_REUSEADDR , &no_use , sizeof no_use ) ) != 0)  
    {
        perror("10003");  
        exit(EXIT_FAILURE);  
    }
	if( ( setsockopt(server_online_sock_fd , SOL_SOCKET , SO_REUSEPORT , &no_use , sizeof no_use ) ) != 0)
	{
		perror("10004");
		exit(EXIT_FAILURE);
	}
	
	if ( bind(server_online_sock_fd , (struct sockaddr *)server_info , sizeof(struct sockaddr)) == -1 )
	{
		perror("-10002");
		exit(EXIT_FAILURE);
	}
	
	return server_online_sock_fd;
}