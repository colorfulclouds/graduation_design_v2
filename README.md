#graduation_design_v2

server:
	check:	用户登录与注册、测试用户邮箱
			更新上传目录、创建上传音乐记录表
	const_value:	基本常量值
	database:	访问数据库的操作
	friend_handle:	向客户端提供好友相关服务
	handle:	向客户端提供音乐相关服务
	head.h:	所有头文件
	main:	程序入口，等待客户端到来
	modes:	处理用户的登录与注册
	recvfrom_client:	接收用户信息和上传的音乐数据
	register:	注册成功后的发送的文本
	sendto_client:	向客户端发送用户信息和音乐数据
	send_recv:	重封的send和recv函数
	server:	初始化并设置服务器套接字
	user:	定义用户信息结构体、服务器套接字集合结构体、释放套接字
	wav_parser:	测试音乐文件并读取音乐文件头部
	
client:
	check_info:	对邮箱和密码进行本地正则校验
	connect_server:	初始化客户端套接字并连接服务器
	const_value:	基本常量值
	download_queue:	下载队列的实现
	friend_go:	对好友操作实现
	grocery_box:	一些闲杂函数
	head:	所有头文件
	log_display:	用户的登录和注册功能
	manual:	简单的手册
	music:	对音乐的操作
	music_shell:	命令解释器的实现
	plays_go:	对服务器上音乐操作的实现
	play_go:	对本地音乐的操作
	play_queue:	播放队列的实现
	recvfrom_server:	接收服务器传来的用户信息和音乐数据
	sendto_server:	向服务器发送用户信息和音乐数据
	send_recv:	重封的send和recv函数
	upload_queue:	上传队列的实现
	user:	定义用户信息结构体、客户端套接字、释放套接字
	user_log:	程序入口，处理信号
