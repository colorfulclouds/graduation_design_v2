//CONST_VALUE
#define MAX_LENGTH_TWENTY 20
#define MAX_LENGTH_THIRTY 30

#define MAX_LENGTH_QUERY_SELECT 80
#define MAX_LENGTH_QUERY_INSERT 200
#define MAX_LENGTH_QUERY_UPDATE 300
#define MAX_LENGTH_QUERY_CREATE	300
#define MAX_LENGTH_QUERY_DELETE 200

#define MAX_LENGTH_REGISTER_SUCCESS 80	//存放注册成功发送邮件的命令的字符串的最大长度

#define MUSIC_NAME_MAX_LENGTH 60	//音乐名字的最大长度
#define MUSIC_PATH_MAX_LENGTH 100	//音乐路径的最大长度

#define MAX_MUSIC_COUNT_BIT 4	//music.c 音乐数目最多为3位数 '\0'留一位

#define CLIENT_IS_ALIVE 1
#define CLIENT_IS_DEAD 0

#define TOP_NUMBER 10	//默认是TOP10
#define LIKE_NUMBER	5	//目前先过去猜你喜欢的前5名（5）
//*****PLAYS********
#define PLAYS_t 4		//获取TOPxx请求
#define PLAYS_s 8		//音乐下载
	//
	#define PLAYS_s_g	2	//下载广库中的音乐
	#define PLAYS_s_p	4	//下载上传库中的音乐
	#define PLAYS_s_f	8	//下载好友库中的音乐
	//
#define PLAYS_o 16		//伪试听
#define PLAYS_d	32		//（未用）
#define PLAYS_u 64		//上传音乐
//#define -PLAYS_o -16	//真试听
#define PLAYS_k 128	//查看不同风格的音乐
	//
	#define PLAYS_k_pop		2	//流行
	#define PLAYS_k_rock	4	//摇滚
	#define PLAYS_k_emo		8	//抒情
	#define PLAYS_k_girl	16	//女声
	#define PLAYS_k_boy		32	//男声
#define MAX_MUSIC_STYLE_LENGTH	4	//音乐类型串最大长度
	//
#define PLAYS_l 256		//用户收藏音乐
#define PLAYS_x 512		//删除标志
#define PLAYS_f	1024	//查找指定音乐名字的音乐
#define PLAYS_r	2048	//猜你喜欢（暂时过去前5名）
#define PLAYS_c 4096	//音乐评论
#define PLAYS_m	8192//修改信息
	//server可能不需要(ex PLAYS_m_q)
	#define PLAYS_m_p 2		//修改密码
	#define PLAYS_m_n 4		//修改昵称
	#define PLAYS_m_a 8		//修改年龄
	#define PLAYS_m_s 16	//修改性别
	#define PLAYS_m_q 32	//退出修改
	//
//******************
//*****FRIEND*******
#define FRIEND		19		//好友间操作

#define FRIEND_A	2		//用户添加好友
#define FRIEND_D	4		//用户删除好友
#define FRIEND_F	8		//用户所有的好友
#define FRIEND_N	16		//利用name模糊查找用户
#define FRIEND_L	32		//查看好友收藏音乐
#define FRIEND_C	64		//拷贝好友的收藏列表
#define FRIEND_U	128		//查看好友上传的音乐
#define FRIEND_S	256		//下载好友上传的音乐
#define FRIEND_T	512	//查看添加自己的用户或同意添加
	//
	#define FRIEND_T_l	2	//查看请求列表
	#define FRIEND_T_a	4	//同意某人的添加
	//

//******************

#define MAX_REQUEST_LENGTH	200	//最大请求列表长度

#define MAX_FRIEND_COUNT	4		//添加还有数量最多位数
#define MAX_USER_ID_LENGTH	6		//用户id最大长度
#define MAX_USER_NAME_LENGTH	20	//用户name最大长度
#define MAX_FRIEND_SET_LENGTH	200	//添加的还有id集合最大长度


#define MUSIC_NAME_MAX_LENGTH 60

#define MAX_SEND_MUSIC_BUFF_LENGTH 20000	//每次发送的数据量

#define LOGOUT 7
#define QUIT 11

#define MAX_BUFF_QUEUE_LENGTH 5			//缓冲队列最多只能有5个在向用户缓冲

#define MAX_USER_UPLOAD_DIR_LENGTH 150	//用户上传目录的最大长度

#define MAX_COLLECT_MUSIC_LENGTH   100	//数据库中收藏音乐字段最大长度

#define MAX_TABLE_NAME_LENGTH 20		//数据表名最大长度

#define MAX_INPUT_BUFF 500				//类缓冲区大小
#define MAX_LENGTH_COMMENT	363			//评论最大长度
#define MAX_COMMENT_FILE_PATH	100		//评论文件的路径长度

//可能没用
#define USER_STATUS_ONLINE	1	//用户在线
#define USER_STATUS_OFFLINE	0	//用户不在线