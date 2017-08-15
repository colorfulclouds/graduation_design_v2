#ifndef DATABASE
#define DATABASE
#include "database.h"
#endif

//连接数据库
void init_database(MYSQL *connection)
{
	mysql_init(connection);
	
	//连接数据库 API
	if( !(connection = mysql_real_connect(connection , "localhost" , "admin_name" , "admin_password!" , "database_name" , 0 , NULL , 0) ) )
    {
        fprintf( stderr,"[11001]%s\n",mysql_error(connection) );
        exit(EXIT_FAILURE);
    }
}

//flag=1 : select
//flag=2 : insert
//flag=3 : delete
//flag=4 : update 
int sql_exec(MYSQL *connection , const char *query , int flag , void *one)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	int result_number;
	
	struct user *person;
	
	switch(flag)
	{
		case 1:
			if( mysql_query(connection , query) ) //success return 0
			{
				fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
				exit(EXIT_FAILURE);
			}
			result = mysql_store_result(connection);	
			result_number = mysql_num_rows(result);		//result is the number of result
			//为了在用户登录之后，将用户的信息传回客户端所增加的
			if(one)
			{
				person = (struct user *)one;
				row = mysql_fetch_row(result);
	
				strcpy(person->id , row[0]);
				strcpy(person->password , row[1]);
				strcpy(person->email , row[2]);
				strcpy(person->name , row[3]);
				person->age = atoi(row[4]);
				person->sex = row[5][0];
				strcpy(person->reg_time , row[6]);
			}
			mysql_free_result(result);
			return result_number;
//printf("result:%d\n" , num);
			break;
		case 2:
			if( mysql_query(connection , query) ) //success return 0
			{
				fprintf(stderr,"[11003]%s\n",mysql_error(connection) );
				exit(EXIT_FAILURE);
			}
			//insert
			break;
		
		//un used
		case 3:
			//delete
			break;
			
		case 4:
			//update
			break;
			
		default:
			//impossible here
			break;
	}
}

//4.28 新增 用户修改个人信息
void modify_info(MYSQL *connection , char *user_id , struct user *one)
{
	char temp_age[3];
	bzero(temp_age , sizeof(temp_age) );
	sprintf(temp_age , "%d" , one->age);
	
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "update user set password ='");
	strcat(query , one->password);
	strcat(query , "' , name = '");
	strcat(query , one->name);
	strcat(query , "' , age = ");
	strcat(query , temp_age);
	strcat(query , ", sex = '");
	query[strlen(query)] = one->sex;
	strcat(query , "' where id =");
	strcat(query , user_id);

	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

int test_music_id(int music_id , MYSQL *connection)
{
	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c) );
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_SELECT];
	strcpy(query , "select * from music where id = ");
	strcat(query , music_id_c);
	
	MYSQL_RES *result;
	int result_number;
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
	result = mysql_store_result(connection);
	result_number = mysql_num_rows(result);
	mysql_free_result(result);
	
	if(result_number == 0)
		return -1;
	else
		return 1;
}

int get_random_music_id(MYSQL *connection)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	int result_number_max;
	int result_number_min;
	int random_music_id;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	
	//得到音乐music_id的最大值
	bzero(query , sizeof(query) );
	strcpy(query , "select * from music order by id desc limit 1");
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	result_number_max = atoi(row[0]);
	mysql_free_result(result);
	//得到音乐music_id的最小值
	bzero(query , sizeof(query) );
	strcpy(query , "select * from music order by id limit 1");
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	result_number_min = atoi(row[0]);
	mysql_free_result(result);
	
	srand((unsigned) time(NULL));
	
	while(1)
	{
		random_music_id = rand() % (result_number_max - result_number_min + 1) + result_number_min;
		if( test_music_id(random_music_id , connection) == 1)
			break;
	}
	return random_music_id;
}

