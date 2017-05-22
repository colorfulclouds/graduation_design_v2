#ifndef HANDLE
#define HANDLE
#include "handle.h"
#endif

void init_affair_pthread(struct pthread_number *affair_pthread , struct pthread *socket_kit)
{
	affair_pthread->topxx_thread = 0;	//未使用
	affair_pthread->online_thread = 0;
	affair_pthread->download_thread = 0;
	affair_pthread->upload_thread = 0;
	
	affair_pthread->socket_kit = socket_kit;

}

//销毁相关的线程
void cancel_thread_release_affair_pthread(struct pthread_number *affair_pthread)
{
	int pthread_test;
	//下载线程销毁
	if(affair_pthread->download_thread != 0)
	{
		pthread_test = pthread_kill(affair_pthread->download_thread , 0);
		
		if(pthread_test != ESRCH)
				pthread_cancel(affair_pthread->download_thread);
	}
	//试听线程销毁
	if(affair_pthread->online_thread != 0)
	{
		pthread_test = pthread_kill(affair_pthread->online_thread , 0);
		
		if(pthread_test != ESRCH)
			pthread_cancel(affair_pthread->online_thread);
	}
	//上传线程销毁
	if(affair_pthread->upload_thread != 0)
	{
		pthread_test = pthread_kill(affair_pthread->upload_thread , 0);
		
		if(pthread_test != ESRCH)
			pthread_cancel(affair_pthread->upload_thread);
	}
	
	free(affair_pthread);
}

