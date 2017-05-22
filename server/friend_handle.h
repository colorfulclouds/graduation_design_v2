//FRIEND_HANDLE
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef HANDLE
#define HANDLE
#include "handle.h"
#endif

#ifndef DATABASE
#define DATABASE
#include "database.h"
#endif

int friend_work_a(struct pthread_number *affair_pthread);		//用户添加好友

int friend_work_d(struct pthread_number *affair_pthread);		//用户删除好友

int friend_work_f(struct pthread_number *affair_pthread);		//用户查看好友

int friend_work_n(struct pthread_number *affair_pthread);		//用户用name模糊查找

int friend_work_l(struct pthread_number *affair_pthread);		//用户查看好友的收藏音乐

int friend_work_c(struct pthread_number *affair_pthread);		//用户拷贝好友的收藏列表

int friend_work_u(struct pthread_number *affair_pthread);		//用户查看好友上传的音乐

int friend_work_t(struct pthread_number *affair_pthread);		//用户查看请求列表或同意添加

int friend_work_t_l(struct pthread_number *affair_pthread);		//用户查看请求列表

int friend_work_t_a(struct pthread_number *affair_pthread);		//用户同意添加