//从广库下载使用
int get_music_path_name(int music_id , MYSQL *connection , char *music_path_t , char *music_name_t)
{
	if(music_id <= 0)
	{
		printf("wrong music_id:%d\n", music_id);
		return -1;
	}
	char *music_path = (char *)malloc(sizeof(char) * MUSIC_PATH_MAX_LENGTH);
	bzero(music_path , sizeof(music_path));
	char *music_name = (char *)malloc(sizeof(char) * MUSIC_NAME_MAX_LENGTH);
	bzero(music_name , sizeof(music_name));
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c) );
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_SELECT];
	strcpy(query , "select * from music where id = ");
	strcat(query , music_id_c);
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(music_path , row[2]);
	strcpy(music_name , row[1]);
	mysql_free_result(result);
	
	if(music_path_t != NULL)
		strcpy(music_path_t , music_path);
	
	if(music_name_t != NULL)
		strcpy(music_name_t , music_name);
	
	free(music_path);
	free(music_name);
	
	return 1;
}

//从用户的上传库下载使用
int get_music_path_name_upload(MYSQL *connection , char *user_id , int music_id  , char *music_path_t , char *music_name_t)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char music_path[MUSIC_PATH_MAX_LENGTH];	//100
	bzero(music_path , sizeof(music_path));
	char music_name[MUSIC_NAME_MAX_LENGTH];	//60
	bzero(music_name , sizeof(music_name));
	
	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c) );
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_SELECT];
	bzero(query , sizeof(query));
	strcpy(query , "select upload_dir from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	strcpy(music_path , row[0]);
	mysql_free_result(result);
	
	bzero(query , sizeof(query));
	strcpy(query , "select music_name from upload_");
	strcat(query , user_id);
	strcat(query , " where music_id =");
	strcat(query , music_id_c);
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	strcpy(music_name , row[0]);
	mysql_free_result(result);
	
	if(music_path_t != NULL)
		strcpy(music_path_t , music_path);
	
	if(music_name_t != NULL)
		strcpy(music_name_t , music_name);
	
	return 1;
}

void get_user_upload_dir(struct pthread_number *affair_pthread , char *user_upload_dir)
{
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	strcpy(query , "select upload_dir from user where id = '");
	strcat(query , affair_pthread->socket_kit->person->id);
	strcat(query , "'");
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(user_upload_dir , row[0]);	//获取该登录用户的上传目录
	mysql_free_result(result);
}

void get_user_collect(MYSQL *connection , char *user_id , char *user_collect_c)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];
	bzero(query , sizeof(query));
	
	strcpy(query , "select collect from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(user_collect_c , row[0]);
	mysql_free_result(result);
}

void add_user_collect(struct pthread_number *affair_pthread , int music_id)
{
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c));
	sprintf(music_id_c , "%d" , music_id);
	strcat(music_id_c , "#");

	int collect_count;
	char collect_count_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(collect_count_c , sizeof(collect_count_c));
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];		//160
	bzero(query , sizeof(query));
	char user_collect_c[MAX_COLLECT_MUSIC_LENGTH];	//100
	bzero(user_collect_c , sizeof(user_collect_c));
	
	strcpy(query , "select collect , collect_count from user where id =");
	strcat(query , affair_pthread->socket_kit->person->id);

	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n" , mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);

	if( strlen(row[0]) != 0)
		strcpy(user_collect_c , row[0]);
	strcat(user_collect_c ,music_id_c);

	strcpy(collect_count_c , row[1]);
	collect_count = atoi(collect_count_c);
	collect_count ++;
	sprintf(collect_count_c , "%d" , collect_count);
	
	mysql_free_result(result);
	
	//将新的收藏的music_id更新至数据库中
	bzero(query , sizeof(query));
	strcpy(query , "update user set collect ='");
	strcat(query , user_collect_c);
	strcat(query , "'");
	strcat(query , ", collect_count =");
	strcat(query , collect_count_c);
	strcat(query , " where id = ");
	strcat(query , affair_pthread->socket_kit->person->id);
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

void erase_sub_string(char *main_string , char *sub_string)
{
	strcat(sub_string , "#");

	char *pstr = strstr(main_string , sub_string);

	for(pstr ; pstr != NULL ; pstr = strstr(pstr , sub_string) )
	{
		char *temp = pstr;
		char *ptemp = pstr;
		ptemp += strlen(sub_string);
		
		while(*temp != '\0')
		{
			*temp = *ptemp;
			temp ++;
			ptemp ++;
		}

	}
}

