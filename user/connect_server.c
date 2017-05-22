#ifndef CONNECT_SERVER
#define CONNECT_SERVER
#include "connect_server.h"
#endif

int init_client(struct sockaddr_in *client_info , int port)
{
	int client_sock_fd;
	client_sock_fd = socket(AF_INET,SOCK_STREAM,0);
	
	if(client_sock_fd == -1)
	{
		printf("[20001]FATAL ERROR!\n");
		exit(EXIT_FAILURE);
	}
	
	bzero(client_info,sizeof(struct sockaddr_in));
	
	client_info->sin_family = AF_INET;
	client_info->sin_port = htons(port);
	client_info->sin_addr.s_addr = inet_addr("123.207.118.238");
	
	if(connect(client_sock_fd,(struct sockaddr *)client_info,sizeof(struct sockaddr)) == -1)
	{
		printf("[20002]THE SERVER IS DOWNTIME\n");
		printf("you can choose \"stand alone\" mode to enjoy the music on you machine.\n");
		client_sock_fd = -1;
	}
	
	return client_sock_fd;
}

int init_online_sock_fd(struct sockaddr_in *client_info , int port)
{
	int no_use = 1;
	int online_sock_fd;
	online_sock_fd = socket(AF_INET , SOCK_DGRAM , 0);
	
	if(online_sock_fd == -1)
	{
		printf("[20003]FATAL ERROR!\n");
		exit(EXIT_FAILURE);
	}
	
	bzero(client_info,sizeof(struct sockaddr_in));
	
	client_info->sin_family = AF_INET;
	client_info->sin_port = htons(port);
	client_info->sin_addr.s_addr = inet_addr("123.207.118.238");
	
	// if( ( setsockopt(online_sock_fd , SOL_SOCKET , SO_REUSEADDR , &no_use , sizeof no_use ) ) != 0)  
    // {
        // perror("10003");  
        // exit(EXIT_FAILURE);  
    // }
	// if( ( setsockopt(online_sock_fd , SOL_SOCKET , SO_REUSEPORT , &no_use , sizeof no_use ) ) != 0)
	// {
		// perror("10004");
		// exit(EXIT_FAILURE);
	// }
	
	// if ( bind(online_sock_fd , (struct sockaddr *)client_info , sizeof(struct sockaddr)) == -1 )
	// {
		// perror("-10002");
		// exit(EXIT_FAILURE);
	// }
	
	return online_sock_fd;
}