int handle_work(struct pthread *socket_kit)
{
	int pthread_test;
	
	int user_request;
	
	struct pthread_number *affair_pthread;	//defined in user.h
	affair_pthread = (struct pthread_number *)malloc(sizeof(struct pthread_number));
	
	init_affair_pthread(affair_pthread , socket_kit);
	
	//开启下载监视线程 持续监视 用户的下载请求
    pthread_create( &(affair_pthread->download_thread) , NULL , buff_music , (void *)affair_pthread );
	//开启在线试听监视线程
	// pthread_create( &(affair_pthread->online_thread) , NULL , online_music , (void *)affair_pthread );
	//开启上传监视线程 持续监视 用户的上传请求（正常登录的用户才可以上传）
	//匿名用户id=10036不可以上传
	if( strcmp(affair_pthread->socket_kit->person->id , "10036") != 0)
		pthread_create( &(affair_pthread->upload_thread) , NULL , recv_upload_music , (void *)affair_pthread );
	
	while(1)
	{
		if( recv_user_request(affair_pthread->socket_kit->client_sock_fd , &user_request) == -1)
		{
			cancel_thread_release_affair_pthread(affair_pthread);
			return -1;
		}
	
		switch(user_request)
		{
			case PLAYS_t:	//4
				//向用户发送TOPxx
				if( handle_work_t(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
			
			case PLAYS_s:
				//向用户传输音乐
				if( handle_work_s(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_o:
				//在线试听（伪试听 校验music_id）
				if( handle_work_o(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case -PLAYS_o://（真试听 真实数据的传输）
				handle_work_o_c(affair_pthread);
				break;
				
			case PLAYS_k:
				if( handle_work_k(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_l:
				if( handle_work_l(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_r:
				if( handle_work_r(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_f:
				if( handle_work_f(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_m:
				if( handle_work_m(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_c:
				if( handle_work_c(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_l | PLAYS_d:
				if( handle_work_l_d(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_l | PLAYS_x:
				if(handle_work_l_x(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_l | PLAYS_u:
				if(handle_work_l_u(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_u | PLAYS_x:
				if(handle_work_u_x(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case PLAYS_u | PLAYS_s:
				if(handle_work_u_s(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
			
			case PLAYS_c | PLAYS_l:
				if(handle_work_c_l(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			//******************
			//friend相关命令处理
			//******************
			case FRIEND + FRIEND_A:
				if(friend_work_a(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_D:
				if(friend_work_d(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_F:
				if(friend_work_f(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_N:
				if(friend_work_n(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_L:
				if(friend_work_l(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
			
			case FRIEND + FRIEND_C:
				if(friend_work_c(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_U:
				if(friend_work_u(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_S:
				if(friend_work_s(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case FRIEND + FRIEND_T:
				if(friend_work_t(affair_pthread) == -1)
				{
					cancel_thread_release_affair_pthread(affair_pthread);
					return -1;
				}
				break;
				
			case LOGOUT:
					;	//这里没有break 不需要 向下传递
			case QUIT:
				cancel_thread_release_affair_pthread(affair_pthread);
				return 1;
				//break;
				
			default:
				continue ;	//用户传过来无意义的参数 服务器忽略
		}
	}
	
	//不会执行到这里
	cancel_thread_release_affair_pthread(affair_pthread);
	return 1;
}

//用户请求TOPxx
int handle_work_t(struct pthread_number *affair_pthread)
{
	int i;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char *query = "select * from music order by listen_count DESC limit 10";	//目前是获取TOP10
	
	mysql_query(affair_pthread->socket_kit->connection , query);

	result = mysql_store_result(affair_pthread->socket_kit->connection );
	
	for(i = 0 ; i<TOP_NUMBER ; i ++)
	{
		mysql_data_seek(result , i);
		row = mysql_fetch_row(result);
		usleep(1000);	//缓和服务器
		if( send_music_id_name(affair_pthread->socket_kit->client_sock_fd , atoi(row[0]) , row[1]) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
	}
	mysql_free_result(result);
	return PLAYS_t;
}

//用户下载音乐（只对下载请求的music_id进行校验）（数据传输由下载线程进行）
int handle_work_s(struct pthread_number *affair_pthread)
{
	//这个函数 只是校验用户music_id合法性 并返回音乐的名字 有专门的下载线程 一直在等待用户的下载请求
	int music_id;
	int succ = 1;
	int unsucc = -1;
	
	char music_name[MUSIC_NAME_MAX_LENGTH];
	bzero(music_name , sizeof(music_name));
	
	if( _recv(affair_pthread->socket_kit->client_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		return -1;
	}
	
	if(music_id == 0)
	{
		music_id = get_random_music_id(affair_pthread->socket_kit->connection);	//服务器随机的music_id肯定是合法的
		get_music_path_name(music_id , affair_pthread->socket_kit->connection , NULL , music_name);

		if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)&music_id , -1 , 2) == -1)
			return -1;
		if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			return -1;
		//在这里向用户传输音乐
	}
	else
	{	
		//检验用户传来的music_id合法性
		if( test_music_id(music_id , affair_pthread->socket_kit->connection) == -1)
		{
			//music_id不合法
			if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)&unsucc , -1 , 2) == -1)
				return -1;
			
			return PLAYS_s;
		}
		else
		{
			//music_id合法
			if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)&succ , -1 , 2) == -1)
				return -1;
			
			get_music_path_name(music_id , affair_pthread->socket_kit->connection , NULL , music_name);
			//这里不需要发送music_id 因为用户输入的已经合法了
			if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
				return -1;
		}
	}

	return PLAYS_s;
}

//用户在线试听（不发送音乐数据本身）（伪试听）
int handle_work_o(struct pthread_number *affair_pthread)
{
	int music_id;
	int succ = 1;
	int unsucc = -1;
	
	char music_name[MUSIC_NAME_MAX_LENGTH];
	bzero(music_name , sizeof(music_name));
	
	if( _recv(affair_pthread->socket_kit->client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		return -1;
	
	if( test_music_id(music_id , affair_pthread->socket_kit->connection) == -1)
	{
		//music_id不合法
		if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)&unsucc , -1 , 2) == -1)
			return -1;
		
		return PLAYS_o;
	}
	else
	{
		//music_id合法
		if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)&succ , -1 , 2) == -1)
			return -1;
		
		//发送音乐名字
		get_music_path_name(music_id , affair_pthread->socket_kit->connection , NULL , music_name);
		if( _send(affair_pthread->socket_kit->client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			return -1;
	}
	return PLAYS_o;
}

//用户在线试听（音乐数据本身的传送）（真试听）
int handle_work_o_c(struct pthread_number *affair_pthread)
{
	int pthread_test;
	
	if(affair_pthread->online_thread == 0)
		pthread_create( &(affair_pthread->online_thread) , NULL , online_music , (void *)affair_pthread );
	else
	{
		pthread_test = pthread_kill(affair_pthread->online_thread , 0);
		if(pthread_test == ESRCH)
			pthread_create( &(affair_pthread->online_thread) , NULL , online_music , (void *)affair_pthread );
		else
		{
			pthread_cancel(affair_pthread->online_thread);
			pthread_create( &(affair_pthread->online_thread) , NULL , online_music , (void *)affair_pthread );
		}
		
	}
	return -PLAYS_o;
}

//用户根据music_style查看音乐
int handle_work_k(struct pthread_number *affair_pthread)
{	
	int i;
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int music_style;
	if( _recv(client_sock_fd , (void *)&music_style , -1 , 2) == -1)
	{
		return -1;
	}
	
	char music_style_str[3];
	bzero(music_style_str , sizeof(music_style_str) );
	sprintf(music_style_str , "%d" , music_style);

	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	strcpy(query , "select * from music where style = ");
	strcat(query , music_style_str);

	MYSQL_RES *result;
	MYSQL_ROW row;
	int music_style_count;
	
	mysql_query(affair_pthread->socket_kit->connection , query);
	result = mysql_store_result(affair_pthread->socket_kit->connection );
	music_style_count = mysql_num_rows(result);
	
	if( _send(client_sock_fd , (void *)&music_style_count , -1 , 2) == -1)
	{
		mysql_free_result(result);
		return -1;
	}
	
	for(i = 0 ; i < music_style_count ; i ++)
	{
		mysql_data_seek(result , i);
		row = mysql_fetch_row(result);
		usleep(1000);
		
		if( send_music_id_name(client_sock_fd , atoi(row[0]) , row[1]) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
	}
	
	mysql_free_result(result);
	return PLAYS_k;
}

//用户收藏音乐
int handle_work_l(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int music_id;
	int succ = 1;
	int unsucc = -1;
	
	if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		return -1;
	
	if( test_music_id(music_id , affair_pthread->socket_kit->connection) == -1)
	{
		//music_id不合法
		if( _send(client_sock_fd , (void *)&unsucc , -1 , 2) == -1)
		{
			return -1;
		}
		
		return PLAYS_l;
	}
	else
	{
		//music_id合法
		if( _send(client_sock_fd , (void *)&succ , -1 , 2) == -1)
		{
			return -1;
		}
		add_user_collect(affair_pthread , music_id);	//添加到用户收藏一栏（user.collect）
	}
	
	return PLAYS_l;
}

//用户查看收藏的音乐
int handle_work_l_d(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	int collect_count;
	collect_count = get_collect_count(connection , affair_pthread->socket_kit->person->id);
	
	if( _send(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
	{
		return -1;
	}
	if(collect_count == 0)
		return PLAYS_l | PLAYS_d;
	
	char query[MAX_LENGTH_QUERY_SELECT];
	bzero(query , sizeof(query));
	
	char user_collect_c[MAX_COLLECT_MUSIC_LENGTH];	//100
	bzero(user_collect_c , sizeof(user_collect_c));
	get_user_collect(connection , affair_pthread->socket_kit->person->id , user_collect_c);

	char *music_id_c;
	//分解user_collect 得到各种music_id 向用户发送
	
	music_id_c = strtok(user_collect_c , "#");
	if(music_id_c == NULL)
		return PLAYS_l | PLAYS_d;
	
	strcpy(query , "select name from music where id=");
	strcat(query , music_id_c);

	mysql_query(connection , query);
	result = mysql_store_result(connection );
	row = mysql_fetch_row(result);

	if( send_music_id_name(client_sock_fd , atoi(music_id_c) , row[0]) == -1)
	{
		mysql_free_result(result);
		return -1;
	}
	
	while(1)
	{
		mysql_free_result(result);
		
		music_id_c = strtok(NULL , "#");
		if(music_id_c == NULL)
			return PLAYS_l | PLAYS_d;
		
		bzero(query , sizeof(query));
		strcpy(query , "select name from music where id=");
		strcat(query , music_id_c);
		mysql_query(connection , query);
		result = mysql_store_result(connection );
		row = mysql_fetch_row(result);
		if( send_music_id_name(client_sock_fd , atoi(music_id_c) , row[0]) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
	}

	mysql_free_result(result);
	return PLAYS_l | PLAYS_d;
}

//用户删除收藏的音乐
int handle_work_l_x(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int music_id;
	int succ = 1;
	int unsucc = -1;
	int exist = 2;
	int unexist = -2;
	
	//****
	int collect_count;
	collect_count = get_collect_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);
	if( _send(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
	{
		return -1;
	}
	if(collect_count == 0)
		return PLAYS_l | PLAYS_x;	//用户没有收藏 结束后续操作
	//****
	
	if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		return -1;
	}
	
	if( test_music_id(music_id , affair_pthread->socket_kit->connection) == -1)
	{
		//music_id不合法
		if( _send(client_sock_fd , (void *)&unsucc , -1 , 2) == -1)
		{
			return -1;
		}
		
		return PLAYS_l | PLAYS_x;
	}
	else
	{
		//music_id合法
		if( _send(client_sock_fd , (void *)&succ , -1 , 2) == -1)
		{
			return -1;
		}

		char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
		bzero(music_id_c , sizeof(music_id_c));
		sprintf(music_id_c , "%d" , music_id);
		
		char user_collect_c[MAX_COLLECT_MUSIC_LENGTH];	//100
		bzero(user_collect_c , sizeof(user_collect_c));
		get_user_collect(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , user_collect_c);
		
		if(strstr(user_collect_c , music_id_c) == NULL)
		{
			//不在收藏中
			if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
				return -1;
		}
		else
		{
			//在收藏中
			if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
				return -1;
			
			//从收藏中删除
			delete_user_collect(affair_pthread , user_collect_c , music_id_c , collect_count);
		}
	}
	
	return PLAYS_l | PLAYS_x;
}

//用户查看曾经上传的音乐
int handle_work_l_u(struct pthread_number *affair_pthread)
{
	int i;
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;

	int upload_count;
	upload_count = get_upload_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);
	if( _send(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
	{
		return -1;
	}
	
	if(upload_count == 0)
		return PLAYS_l | PLAYS_u;
	
	//上传不为0
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];
	bzero(query , sizeof(query));
	strcpy(query , "select * from upload_");
	strcat(query , affair_pthread->socket_kit->person->id);
	mysql_query(connection , query);
	result = mysql_store_result(connection );
	
	for(i = 0 ; i < upload_count ; i ++)
	{
		mysql_data_seek(result , i);
		row = mysql_fetch_row(result);
		
		if( send_music_id_name(client_sock_fd , atoi(row[0]) , row[1]) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
	}
	
	mysql_free_result(result);
	return PLAYS_l | PLAYS_u;
}

//用户删除曾经上传的音乐
int handle_work_u_x(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int music_id;
	int exist = 2;
	int unexist = -2;
	
	int upload_count;
	upload_count = get_upload_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);
	if( _send(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
		return -1;
	
	if(upload_count == 0)
		return PLAYS_u | PLAYS_x;
	
	if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		return -1;
	}
	//****
	if( test_upload_music_id(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , music_id) == -1)
	{
		//不曾上传
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
			return -1;
	}
	else
	{
		//曾经上传
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
			return -1;
		
		delete_user_upload(affair_pthread , music_id);
	}
	//****
	return PLAYS_u | PLAYS_x;
}

//用户下载曾经上传的音乐（只对下载请求的music_id进行校验）（数据传输由下载线程进行）
int handle_work_u_s(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	
	int music_id;
	int exist = 2;
	int unexist = -2;
	
	char music_name[MUSIC_NAME_MAX_LENGTH];	//60
	bzero(music_name , sizeof(music_name));
	
	if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		return -1;
	}
	//****
	if( test_upload_music_id(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , music_id) == -1)
	{
		//不曾上传
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
			return -1;
	}
	else
	{
		//曾经上传
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
			return -1;
		
		//发送名字
		get_music_path_name_upload(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , music_id , NULL , music_name);
		
		if( _send(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			return -1;
	}
	//****
	
	
	return PLAYS_u | PLAYS_s;
}

//用户使用音乐名字模糊进行搜索
int handle_work_f(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	char music_name[MUSIC_NAME_MAX_LENGTH];	//60
	bzero(music_name , sizeof(music_name));
	
	if( _recv(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
	{
		return -1;
	}
	
	int i;
	int match_count;	//匹配的数目
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select id , name from music where name like '%");
	strcat(query , music_name);
	strcat(query , "%'");
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	match_count = mysql_num_rows(result);
	
	if( _send(client_sock_fd , (void *)&match_count , -1 , 2) == -1)
	{
		mysql_free_result(result);
		return -1;
	}
	
	if(match_count == 0)
	{
		mysql_free_result(result);
		return PLAYS_f;
	}
	else
	{
		for(i = 0 ; i<match_count ; i ++)
		{
			mysql_data_seek(result , i);
			row = mysql_fetch_row(result);

			if( send_music_id_name(client_sock_fd , atoi(row[0]) , row[1]) == -1)
			{
				mysql_free_result(result);
				return -1;
			}
		}
	}
	
	mysql_free_result(result);
	return PLAYS_f;
}

//用户猜你喜欢
int handle_work_r(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	
	int like_music_style;
	char like_music_style_c[MAX_MUSIC_STYLE_LENGTH];	//4
	bzero(like_music_style_c , sizeof(like_music_style_c));
	
	like_music_style = get_like_music_style(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);	//获取用户听得最多的类型
	sprintf(like_music_style_c , "%d" , like_music_style);
	
	if( _send(client_sock_fd , (void *)&like_music_style , -1 , 2) == -1)
	{
		return -1;
	}
	
	int i;
	int like_count;
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select id , name from music where style =");
	strcat(query , like_music_style_c);
	strcat(query , " order by listen_count desc");
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	like_count = mysql_num_rows(result);
	like_count = like_count > LIKE_NUMBER ? LIKE_NUMBER : like_count;
	
	if( _send(client_sock_fd , (void *)&like_count , -1 , 2) == -1)
	{
		mysql_free_result(result);
		return -1;
	}
	
	for(i = 0 ; i<like_count ; i ++)
	{
		mysql_data_seek(result , i);
		row = mysql_fetch_row(result);
		if( send_music_id_name(client_sock_fd , atoi(row[0]) , row[1]) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
	}
	mysql_free_result(result);
	return PLAYS_r;
}

//用户修改信息
int handle_work_m(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int choice;
	if( _recv(client_sock_fd , (void *)&choice , -1 , 2) == -1)
	{
		return -1;
	}
	if(choice == PLAYS_m_q)
	{
		//退出修改
		return PLAYS_m;
	}
	recv_user_info(client_sock_fd , affair_pthread->socket_kit->person);
	modify_info(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , affair_pthread->socket_kit->person);
	
	return PLAYS_m;
}

//用户评论音乐
int handle_work_c(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int music_id;
	int succ = 1;
	int unsucc = -1;
	
	if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		return -1;
	}
	if( test_music_id(music_id , affair_pthread->socket_kit->connection) == -1)
	{
		//music id无效
		if( _send(client_sock_fd , (void *)&unsucc , -1 ,2) == -1)
		{
			return -1;
		}
		
		return PLAYS_c;
	}
	else
	{
		//music id有效
		if( _send(client_sock_fd , (void *)&succ , -1 , 2) == -1)
		{
			return -1;
		}
		char comment[MAX_LENGTH_COMMENT];	//363
		bzero(comment , sizeof(comment));
		if( _recv(client_sock_fd , (void *)comment , sizeof(comment) , 3) == -1)
		{
			return -1;
		}
		//空内容不能作为评论
		if( strlen(comment) != 0)
		{
			int length = strlen(comment);
			int i;
			
			if(comment[0] == ' ')
			{
				for(i = 1 ; i<length ; i ++)
					if(comment[i] != comment[0])
						break;
				if( i < length)
					add_comment(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , comment , music_id);
			}
			else
				add_comment(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , comment , music_id);
		}
		
		return PLAYS_c;
	}
	
	return PLAYS_c;
}

//用户查看评论
int handle_work_c_l(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int music_id;
	int succ = 1;
	int unsucc = -1;
	
	if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
	{
		return -1;
	}
	
	if( test_music_id(music_id , affair_pthread->socket_kit->connection) == -1)
	{
		//music id无效
		if( _send(client_sock_fd , (void *)&unsucc , -1 , 2) == -1)
		{
			return -1;
		}
		
		return PLAYS_c | PLAYS_l;
	}
	else
	{
		//music id有效
		if( _send(client_sock_fd , (void *)&succ , -1 , 2) == -1)
		{
			return -1;
		}
		char music_name[MUSIC_NAME_MAX_LENGTH];	//60
		bzero(music_name , sizeof(music_name));
		get_music_path_name(music_id , affair_pthread->socket_kit->connection , NULL , music_name);
		
		if( _send(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
		{
			return -1;
		}
		
		int comment_count;
		comment_count = get_comment_count(affair_pthread->socket_kit->connection , music_id);
		if( _send(client_sock_fd , (void *)&comment_count , -1 , 2) == -1)
		{
			return -1;
		}
		
		if( comment_count == 0)
		{
			//没有评论
			return PLAYS_c | PLAYS_l;
		}
		else
		{
			//有评论
			MYSQL *connection = affair_pthread->socket_kit->connection;
			char comment_path[MAX_COMMENT_FILE_PATH];
			bzero(comment_path , sizeof(comment_path));
			get_comment_file_path(connection , comment_path , music_id);
			
			char comment[MAX_LENGTH_COMMENT];	//363
			bzero(comment , sizeof(comment));
			
			FILE *fp = fopen(comment_path , "r");
			if( fp == NULL)
			{
				perror("fatal error");
				exit(EXIT_FAILURE);
			}
			
			int i;
			for( i = 0 ; i<comment_count ; i ++)
			{
				fgets(comment , sizeof(comment) , fp);
				if( _send(client_sock_fd , (void *)comment , sizeof(comment) , 3) == -1)
				{
					fclose(fp);
					return -1;
				}
			}
			fclose(fp);
		}
	}
	
	return PLAYS_c | PLAYS_l;
}

