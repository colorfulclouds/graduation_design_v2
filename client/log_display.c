#ifndef LOG_DISPLAY
#define LOG_DISPLAY
#include "log_display.h"
#endif

int log_display(struct user * one , struct music_dir * user_music )
{
	int client_port = 32165;
	int download_port = 12345;
	int online_port = 54321;
	int upload_port = 56123;
		
	struct socket_t do_socket ;
	do_socket.person = one;
	
	char choice = '\0';

	pthread_t monitor_thread;
	pthread_create(&monitor_thread , NULL , monitor_yff_conf , (void *)user_music);	//目前是 15s 刷新一次用户配置文件
	
	int op_result;	//各种命令执行后的返回值
	
	while(1)
	{
		printf("a>log_in\n");
		printf("b>register\n");
		printf("c>anonymous(ni ming)\n");
		printf("d>user manual\n");
		printf("e>stand alone\n");
		printf("f>quit\n");
		
		printf("which one:");
/*关闭
	store_tty(0);	//保存终端模式
	set_tty();		//无须回车确认
	choice = getchar();
	store_tty(1);	//恢复终端模式
*/		
		scanf("%c",&choice);

		if(choice == 'a' || choice == 'b' || choice == 'c')
		{
			//通用端口连接
			do_socket.client_sock_fd = init_client( &(do_socket.client_info) , client_port);
			if(do_socket.client_sock_fd == -1)
			{
				printf("[20002] connect server error\n");
				continue ;
			}
			//下载端口连接
			do_socket.download_sock_fd = init_client( &(do_socket.download_info) , download_port);
			if(do_socket.download_sock_fd == -1)
			{
				close(do_socket.client_sock_fd);
				printf("[20002] connect server error\n");
				continue ;
			}
			//在线试听端口连接
			do_socket.online_sock_fd = init_client( &(do_socket.online_info) , online_port);
			if(do_socket.online_sock_fd == -1)
			{
				close(do_socket.client_sock_fd);
				close(do_socket.download_sock_fd);
				printf("[20002] connect server error\n");
				continue ;
			}
			//上传端口连接
			do_socket.upload_sock_fd = init_client( &(do_socket.upload_info) , upload_port);
			if(do_socket.upload_sock_fd == -1)
			{
				close(do_socket.client_sock_fd);
				close(do_socket.download_sock_fd);
				close(do_socket.online_sock_fd);
				printf("[20002] connect server error\n");
				continue ;
			}
		}

		switch(choice)
		{
			case 'a':
				_flush();
				if( send_order(do_socket.client_sock_fd , choice) == -1)
				{
					release_socket_t(&do_socket);
					printf("[22013]the server is downtime.\nyou can choose \"stand alone\" mode to enjoy the music on you machine.\n");
					continue ;
				}
				if ( log_display_in(one , do_socket.client_sock_fd , MODE_LOAD) == -1)
				{
					release_socket_t(&do_socket);
					continue ;
				}
				else
				{					
					//下面的流程 最后梳理 
					op_result = op_shell(user_music , &do_socket , MODE_LOAD);	//这里是关键的地方
					
					switch(op_result)
					{
						case LOGOUT:
							release_socket_t(&do_socket);
							continue ;
//							break;	//形式完整 删除无妨
							
						case QUIT:
							release_socket_t(&do_socket);
							break;
					}
					break;
				}
				
			case 'b':
				_flush();
				if( send_order(do_socket.client_sock_fd , choice) == -1)
				{
					release_socket_t(&do_socket);
					printf("[22013]the server is downtime.\nyou can choose \"stand alone\" mode to enjoy the music on you machine.\n");					
					continue ;
				}
				if ( log_display_register(one , do_socket.client_sock_fd) == -1)
				{
					release_socket_t(&do_socket);
					printf("register error[net]\n");
					continue ;
				}
				else
				{
					//用户注册成功，接下来就是登陆了(自行登陆)
					release_socket_t(&do_socket);
					continue ;
				}
				
			case 'c':
				_flush();
				if( send_order(do_socket.client_sock_fd , choice) == -1)
				{
					release_socket_t(&do_socket);
					printf("[22013]the server is downtime.\nyou can choose \"stand alone\" mode to enjoy the music on you machine.\n");
					continue ;
				}
				if ( log_display_in(one , do_socket.client_sock_fd , MODE_ANONYMOUS) == -1)
				{
					release_socket_t(&do_socket);
					printf("login error[net]\n");
					continue ;
				}
				else
				{
					op_result = op_shell(user_music , &do_socket , MODE_ANONYMOUS);	//这里是关键的地方
					
					switch(op_result)
					{
						case LOGOUT:
							release_socket_t(&do_socket);
							continue ;
//							break;	//形式完整 删除无妨
							
						case QUIT:
							release_socket_t(&do_socket);
							break;
					}
					break;
				}
				
			case 'd':
				_flush();
				{
					log_display_manual();
					continue ;	//重选功能
				}
//				break;	//有或无效果一样

			case 'e':	//单机模式
				_flush();
				op_result = op_shell(user_music , &do_socket , MODE_ALONE);;
				switch(op_result)
				{
					case LOGOUT:
						;
						continue ;
//							break;	//形式完整 删除无妨
						
					case QUIT:
						;
						break;
				}
				
				break;
				
			case 'f':
				;	//quit this app
				break;
				
			default:
				_flush();
				printf("[21001]input error! TRY AGAIN!\n");
				continue ;
		}
		
		break;	//这个break 起到退出的作用
	}
	
	pthread_cancel(monitor_thread);	//结束掉监控配置文件的线程
}

