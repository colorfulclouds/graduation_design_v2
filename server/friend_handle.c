#ifndef FRIEND_HANDLE
#define FRIEND_HANDLE
#include "friend_handle.h"
#endif

int friend_work_a(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int exist = 2;		//好友存在
	int unexist = -2;	//好友不存在
	
	int pal = 1;	//已经是好友
	int unpal = -1;	//还不是好友
	
	char user_id[MAX_USER_ID_LENGTH];	//6
	bzero(user_id , sizeof(user_id));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}
	
	if(test_user_id(affair_pthread , user_id) == -1)
	{
		//用户的user_id无效
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
		{
			return -1;
		}
		return FRIEND + FRIEND_A;
	}
	else
	{
		//用户的user_id有效
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
		{
			return -1;
		}
		
		if( test_user_id_in(affair_pthread , user_id) == 1)
		{
			//已经有这个好友 不要在请求添加了
			if( _send(client_sock_fd , (void *)&pal , -1 , 2) == -1)
			{
				return -1;
			}
			return FRIEND + FRIEND_A;
		}
		else
		{
			//没有这个好友 可以请求添加
			if( _send(client_sock_fd , (void *)&unpal , -1 , 2) == -1)
			{
				return -1;
			}
			//继续下面的操作流程
		}

		add_request(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , user_id);
		return FRIEND + FRIEND_A;
	}
	
	return FRIEND + FRIEND_A;
}

int friend_work_d(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int exist = 2;
	int unexist = -2;
	
	char user_id[MAX_USER_ID_LENGTH];	//6
	bzero(user_id , sizeof(user_id));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}
	if(test_user_id_in(affair_pthread , user_id) == -1)
	{
		//用户的user_id不是自己的好友
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
		{
			return -1;
		}
		return FRIEND + FRIEND_D;
	}
	else
	{
		//用户的user_id是自己的好友
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
		{
			return -1;
		}
		//删除好友（双向删除）
		delete_friend(affair_pthread->socket_kit->person->id , affair_pthread->socket_kit->connection , user_id);
		delete_friend(user_id , affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);

		return FRIEND + FRIEND_D;
	}
	
	
	return FRIEND + FRIEND_D;
}

