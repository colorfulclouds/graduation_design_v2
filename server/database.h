//DATABASE
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef USER
#define USER
#include "user.h"
#endif

#ifndef HANDLE
#define HANDLE
#include "handle.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

void init_database(MYSQL *connection);

int sql_exec(MYSQL *connection , const char *query  , int flag , void *one);

void modify_info(MYSQL *connection , char*user_id , struct user *one);	//修改用户信息（数据库）

int get_random_music_id(MYSQL *connection);					 	//用户没有指出需要哪一首音乐 就随机一首

int get_music_path_name(int music_id , MYSQL *connection , char *music_path_t , char *music_name_t);	//得到指定音乐的目录和名称（从普通音乐库中得到）

int get_music_path_name_upload(MYSQL *connection , char *user_id , int music_id , char *music_path_t , char *music_name_t);	//得到指定音乐的目录和名称（从用户自己上传的音乐库中得到）

int test_music_id(int music_id , MYSQL *connection);		 	//测试music_id的合法性

void get_user_upload_dir(struct pthread_number *affair_pthread , char *user_upload_dir);				//利用该登录用户的id获取上传目录

void get_user_collect(MYSQL *connection , char *user_id , char *user_collect_c);					//查看用户收藏音乐的music_id集合

void add_user_collect(struct pthread_number *affair_pthread , int music_id);							//用户新增音乐收藏

void delete_user_collect(struct pthread_number *affair_pthread , char *user_collect_c , char *music_id_c , int collect_count); //用户删除指定收藏

int get_collect_count(MYSQL *connection , char *user_id);	//得到用户收藏音乐的数目

void erase_sub_string(char *main_string , char *sub_string);//从收藏中删除指定的music_id 或者从好友列表中删除指定的好友 或者从请求列表中删除user_id

void add_user_upload(struct pthread_number *affair_pthread , char *music_name);							//用户上传音乐后 在数据库中添加

int get_upload_count(MYSQL *connection , char *user_id);	//得到用户上传音乐的数目

int test_upload_music_id(MYSQL *connection , char *user_id , int music_id);							//判断指定的music_id是否是用户曾经上传过的

void delete_user_upload(struct pthread_number *affair_pthread , int music_id);							//删除用户指定上传过的音乐

int test_user_id(struct pthread_number *affair_pthread , char *user_id);								//测试用户添加的user_id是否有效

void add_friend(char *self_id , MYSQL *connection , char *user_id);										//用户添加好友

int test_user_id_in(struct pthread_number *affair_pthread , char *user_id);								//测试这个用户是否为好友

void delete_friend(char *self_id , MYSQL *connection , char *user_id);									//用户删除好友

void update_user_collect(MYSQL *connection , char *user_id , char *user_collect_c);						//更新好友的收藏

void erase_collect_repetition(char *self_collect , char *pal_collect);									//处理两个有重复的收藏列表

int get_collect_count_collect_c(char *collect);							//从收藏集合中 得到收藏的个数

void increase_download_count(MYSQL *connection , char *user_id , int music_id);	//下载音乐后 下载次数加1 用户列表中的下载此处也加1

void increase_listen_count(MYSQL *connection , char *user_id , int music_id);	//在线试听音乐后 music和用户的试听次数加1
//下面的函数暂时未用
int get_user_status(MYSQL *connection , char *user_id);					//获取用户的登录状态

int get_music_style(MYSQL *connection , int music_id);								//根据music_id获得音乐类型（广库）

void increase_style_count(MYSQL *connection , char *user_id , int music_style);		//增加用户听某类型音乐的数目

int get_like_music_style(MYSQL *connection , char *user_id);			//获取用户最喜欢的音乐类型

//下列和添加好友有关
int get_request_count(MYSQL *connection , char *user_id);				//获取请求添加的数目

void get_request(MYSQL *connection , char *user_id , char *request);	//获取请求列表

void get_user_name_with_id(MYSQL *connection , char *user_id , char *user_name);	//根据用户id得到name

void add_request(MYSQL *connection , char *self_id , char *user_id );				//先欲添加的好友通知

int test_user_id_request(MYSQL *connection , char *self_id , char *user_id);		//测试是否在自己的请求列表中

void delete_request(MYSQL *connection , char *self_id , char *user_id);				//同意添加后 从请求列表中删除

//评论相关
void get_comment_file_path(MYSQL *connection , char *comment_path , int music_id);		//获取评论文件的路径

void add_comment(MYSQL *connection , char *user_id , char *comment , int music_id);	//将评论更新至服务器

int get_comment_count(MYSQL *connection , int music_id);				//获取指定音乐的评论数目