void delete_user_collect(struct pthread_number *affair_pthread , char *user_collect_c , char *music_id_c , int collect_count)
{

	erase_sub_string(user_collect_c , music_id_c);
	collect_count -= 1;

	char collect_count_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(collect_count_c , sizeof(collect_count_c));
	sprintf(collect_count_c , "%d" , collect_count);
	
	
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	bzero(query , sizeof(query));
	strcpy(query , "update user set collect ='");
	strcat(query , user_collect_c);
	strcat(query , "'");
	strcat(query , ", collect_count =");
	strcat(query , collect_count_c);
	strcat(query , " where id = ");
	strcat(query , affair_pthread->socket_kit->person->id);
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

int get_collect_count(MYSQL *connection , char *user_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	int collect_count;
	char collect_count_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(collect_count_c , sizeof(collect_count_c));
	
	char query[MAX_LENGTH_QUERY_SELECT];
	bzero(query , sizeof(query));
	
	strcpy(query , "select collect_count from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(collect_count_c , row[0]);
	collect_count = atoi(collect_count_c);
	
	mysql_free_result(result);
	return collect_count;
}

void add_user_upload(struct pthread_number *affair_pthread , char *music_name)
{
	MYSQL *connection = affair_pthread->socket_kit->connection;
	
	char query[MAX_LENGTH_QUERY_INSERT];	//200
	bzero(query , sizeof(query));
	strcpy(query , "insert into upload_");
	strcat(query , affair_pthread->socket_kit->person->id);
	strcat(query , "(music_name) values('");
	strcat(query , music_name);
	strcat(query , "')");
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

int get_upload_count(MYSQL *connection , char *user_id)
{
	MYSQL_RES *result;
	
	int upload_count;
	
	char query[MAX_LENGTH_QUERY_SELECT];
	bzero(query , sizeof(query));
	strcpy(query , "select * from upload_");
	strcat(query , user_id);

	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection );
	upload_count = mysql_num_rows(result);
	
	mysql_free_result(result);
	return upload_count;
}

int test_upload_music_id(MYSQL *connection , char *user_id , int music_id)
{
	char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(music_id_c , sizeof(music_id_c));
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	bzero(query , sizeof(query));
	strcpy(query , "select * from upload_");
	strcat(query , user_id);
	strcat(query , " where music_id =");
	strcat(query , music_id_c);
	
	MYSQL_RES *result;
	int count;
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002pp]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	count = mysql_num_rows(result);
	mysql_free_result(result);

	if(count == 0)
		return -1;
	else
		return 1;
}

void delete_user_upload(struct pthread_number *affair_pthread , int music_id)
{
	char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(music_id_c , sizeof(music_id_c));
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_DELETE];	//200
	bzero(query , sizeof(query));
	strcpy(query , "delete from upload_");
	strcat(query , affair_pthread->socket_kit->person->id);
	strcat(query , " where music_id =");
	strcat(query , music_id_c);
	
	MYSQL *connection = affair_pthread->socket_kit->connection;
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
	//这里可以继续进行从磁盘上删除
	//需要先从user表中获得upload目录 然后进行rm
}

int test_user_id(struct pthread_number *affair_pthread , char *user_id)
{
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	
	int count;
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select * from user where id=");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
	result = mysql_store_result(connection);
	count = mysql_num_rows(result);
	mysql_free_result(result);
	
	if(count == 0)
		return -1;
	else
		return 1;
	
}