int friend_work_f(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int i;
	
	int friend_count;
	char friend_count_c[MAX_FRIEND_COUNT];	//4
	bzero(friend_count_c , sizeof(friend_count_c));
	char friend_c[MAX_FRIEND_SET_LENGTH];	//200
	bzero(friend_c , sizeof(friend_c));
	
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select friend_count , friend from user where id=");
	strcat(query , affair_pthread->socket_kit->person->id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(friend_count_c , row[0]);
	friend_count = atoi(friend_count_c);
	strcpy(friend_c , row[1]);
	
	mysql_free_result(result);
	
	if( _send(client_sock_fd , (void *)&friend_count , -1 , 2) == -1)
		return -1;
	if( friend_count == 0)
	{
		//该用户没有好友
		return FRIEND + FRIEND_F;
	}
	
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	char user_name[MAX_USER_NAME_LENGTH];	//20
	bzero(user_name , sizeof(user_name));
	
	// i = 0
	strcpy(user_id , strtok(friend_c , "#"));
	bzero(query , sizeof(query));
	strcpy(query , "select name from user where id=");
	strcat(query , user_id);
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	strcpy(user_name , row[0]);
	
	if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		mysql_free_result(result);
		return -1;
	}
	if( _send(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
	{
		mysql_free_result(result);
		return -1;
	}
	mysql_free_result(result);
	
	for(i = 1 ; i<friend_count ; i ++)
	{
		strcpy(user_id , strtok(NULL , "#"));
		
		bzero(query , sizeof(query));
		strcpy(query , "select name from user where id=");
		strcat(query , user_id);
		if( mysql_query(connection , query) )
		{
			fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
			exit(EXIT_FAILURE);
		}
		result = mysql_store_result(connection);
		row = mysql_fetch_row(result);
		strcpy(user_name , row[0]);
		
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
		if( _send(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
		{
			mysql_free_result(result);
			return -1;
		}
		
		mysql_free_result(result);
	}
	
	return FRIEND + FRIEND_F;
}

int friend_work_n(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	char user_name[MAX_USER_NAME_LENGTH];	//20
	bzero(user_name ,sizeof(user_name));
	
	if( _recv(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
	{
		return -1;
	}
	
	int i;
	int match_count;
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select id , name from user where name like '%");
	strcat(query , user_name);
	strcat(query , "%'");
	
	if( mysql_query(connection , query) )
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
		return FRIEND + FRIEND_N;
	}
	else
	{
		for(i = 0 ; i<match_count ; i ++)
		{
			mysql_data_seek(result , i);
			row = mysql_fetch_row(result);
			
			strcpy(user_id , row[0]);
			strcpy(user_name , row[1]);
			
			if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
			{
				mysql_free_result(result);
				return -1;
			}
			if( _send(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
			{
				mysql_free_result(result);
				return -1;
			}
		}
		
		mysql_free_result(result);
	}
	
	
	
	return FRIEND + FRIEND_N;
}

int friend_work_l(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int exist = 2;
	int unexist = -2;
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}

	if(test_user_id_in(affair_pthread , user_id) == -1)
	{
		//不是自己的好友
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
		{
			return -1;
		}
		return FRIEND + FRIEND_L;
	}
	else
	{
		//是自己的好友
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
		{
			return -1;
		}
		
		int collect_count;
		collect_count = get_collect_count(affair_pthread->socket_kit->connection , user_id);

		if( _send(client_sock_fd , (void *)&collect_count , -1 , 2) == -1)
		{
			return -1;
		}
		
		if(collect_count == 0)
		{
			//无须继续
			return FRIEND + FRIEND_L;
		}
		
		char user_collect_c[MAX_COLLECT_MUSIC_LENGTH];	//100
		bzero(user_collect_c , sizeof(user_collect_c));
		get_user_collect(affair_pthread->socket_kit->connection , user_id , user_collect_c);
		
		MYSQL *connection = affair_pthread->socket_kit->connection;
		MYSQL_RES *result;
		MYSQL_ROW row;

		char *music_id_c;
		music_id_c = strtok(user_collect_c , "#");
		if(music_id_c == NULL)
			return FRIEND + FRIEND_L;
		
		char query[MAX_LENGTH_QUERY_SELECT];	//80
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
		while(1)
		{
			mysql_free_result(result);
			
			music_id_c = strtok(NULL , "#");
			if(music_id_c == NULL)
				return FRIEND + FRIEND_L;

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
		
		return FRIEND + FRIEND_L;
	}

	return FRIEND + FRIEND_L;
}

int friend_work_c(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int exist = 2;
	int unexist = -2;
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}

	if(test_user_id_in(affair_pthread , user_id) == -1)
	{
		//不是自己的好友
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
		{
			return -1;
		}
		return FRIEND + FRIEND_C;
	}
	else
	{
		//是自己的好友
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
		{
			return -1;
		}
		
		char user_collect_c[MAX_COLLECT_MUSIC_LENGTH];	//100
		bzero(user_collect_c , sizeof(user_collect_c));
		get_user_collect(affair_pthread->socket_kit->connection , user_id , user_collect_c);

		//拷贝给自己
		update_user_collect(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , user_collect_c);
		
		return FRIEND + FRIEND_C;
	}
	
	return FRIEND + FRIEND_C;
}

int friend_work_u(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int exist = 2;
	int unexist = -2;
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}

	if(test_user_id_in(affair_pthread , user_id) == -1)
	{
		//不是自己的好友
		if( _send(client_sock_fd , (void *)&unexist , -1 , 2) == -1)
		{
			return -1;
		}
		return FRIEND + FRIEND_U;
	}
	else
	{
		//是自己的好友
		if( _send(client_sock_fd , (void *)&exist , -1 , 2) == -1)
		{
			return -1;
		}
		
		int i;
		int upload_count;
		upload_count = get_upload_count(affair_pthread->socket_kit->connection , user_id);
		
		if( _send(client_sock_fd , (void *)&upload_count , -1 , 2) == -1)
		{
			return -1;
		}
		
		MYSQL *connection = affair_pthread->socket_kit->connection;
		MYSQL_RES *result;
		MYSQL_ROW row;
		
		char query[MAX_LENGTH_QUERY_SELECT];	//80
		bzero(query , sizeof(query));
		strcpy(query ,  "select music_id , music_name from upload_");
		strcat(query , user_id);
		
		if( mysql_query(connection , query) )
		{
			fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
			exit(EXIT_FAILURE);
		}
		result = mysql_store_result(connection);
		
		for(i = 0 ; i<upload_count ; i ++)
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
	}
	
	return FRIEND + FRIEND_U;
}

int friend_work_s(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int pal_id_exist = 2;
	int pal_id_unexist = -2;
	int music_id_exist = 2;
	int music_id_unexist = -2;
	
	int music_id;
	char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(music_id_c , sizeof(music_id_c));
	char user_id[MAX_USER_ID_LENGTH];		//6
	bzero(user_id , sizeof(user_id));
	char music_name[MUSIC_NAME_MAX_LENGTH];	//60
	bzero(music_name , sizeof(music_name));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}

	if(test_user_id_in(affair_pthread , user_id) == -1)
	{
		//不是自己的好友
		if( _send(client_sock_fd , (void *)&pal_id_unexist , -1 , 2) == -1)
		{
			return -1;
		}
		
		return FRIEND + FRIEND_S;
	}
	else
	{
		//是自己的好友
		if( _send(client_sock_fd , (void *)&pal_id_exist , -1 , 2) == -1)
		{
			return -1;
		}
		//接收music_id
		if( _recv(client_sock_fd , (void *)&music_id , -1 , 2) == -1)
		{
			return -1;
		}

		//判断好友是否有这个音乐
		if( test_upload_music_id(affair_pthread->socket_kit->connection , user_id , music_id) == -1)
		{

			//好友没有这个音乐
			if( _send(client_sock_fd , (void *)&music_id_unexist , -1 , 2 ) == -1)
			{
				return -1;
			}
			return FRIEND + FRIEND_S;
		}
		else
		{
			//好友有这个音乐
			if( _send(client_sock_fd , (void *)&music_id_exist , -1 , 2 ) == -1)
			{
				return -1;
			}
	
			get_music_path_name_upload(affair_pthread->socket_kit->connection , user_id , music_id , NULL , music_name);

			if( _send(client_sock_fd , (void *)music_name , sizeof(music_name) , 3) == -1)
			{
				return -1;
			}
			return FRIEND + FRIEND_S;
		}
	}
	
	return FRIEND + FRIEND_S;
}

int friend_work_t(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	
	int mode;
	if( _recv(client_sock_fd , (void *)&mode , -1 , 2) == -1)
	{
		return -1;
	}
	
	switch(mode)
	{
		case FRIEND_T_l:
			return friend_work_t_l(affair_pthread);
			break;
			
		case FRIEND_T_a:
			return friend_work_t_a(affair_pthread);
			break;
			
		default:
			;	//impossible
			break;
	}
	
	return FRIEND + FRIEND_T;
}

int friend_work_t_l(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int request_count;
	
	request_count = get_request_count(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);
	if( _send(client_sock_fd , (void *)&request_count , -1 , 2) == -1)
	{
		return -1;
	}
	
	if(request_count == 0)
	{
		//没有请求
		return FRIEND + FRIEND_T;
	}
	else
	{
		//有请求
		char *p;
		int i;
		char user_id[MAX_USER_ID_LENGTH];		//6
		bzero(user_id , sizeof(user_id));
		char user_name[MAX_USER_NAME_LENGTH];	//20
		bzero(user_name , sizeof(user_name));
		
		char request[MAX_REQUEST_LENGTH];	//200
		bzero(request , sizeof(request));
		get_request(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , request);
		
		p = strtok(request , "#");
		strcpy(user_id , p);
		get_user_name_with_id(affair_pthread->socket_kit->connection , user_id , user_name);
		if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
		{
			return -1;
		}
		if( _send(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
		{
			return -1;
		}
		
		for(i = 1 ; i<request_count ; i ++)
		{
			p = strtok(NULL , "#");
			strcpy(user_id , p);
			get_user_name_with_id(affair_pthread->socket_kit->connection , user_id , user_name);
			if( _send(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
			{
				return -1;
			}
			if( _send(client_sock_fd , (void *)user_name , sizeof(user_name) , 3) == -1)
			{
				return -1;
			}
		}
		
		return FRIEND + FRIEND_T;
	}
	return FRIEND + FRIEND_T;
}

int friend_work_t_a(struct pthread_number *affair_pthread)
{
	int client_sock_fd = affair_pthread->socket_kit->client_sock_fd;
	int request = 1;	//请求列表中有
	int unrequest = -1;	//请求列表中没有
	char user_id[MAX_USER_ID_LENGTH];	//6
	bzero(user_id , sizeof(user_id));
	
	if( _recv(client_sock_fd , (void *)user_id , sizeof(user_id) , 3) == -1)
	{
		return -1;
	}
	
	if(test_user_id_request(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , user_id) == 1)
	{
		//在请求列表中 予以同意
		if( _send(client_sock_fd , (void *)&request , -1 , 2) == -1)
		{
			return -1;
		}

		delete_request(affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id , user_id);	//先从请求列表中删除
		add_friend(affair_pthread->socket_kit->person->id , affair_pthread->socket_kit->connection , user_id);		//在自己的好友中添加
		add_friend(user_id , affair_pthread->socket_kit->connection , affair_pthread->socket_kit->person->id);		//在好友的好友中添加自己
	}
	else
	{
		//不在请求列表中 不能同意
		if( _send(client_sock_fd , (void *)&unrequest , -1 , 2) == -1)
		{
			return -1;
		}
		return FRIEND + FRIEND_T;
	}
	
	return FRIEND + FRIEND_T;
}

