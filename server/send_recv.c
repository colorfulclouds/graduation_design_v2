#ifndef SEND_RECV
#define SEND_RECV
#include "send_recv.h"
#endif

int _recv(int client_sock_fd , void *buff , int length , int flag)
{
	int result;
	
	char test_server = 'y';
	
	
	switch(flag)
	{
		case 1:
			result = recv(client_sock_fd , buff , sizeof(char) , 0);
			send(client_sock_fd , (void *)&test_server , sizeof(char) , 0);
			break;
		case 2:
			result = recv(client_sock_fd , buff , sizeof(int) , 0);
			send(client_sock_fd , (void *)&test_server , sizeof(char) , 0);
			break;
		case 3:
			result = recv(client_sock_fd , buff , length , 0);
			send(client_sock_fd , (void *)&test_server , sizeof(char) , 0);
			break;
			
		default:
			;	//impossible here
			break;
	}
	
	if(result == 0)
		return -1;
	else
		return 1;
	
	
}

int _send(int client_sock_fd , void *buff , int length , int flag)
{
	int result;
	
	char test_server ;
	
	switch(flag)
	{
		case 1:	
			send(client_sock_fd , buff , sizeof(char) , 0);
			result = recv(client_sock_fd , (void *)&test_server , sizeof(char) , 0);
			break;
			
		case 2:
			send(client_sock_fd , buff , sizeof(int) , 0);
			result = recv(client_sock_fd , (void *)&test_server , sizeof(char) , 0);
			break;
		case 3:
			send(client_sock_fd , buff , length , 0);
			result = recv(client_sock_fd , (void *)&test_server , sizeof(char) , 0);
			break;
			
		default:
			;	//impossible here
			break;
	}	
	if(result == 0)
		return -1;
	else
		return 1;
}

int send_music_block(int download_sock_fd , void *buff , int length)
{
	int result;
	int sent = 0;	//已经发送的数量
	
	while(1)
	{
		result = send(download_sock_fd , buff , length , 0);
		if(result == -1)
			return -1;
		
		sent += result;
		if(sent >= length)
			break;
		
		length -= result;
		buff = (char *)buff + result;
	}
	
	return 1;
}