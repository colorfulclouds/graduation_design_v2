//CONST_VALUE
#define MODE_LOAD		2	//登录使用模式（全功能）
#define MODE_ANONYMOUS	4	//匿名登录模式（受限功能）
#define MODE_ALONE		8	//单机模式（play可用）

#define MAX_LENGTH_TWENTY 20
#define MAX_LENGTH_THIRTY 30

#define MAX_INPUT_BUFF 500	//类缓冲区大小

#define MAX_LENGTH_ERROR_REASON_BUFF 100
#define MAX_LENGTH_ERROR_CODE_BUFF 6

#define MAX_MANUAL_BUFF 100

#define SERVER_IS_ALIVE 1
#define SERVER_IS_DOWNTIME 0

#define MAX_MUSIC_COUNT_BIT 3	//music.c 音乐数目最多为3位数

#define CONF_PATH_MAX_LENGTH 100
#define MUSIC_NAME_MAX_LENGTH 60		//音乐名字的最大长度
#define MUSIC_PATH_MAX_LENGTH 100		//音乐路径的最大长度

#define MUSIC_FORMAT_MAX_LENGTH 5		//音乐格式的最长长度
#define MUSIC_FORMAT_MAX_COUNT 4		//音乐格式的最大种类数

#define MUSIC_STYLE_NAME_MAX_LENGTH 10	//音乐风格描述最大长度

#define MUSIC_DIRECT_DIR_MAX_COUNT 10	//用户机器上直接用音乐目录数量
#define MAX_ONE_DIR_MUSIC_COUNT 100	//一个直接目录下音乐的最大数量

#define MAX_DIRECTORY_DEPTH 16		//最深目录深度

#define SHELL_MAX_LENGTH 100		//shell命令的最大长度
#define SHELL_MAX_MUSIC_COUNT 10	//一个shell命令中可以含有的最大音乐个数

#define UPDATE_YFF_CONF_F 15		//进行检测一次配置文件的周期

#define RECYCLE_NO -1	//播放不循环
#define RECYCLE_YES 1	//播放循环

#define PLAY 3		//本地音乐操作标识符
#define LOGOUT 7	//注销标识符
#define QUIT 11		//退出标识符
#define CLS 13		//清屏标识符
#define PLAYS 17	//网络服务操作标识符
#define FRIEND 19	//好友操作标识符

#define PLAY_m 1 	//用户机器上
#define PLAY_s 2	//服务器上	//作废 见plays命令
#define PLAY_r 4	//循环播放
#define PLAY_R 8	//随机播放
#define PLAY_q 16	//显示播放队列
#define PLAY_f 32	//上一首
#define PLAY_l 64	//下一首
#define PLAY_d 128	//本机配置文件中指定的音乐
#define PLAY_n 256	//指定两个数字就可以进行播放了，前提是根据-d参数
#define PLAY_a 512	//向播放队列中添加音乐
#define PLAY_c 1024	//清空播放队列
#define PLAY_p 2048	//添加非配置文件中目录中的音乐（与a参数合用）
#define PLAY_t 4096	//更改播放状态 暂停or继续
#define PLAY_x 8192	//配置文件增加目录

//*****PLAYS********
#define PLAYS_t 	4	//获取TOPxx
#define PLAYS_s		8	//音乐下载
	//
	#define PLAYS_s_g	2	//下载广库中的音乐
	#define PLAYS_s_p	4	//下载上传库中的音乐
	#define PLAYS_s_f	8	//下载好友库中的音乐
	//
#define PLAYS_o		16	//将来可能是在线播放
#define PLAYS_d 	32	//查看音乐下载队列
#define PLAYS_u 	64	//上传音乐
#define PLAYS_k		128	//查看不同风格的音乐
	//
	#define PLAYS_k_pop		2	//流行
	#define PLAYS_k_rock	4	//摇滚
	#define PLAYS_k_emo		8	//抒情
	#define PLAYS_k_girl	16	//女声
	#define PLAYS_k_boy		32	//男声
	//
#define PLAYS_l 	256	//用户收藏音乐
#define PLAYS_x		512	//删除标志
#define PLAYS_f		1024//查找指定音乐名字的音乐
#define PLAYS_r		2048//猜你喜欢（暂时获取前5名）
#define PLAYS_c		4096//音乐评论
#define PLAYS_m		8192//修改信息
	//
	#define PLAYS_m_p 2	//修改密码
	#define PLAYS_m_n 4	//修改昵称
	#define PLAYS_m_a 8	//修改年龄
	#define PLAYS_m_s 16//修改性别
	#define PLAYS_m_q 32//退出修改
	//
#define PLAYS_i		16384	//查看自己的信息
//******************
//*****FRIEND*******
#define FRIEND_A	2	//添加好友
#define FRIEND_D	4	//删除好友
#define FRIEND_F	8	//查看所有的好友
#define FRIEND_N	16	//利用name模糊查找
#define FRIEND_L	32	//查看自己好友收藏的音乐
#define FRIEND_C	64	//拷贝好友的收藏列表
#define FRIEND_U	128	//查看好友上传的音乐
#define FRIEND_S	256	//下载好友上传的音乐
#define FRIEND_T	512	//查看添加自己的用户或同意添加
	//
	#define FRIEND_T_l	2	//查看请求列表
	#define FRIEND_T_a	4	//同意某人的添加
	//
//******************

#define MAX_USER_ID_LENGTH		6	//用户id最大长度
#define MAX_USER_NAME_LENGTH	20	//用户name最大长度

#define TOP_NUMBER 10		//目前先获取TOP10（10）
#define LIKE_NUMBER	5		//目前先过去猜你喜欢的前5名（5）

#define MAX_F_WAIT_MUSIC 10	//等待服务器缓冲10s后继续播放音乐	in music.c

#define PLAY_QUEUE_MAX_LENGTH 500			//播放队列长度
#define MAX_SEND_MUSIC_BUFF_LENGTH 20000	//音乐数据缓存大小
#define MAX_WAIT_BUFF 5						//在等待5秒置flag

#define MAX_DOWNLOAD_COUNT 100	//下载队列长度 100
#define MAX_UPLOAD_COUNT 100	//上传队列长度 100

#define MAX_FFMPEG_MAX_LENGTH	100	//ffmpeg命令最大长度

#define MAX_LENGTH_COMMENT	363	//评论最大长度

#define FORMAT_WAV	0	//wav格式音乐文件
#define FORMAT_MP3	1	//mp3格式音乐文件
/*
wav
mp3
flac
ape
*/