int log_display_register_id(struct user * one , int client_sock_fd)
{
	return _recv(client_sock_fd , (void *)one->id , sizeof(one->id) , 3);
}

int log_display_register_reg_time(struct user * one , int client_sock_fd)
{
	return _recv(client_sock_fd , (void *)one->password , sizeof(one->password) , 3);
}

void log_display_register_name(struct user * one)
{
	bzero(one->name , sizeof(one->name) );
	
	char *name;
	name = (char *)malloc(sizeof(char) * MAX_INPUT_BUFF );
	
	printf("name:");
	fgets(name , MAX_INPUT_BUFF , stdin);
	strncpy(one->name , name , MAX_LENGTH_TWENTY - 1);
	
	if(strlen(one->name) < MAX_LENGTH_TWENTY - 1)	//remove last enter
		one->name[strlen(one->name) - 1] = '\0';
	
	free(name);
}

int log_display_register_email(struct user * one , int client_sock_fd)
{
	bzero(one->email , sizeof(one->email) );
	char server_check_result;
	
	char *email;
	email = (char *)malloc(sizeof(char) * MAX_INPUT_BUFF );

	while(1)
	{
		printf("email:");
		fgets(email , MAX_INPUT_BUFF , stdin);
		strncpy(one->email , email , MAX_LENGTH_THIRTY - 1);
		
		if(strlen(one->email) < MAX_LENGTH_THIRTY - 1)	//remove last enter
			one->email[strlen(one->email) - 1] = '\0';
		
		if(check_result((void *)one->email , 2) == 1)	//这句话在客户端进行正则表达式进行校验
		{
			if( recv_check_email_result(client_sock_fd , one , &server_check_result) == -1)
			{
				printf("[22010]the server is downtime.\nyou can choose another mode\n");
				free(email);
				return -1;
			}
			
			if(server_check_result == 'y')
			{
				printf("[22009]the email has registered.CHANGE ANOTHER ONE!\n");
				continue ;
			}	
			//server_check_result == 'n'
			//用户注册的这有邮箱还没有人使用，所以可以进行注册
			else
				break ;		//check success
		}
		else
		{
			printf("[22003]check your mail format.TRY AGAIN!\n");
			continue ;	//fail to check
		}
	}
	
	free(email);
	return 1;
}

void log_display_register_password(struct user * one)
{
	bzero(one->password , sizeof(one->password) );
	
	char *password;
	password = (char *)malloc(sizeof(char) * MAX_INPUT_BUFF );
	
	char *password_again;
	password_again = (char *)malloc(sizeof(char) * MAX_LENGTH_TWENTY );

	while(1)
	{
		printf("password:");
system("stty -echo");
		fgets(password , MAX_INPUT_BUFF , stdin);
		strncpy(one->password , password , MAX_LENGTH_TWENTY - 1);
		
		if(strlen(one->password) < MAX_LENGTH_THIRTY - 1)	//remove last enter
			one->password[strlen(one->password) - 1] = '\0';
		
		if(check_result( (void *)one->password , 3) == 1)
		{
			//twice input password
system("stty echo");
printf("\n");
			printf("password again:");
system("stty -echo");
			fgets(password , MAX_INPUT_BUFF , stdin);
			strncpy(password_again , password , MAX_LENGTH_TWENTY - 1);
			if(strlen(password_again) < MAX_LENGTH_THIRTY - 1)
				password_again[strlen(password_again) - 1] = '\0';
			if(strcmp(password_again , one->password) == 0)
				break;
			else
			{
system("stty echo");
printf("\n");
				printf("[22005]two password are not same.TRY AGAIN!\n");
				continue ;
			}
		}
		else
		{
system("stty echo");
printf("\n");
			printf("[22004]check your password format.TRY AGAIN!\n");
			continue ;
		}
	}
system("stty echo");

	free(password);
	free(password_again);
}

void log_display_register_sex(struct user * one)
{
	while(1)
	{
		printf("sex:");
		scanf("%c", &(one->sex) );
		
		if(one->sex != 'w' && one->sex != 'm' && one->sex != 'n')
		{
			printf("[]wrong sex.TRY AGAIN!\n");
			_flush();
			continue ;
		}
		_flush();
		break;
	}
	
}

