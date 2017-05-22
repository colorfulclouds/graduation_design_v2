#ifndef CHECK
#define CHECK
#include "check.h"
#endif

char check_user(struct user *one , MYSQL *connection , void *_one)
{
	int user_status;	//有效用户的登录状态
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query) );
	
	strcpy(query , "select * from user where id = '");
	strcat(query , one->id);
	strcat(query , "' and password = '");
	strcat(query , one->password);
	strcat(query , "'");

	if(sql_exec(connection , query , 1 , _one) != 0)
	{
		//数据库有这个用户的信息
//		user_status = get_user_status(connection , one->id);	//获取用户的登录状态
	
		return 'y';	
	}
	else
		return 'n';	//数据库没有这个用户的信息
}

int check_new_user_email(int client_sock_fd  , struct user *one , MYSQL *connection , char *result)
{
	char yes = 'y';
	char no = 'n';
	
	int test_result;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query) );
	
	if( _recv(client_sock_fd , (void *)one->email , sizeof(one->email) , 3) == -1)
		return -1;
	
	strcpy(query , "select * from user where email = '");
	strcat(query , one->email);
	strcat(query , "'");

	if(sql_exec(connection , query , 1 , NULL) != 0)
	{
		//这个邮箱有人用了，不能再用了
		if( _send(client_sock_fd , (void *)&yes , -1 , 1) == -1)
			return -1;
		
		*result = 'y';
		return 1;
	}
	else
	{
		//这个新用户注册使用的邮箱没有人使用，所以这个邮箱可以让新用户使用
		if( _send(client_sock_fd , (void *)&no , -1 , 1) == -1 )
			return -1;
	
		*result = 'n';
		return 1;
	}
	
	return 1;
}

void add_new_user(struct user *one , MYSQL *connection)
{
	char temp_age[3];
	bzero(temp_age , sizeof(temp_age) );
	sprintf(temp_age , "%d" , one->age);
	
	char query[MAX_LENGTH_QUERY_INSERT];	//200
	bzero(query , sizeof(query) );
	
	strcpy(query , "insert into user(password,email,name,age,sex) values('");
	strcat(query , one->password);
	strcat(query , "','");
	strcat(query , one->email);
	strcat(query , "','");
	strcat(query , one->name);
	strcat(query , "',");
	strcat(query , temp_age);
	strcat(query , ",'");
	query[strlen(query)] = one->sex;
	query[strlen(query) + 1] = '\0';
	strcat(query , "')");
	
	sql_exec(connection , query , 2 , NULL);
}

void get_user_id__reg_time(struct user *one , MYSQL *connection)
{
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query) );
	
	strcpy(query , "select * from user where email = '");
	strcat(query , one->email);
	strcat(query , "'");
	
	sql_exec(connection , query , 1 , one);
}

void add_user_upload_dir(struct user *one , MYSQL *connection)
{
	char mkdir_user_upload_dir[MAX_LENGTH_REGISTER_SUCCESS * 2];	//160
	strcpy(mkdir_user_upload_dir , "/home/leaf/graduation_design/upload/");
	strcat(mkdir_user_upload_dir , one->id);
	strcat(mkdir_user_upload_dir , "/");
	
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	
	strcpy(query , "update user set upload_dir='");
	strcat(query , mkdir_user_upload_dir);
	strcat(query , "' where id=");
	strcat(query , one->id);
	
	sql_exec(connection , query , 2 , NULL);
}

void create_upload_table(struct user *one , MYSQL *connection)
{
	char table_name[MAX_TABLE_NAME_LENGTH];	//20
	bzero(table_name , sizeof(table_name));
	strcpy(table_name , "upload_");
	strcat(table_name , one->id);
	
	char query[MAX_LENGTH_QUERY_CREATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "create table ");
	strcat(query , table_name);
	strcat(query , "(music_id int(11) not null primary key auto_increment,");
	strcat(query , "music_name varchar(60))");

	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}