void add_friend(char *self_id , MYSQL *connection , char *user_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	bzero(query , sizeof(query));
	strcpy(query , "select friend_count , friend from user where id=");
	strcat(query , self_id);
	
	char friend_count_c[MAX_FRIEND_COUNT];	//4
	bzero(friend_count_c , sizeof(friend_count_c));
	int friend_count;
	
	char friend_c[MAX_FRIEND_SET_LENGTH];	//200
	bzero(friend_c , sizeof(friend_c));
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(friend_count_c , row[0]);
	strcpy(friend_c , row[1]);
	
	mysql_free_result(result);
	//更新之前判断一下  如果已经添加过 停止后续操作
	if( strstr(friend_c , user_id) != NULL)
	{
		//曾经添加过
		return ;
	}
	else
	{
		//更新好友列表
		strcat(friend_c , user_id);
		strcat(friend_c , "#");
		//更新好友数目
		friend_count = atoi(friend_count_c);
		friend_count += 1;
		bzero(friend_count_c , sizeof(friend_count_c));
		sprintf(friend_count_c , "%d" , friend_count);
		
		bzero(query , sizeof(query));
		strcpy(query , "update user set friend_count =");
		strcat(query , friend_count_c);
		strcat(query , ", friend ='");
		strcat(query , friend_c);
		strcat(query , "' where id=");
		strcat(query , self_id);

		if( mysql_query(connection , query) ) //success return 0
		{
			fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
			exit(EXIT_FAILURE);
		}
	}
}

int test_user_id_in(struct pthread_number *affair_pthread , char *user_id)
{
	MYSQL *connection = affair_pthread->socket_kit->connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char friend_c[MAX_FRIEND_SET_LENGTH];	//200
	bzero(friend_c , sizeof(friend_c));
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select friend from user where id=");
	strcat(query , affair_pthread->socket_kit->person->id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(friend_c , row[0]);
	mysql_free_result(result);
	if( strstr(friend_c , user_id) == NULL)
	{
		//没有这个好友
		return -1;
	}
	else
	{
		//有这个好友
		return 1;
	}
	return 1;	//impossible
}

void delete_friend(char *self_id , MYSQL *connection , char *user_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	bzero(query , sizeof(query));
	strcpy(query , "select friend_count , friend from user where id=");
	strcat(query , self_id);
	
	char friend_count_c[MAX_FRIEND_COUNT];	//4
	bzero(friend_count_c , sizeof(friend_count_c));
	int friend_count;
	
	char friend_c[MAX_FRIEND_SET_LENGTH];	//200
	bzero(friend_c , sizeof(friend_c));
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(friend_count_c , row[0]);
	strcpy(friend_c , row[1]);
	
	mysql_free_result(result);
	//更新好友列表
	int length = strlen(user_id);
	erase_sub_string(friend_c , user_id);
	user_id[length] = '\0';		//必须加 否则下次会出现#
	//更新好友数量
	friend_count = atoi(friend_count_c);
	friend_count -= 1;
	bzero(friend_count_c , sizeof(friend_count_c));
	sprintf(friend_count_c , "%d" , friend_count);
	
	bzero(query , sizeof(query));
	strcpy(query , "update user set friend_count =");
	strcat(query , friend_count_c);
	strcat(query , ", friend ='");
	strcat(query , friend_c);
	strcat(query , "' where id=");
	strcat(query , self_id);

	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

void erase_collect_repetition(char *self_collect , char *pal_collect)	//拷贝好友的收藏列表
{
	if( strlen(pal_collect) == 0)
		return ;
	
	if( strlen(self_collect) == 0)
	{
		strcpy(self_collect , pal_collect);
		return ;
	}
	
	char *pal_music_id;
	
	pal_music_id = strtok(pal_collect , "#");
	if(pal_music_id == NULL)
		return ;
	else
	{
		if( strstr(self_collect , pal_music_id) == NULL)
		{
			strcat(self_collect , pal_music_id);
			strcat(self_collect , "#");
		}
	}
	
	while(1)
	{
		pal_music_id = strtok(NULL , "#");
		if(pal_music_id == NULL)
			break;
		else
		{
			if( strstr(self_collect , pal_music_id) == NULL)
			{
				strcat(self_collect , pal_music_id);
				strcat(self_collect , "#");
			}
		}
	}
}

int get_collect_count_collect_c(char *collect)
{
	int i;
	int length = strlen(collect);
	int collect_count = 0;
	
	for(i = 0 ; i<length ; i ++)
		if(collect[i] == '#')
			collect_count ++;
		
	return collect_count;
}

void update_user_collect(MYSQL *connection , char *user_id , char *user_collect_c)
{
	char user_collect_c_[MAX_COLLECT_MUSIC_LENGTH];	//100
	bzero(user_collect_c_ , sizeof(user_collect_c_));
	get_user_collect(connection , user_id , user_collect_c_);
	
	erase_collect_repetition(user_collect_c_ , user_collect_c);
	
	int collect_count = get_collect_count_collect_c(user_collect_c_);
	char collect_count_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(collect_count_c , sizeof(collect_count_c));
	sprintf(collect_count_c , "%d" , collect_count);

	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	bzero(query , sizeof(query));
	strcpy(query , "update user set collect_count =");
	strcat(query , collect_count_c);
	strcat(query , " , collect ='");
	strcat(query , user_collect_c_);
	strcat(query , "' where id =");
	strcat(query , user_id);

	mysql_query(connection , query);
		
}

void increase_download_count(MYSQL *connection , char *user_id , int music_id)
{
	//音乐下载次数属性加1
	char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(music_id_c , sizeof(music_id_c));
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "update music set download_count = download_count + 1 where id =");
	strcat(query , music_id_c);
	
	if( mysql_query(connection , query))
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
	//用户下载次数属性加1
	bzero(query , sizeof(query));
	strcpy(query , "update user set download_count = download_count + 1 where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query))
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
}

void increase_listen_count(MYSQL *connection , char *user_id , int music_id)
{
	//music属性listen_count加1
	char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(music_id_c , sizeof(music_id_c));
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "update music set listen_count = listen_count + 1 where id =");
	strcat(query , music_id_c);
	
	if( mysql_query(connection , query))
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	
	//用户属性listen_count加1
	bzero(query , sizeof(query));
	strcpy(query , "update user set listen_count = listen_count + 1 where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query))
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}
//下面的函数暂时未用
int get_user_status(MYSQL *connection , char *user_id)
{
	if( strcmp(user_id , "10036") == 0)	//匿名用户可以多端登录
		return 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	int user_status;

	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select status from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr , "[11002]%s\n" , mysql_error(connection));
		exit(EXIT_FAILURE);
	}
	
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	mysql_free_result(result);
	
	user_status = atoi(row[0]);
	return user_status;
}