void log_display_register_age(struct user * one)
{
	int result;
	
	while(1)
	{
		printf("age:");
		result = scanf("%d", &(one->age) );
		if(result == 0)
		{
			printf("[22006]check your input.TRY AGAIN!\n");
			_flush();	//flush a character in stdin(error input)
			continue ;
		}
		else
		{
			if( (one->age) <= 0 || (one->age) > 120)
			{
				printf("[22007]impossible age!TRY AGAIN!\n");
				_flush();
				continue ;
			}
		}
		_flush();
		break ;
	}
	
}

int log_display_register(struct user * one , int client_sock_fd)
{
	log_display_register_name(one);
	log_display_register_password(one);
	log_display_register_age(one);
	log_display_register_sex(one);
	if( log_display_register_email(one , client_sock_fd) == -1)
		return -1;
	
	if( send_user_info(client_sock_fd , one) == -1 )
		return -1;
	
	if( log_display_register_id(one , client_sock_fd) == -1)
		return -1;
	if( log_display_register_reg_time(one , client_sock_fd) == -1)
		return -1;
	
printf("congratulations! register successfully!\n");
printf("your id is [%s]\n", one->id);
	//用户注册完成之后，就该做和音乐有关的事情了
	
	return 1;	//这个1以后就成为注册成功的标志
}

void log_display_in_id(struct user * one , int mode_flag)
{
	if(mode_flag == MODE_ANONYMOUS)
	{
		strcpy(one->id , "10036");
		return ;
	}
	bzero(one->id , sizeof(one->id) );
	
	char *id;
	id = (char *)malloc(sizeof(char) * MAX_INPUT_BUFF );
	
	while(1)
	{
		printf("id:");
		fgets(id , MAX_INPUT_BUFF , stdin);
		strncpy(one->id , id , MAX_LENGTH_TWENTY - 1);		
		if(one->id[0] == '\n' )
		{
			printf("[21101]input error! TRY AGAIN!\n");
			continue ;
		}
		if(strlen(one->id) < MAX_LENGTH_TWENTY - 1)	//remove last enter
		{
			one->id[strlen(one->id) - 1] = '\0';
			break ;
		}
		if(strlen(one->id) == MAX_LENGTH_TWENTY - 1)
			break;
	}
	free(id);
}

void log_display_in_password(struct user * one , int mode_flag)
{
	if(mode_flag == MODE_ANONYMOUS)
	{
		strcpy(one->password , "xanonymousx");
		return ;
	}
	bzero(one->password , sizeof(one->password) );
	
	char *password;
	password = (char *)malloc(sizeof(char) * MAX_INPUT_BUFF);
	
	while(1)
	{
		printf("password:");
system("stty -echo");
		fgets(password , MAX_INPUT_BUFF , stdin);
		strncpy(one->password , password , MAX_LENGTH_TWENTY - 1);
		if(one->password[0] == '\n')
		{
system("stty echo");
printf("\n");
			printf("[21102]input error! TRY AGAIN!\n");
			continue ;
		}
		if(strlen(one->password) < MAX_LENGTH_TWENTY - 1)
		{
system("stty echo");
printf("\n");
			one->password[strlen(one->password) - 1] = '\0';
			break;
		}
		if(strlen(one->password) < MAX_LENGTH_TWENTY - 1)
		{
system("stty echo");
printf("\n");
			break;
		}
	}
system("stty echo");
	free(password);
}

int log_display_in(struct user * one , int client_sock_fd , int mode_flag)
{
	char check_result;	//server check result
	
	log_display_in_id(one , mode_flag);
	log_display_in_password(one , mode_flag);
	
if(mode_flag == MODE_LOAD)
	printf("loading......\n");

	if( send_id_password(client_sock_fd , one) == -1)
	{
		printf("internet is wrong\n");
		return -1;
	}
	
	if( recv_check_result(client_sock_fd , &check_result) == -1) //服务器传回账号、密码校验结果
	{
		printf("the internet is wrong\n");
		return -1;
	}
	
	if(check_result == 'y')
	{
		if( recv_user_info(client_sock_fd , one ) == -1)
			return -1;
		else
		{
			//只有真实登录时 才显示登录成功
			if(mode_flag == MODE_LOAD)
				printf("loading successfully\n");
			return 1;	//登录成功
		}
	}
	else
	{
		//check_result == 'n' 信息有服务器匹配错误
		printf("[21201]your information is wrong!please check your information or contact the administrator\n");
		return -1;
	}

	return 1;	//impossible here
}

void log_display_manual()
{
	//the manual is on user's machine
	char buff[MAX_MANUAL_BUFF];
	FILE *fp = fopen("manual.txt" , "r");
	
	if(fp == NULL)
	{
		printf("manual is not exist!!!\n");
		return ;
	}
	
	while(fgets(buff , MAX_MANUAL_BUFF , fp))
		printf("%s", buff);
	
	fclose(fp);
}