int get_music_style(MYSQL *connection , int music_id)
{
	int music_style;
	char music_id_c[MAX_MUSIC_COUNT_BIT];	//4
	bzero(music_id_c , sizeof(music_id_c));
	sprintf(music_id_c , "%d" , music_id);
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select style from music where id =");
	strcat(query , music_id_c);
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	music_style = atoi(row[0]);
	
	mysql_free_result(result);
	return music_style;
}

void increase_style_count(MYSQL *connection , char*user_id , int music_style)
{
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "update user set ");
	
	switch(music_style)
	{
		case PLAYS_k_pop:
			strcat(query , "pop_2_count = pop_2_count + 1 ");
			break;
		
		case PLAYS_k_rock:
			strcat(query , "rock_4_count = rock_4_count + 1 ");
			break;
			
		case PLAYS_k_emo:
			strcat(query , "emo_8_count = emo_8_count + 1 ");
			break;
			
		case PLAYS_k_girl:
			strcat(query , "girl_16_count = girl_16_count + 1 ");
			break;
			
		case PLAYS_k_boy:
			strcat(query , "boy_32_count = boy_32_count + 1 ");
			break;
			
		default:
			;	//impossible
			break;
	}
	
	strcat(query , " where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

int get_like_music_style(MYSQL *connection , char *user_id)
{
	int like_music_style;
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT * 2];	//160
	bzero(query , sizeof(query));
	strcpy(query , "select pop_2_count , rock_4_count , emo_8_count , girl_16_count , boy_32_count from user where id =");
	strcat(query , user_id);

	if( mysql_query(connection , query) ) //success return 0
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);

	int i;
	int max_listen_style = 0;
	for(i = 0 ; i<5 ; i ++)
	{
		if( atoi(row[i]) > atoi(row[max_listen_style]) )
			max_listen_style = i;
	}
	mysql_free_result(result);
	return 2 * (int)pow(2 , max_listen_style);
}

//单向添加好友相关
int get_request_count(MYSQL *connection , char *user_id)
{
	int request_count;
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select request_count from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	request_count = atoi(row[0]);
	mysql_free_result(result);
	
	return request_count;
}

void get_request(MYSQL *connection , char *user_id , char *request)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select request from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(request , row[0]);
	mysql_free_result(result);
}

void get_user_name_with_id(MYSQL *connection , char *user_id , char *user_name)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select name from user where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);

	strcpy(user_name , row[0]);
	mysql_free_result(result);
}

void add_request(MYSQL *connection , char *self_id , char *user_id )
{
	char user_request[MAX_REQUEST_LENGTH];	//200
	bzero(user_request , sizeof(user_request));
	get_request(connection , user_id , user_request);
	
	if( strstr(user_request , self_id))
	{
		//说明已经请求过了
		return ;
	}
	
	strcat(user_request , self_id);
	strcat(user_request , "#");
	
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "update user set request_count = request_count + 1 , request ='");
	strcat(query , user_request);
	strcat(query , "' where id =");
	strcat(query , user_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

int test_user_id_request(MYSQL *connection , char *self_id , char *user_id)
{
	char request[MAX_REQUEST_LENGTH];	//200
	bzero(request , sizeof(request));
	
	get_request(connection , self_id , request);
	if( strstr(request , user_id) )
	{
		//在请求列表中
		return 1;
	}
	else
	{
		//不在请求列表中
		return -1;
	}
	
	return 1;	//impossible
	
}

void delete_request(MYSQL *connection , char *self_id , char *user_id)
{
	char request[MAX_REQUEST_LENGTH];	//200
	bzero(request , sizeof(request));
	
	get_request(connection , self_id , request);
	
	char temp_user_id[MAX_USER_ID_LENGTH * 2];	//12
	strcpy(temp_user_id , user_id);
	
	erase_sub_string(request , temp_user_id);
	
	char query[MAX_LENGTH_QUERY_UPDATE];	//300
	bzero(query , sizeof(query));
	strcpy(query , "update user set request_count = request_count - 1 , request ='");
	strcat(query , request);
	strcat(query , "' where id =");
	strcat(query , self_id);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

//评论相关
void get_comment_file_path(MYSQL *connection , char *comment_path , int music_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c) );
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select comment_path from music where id =");
	strcat(query , music_id_c);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	strcpy(comment_path , row[0]);
	mysql_free_result(result);
}

void add_comment(MYSQL *connection , char *user_id , char *comment , int music_id)
{
	char comment_path[MAX_COMMENT_FILE_PATH];
	bzero(comment_path , sizeof(comment_path));
	
	get_comment_file_path(connection , comment_path , music_id);
	
	int comment_fd;
	comment_fd = open(comment_path , O_WRONLY | O_CREAT | O_APPEND);
	if(comment_fd < 0)
	{
		perror("fatal error");
		exit(EXIT_FAILURE);
	}
	write(comment_fd , comment , strlen(comment));
	write(comment_fd , "\n" , 1);
	close(comment_fd);
	
	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c) );
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_UPDATE / 3];	//100
	bzero(query , sizeof(query));
	strcpy(query , "update music set comment_count = comment_count + 1 where id =");
	strcat(query , music_id_c);
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
}

int get_comment_count(MYSQL *connection , int music_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	char music_id_c[MAX_MUSIC_COUNT_BIT];
	bzero(music_id_c , sizeof(music_id_c) );
	sprintf(music_id_c , "%d" , music_id);
	
	char query[MAX_LENGTH_QUERY_SELECT];	//80
	bzero(query , sizeof(query));
	strcpy(query , "select comment_count from music where id =");
	strcat(query , music_id_c);
	
	if( mysql_query(connection , query) )
	{
		fprintf(stderr,"[11002]%s\n",mysql_error(connection) );
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(connection);
	row = mysql_fetch_row(result);
	
	int comment_count = atoi(row[0]);
	mysql_free_result(result);
	
	return comment_count;
	
}

