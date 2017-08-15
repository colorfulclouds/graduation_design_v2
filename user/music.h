//MUSIC
#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef CONST_VALUE
#define CONST_VALUE
#include "const_value.h"
#endif

#ifndef MUSIC_SHELL
#define MUSIC_SHELL
#include "music_shell.h"
#endif

#ifndef PLAY_QUEUE
#define PLAY_QUEUE
#include "play_queue.h"
#endif

#ifndef RECVFROM_SERVER
#define RECVFROM_SERVER
#include "recvfrom_server.h"
#endif

#ifndef SEND_RECV
#define SEND_RECV
#include "send_recv.h"
#endif

#ifndef CONNECT_SERVER
#define CONNECT_SERVER
#include "connect_server.h"
#endif

struct music_dir
{
	int music_number;
	char music_path[CONF_PATH_MAX_LENGTH];	//下面具体音乐对应的目录，这样做是为了区分在不同路径下相同的音乐
	char music_name[MAX_ONE_DIR_MUSIC_COUNT][MUSIC_NAME_MAX_LENGTH];
};
//******************
//******************

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

#ifndef RECVFROM_SERVER
#define RECVFROM_SERVER
#include "recvfrom_server.h"
#endif

//the processing of *.wav
//internet
typedef unsigned char  uint8_t; 
typedef unsigned short uint16_t; 
typedef unsigned int   uint32_t; 
 
#if __BYTE_ORDER == __LITTLE_ENDIAN 
#define COMPOSE_ID(a,b,c,d) ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24)) 
#define LE_SHORT(v)           (v) 
#define LE_INT(v)               (v) 
#define BE_SHORT(v)           bswap_16(v) 
#define BE_INT(v)               bswap_32(v) 
#elif __BYTE_ORDER == __BIG_ENDIAN 
#define COMPOSE_ID(a,b,c,d) ((d) | ((c)<<8) | ((b)<<16) | ((a)<<24)) 
#define LE_SHORT(v)           bswap_16(v) 
#define LE_INT(v)               bswap_32(v) 
#define BE_SHORT(v)           (v) 
#define BE_INT(v)               (v) 
#else 
#error "Wrong endian" 
#endif 
 
#define WAV_RIFF        COMPOSE_ID('R','I','F','F') 
#define WAV_WAVE        COMPOSE_ID('W','A','V','E') 
#define WAV_FMT         COMPOSE_ID('f','m','t',' ') 
#define WAV_DATA        COMPOSE_ID('d','a','t','a') 
 
/* WAVE fmt block constants from Microsoft mmreg.h header */ 
#define WAV_FMT_PCM             0x0001 
#define WAV_FMT_IEEE_FLOAT      0x0003 
#define WAV_FMT_DOLBY_AC3_SPDIF 0x0092 
#define WAV_FMT_EXTENSIBLE      0xfffe 
 
/* Used with WAV_FMT_EXTENSIBLE format */ 
#define WAV_GUID_TAG "/x00/x00/x00/x00/x10/x00/x80/x00/x00/xAA/x00/x38/x9B/x71" 
 
typedef struct WAVHeader 
{
    uint32_t magic;     	//资源交换文件标志	00h-03h
    uint32_t length;        //从下个地址开始到文件尾的总字节数 04h-07h
    uint32_t type;      	//WAV文件标志（WAVE）08h-0bh
} WAVHeader_t; 
 
typedef struct WAVFmt 
{ 
    uint32_t magic;  		//波形格式标志（fmt ），最后一位空格
    uint32_t fmt_size; 		//过滤字节（一般为00000010H），若为00000012H则说明数据头携带附加信息（见“附加信息”）
    uint16_t format;        //格式种类（值为1时，表示数据为线性PCM编码）
    uint16_t channels; 		//通道数，单声道为1，双声道为2
    uint32_t sample_rate;   //采样频率
    uint32_t bytes_p_second;//波形数据传输速率（每秒平均字节数）
    uint16_t blocks_align;  //DATA数据块长度，字节。
    uint16_t sample_length; //PCM位宽
} WAVFmt_t; 

typedef struct WAVChunkHeader 
{ 
    uint32_t type;    		/* 'data' */ 
    uint32_t length;        /* samplecount */ 
} WAVChunkHeader_t; 
 
typedef struct WAVContainer 
{ 
    WAVHeader_t header; 
    WAVFmt_t format; 
    WAVChunkHeader_t chunk; 
} WAVContainer_t; 
 
int WAV_ReadHeader(int fd, WAVContainer_t *container); 	//读取本机音乐的信息
//***************************
//***************************
typedef long long off64_t; 
 
typedef struct SNDPCMContainer 
{ 
    snd_pcm_t *handle; 
    snd_output_t *log; 
    snd_pcm_uframes_t chunk_size; 
    snd_pcm_uframes_t buffer_size; 
    snd_pcm_format_t format; 
    uint16_t channels; 
    size_t chunk_bytes; 
    size_t bits_per_sample; 
    size_t bits_per_frame; 
 
    uint8_t *data_buf; 
} SNDPCMContainer_t; 
 
ssize_t SNDWAV_WritePcm(SNDPCMContainer_t *sndpcm, size_t wcount); 
 
int SNDWAV_SetParams(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav); 

ssize_t SNDWAV_P_SaveRead(int fd, void *buf, size_t count);

void SNDWAV_Play(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav, int fd , int *stop_playing);	//播放声音的关键 
void SNDWAV_Play_s(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav, int fd , int *stop_playing) ;

int play_music(void *arg , int *stop_playing);
int play_music_s(int online_sock_fd , int music_id , int *stop_playing , struct music_shell *order_struct);

int display_music_menu(struct music_dir * user_music);
int display_conf_dir_music(char *dir_name);

int get_format_position(char *file_name , char *file_format);
int is_music(char *file_format);

void get_indicator_position(char *dir_name);

//下面这个函数暂时没用
void init_user_music(struct music_dir * user_music);

int get_music_from_dir(struct music_dir * user_music);		//从配置文件中将每一个目录的音乐获取出来

void *go_music(void *arg);
void random_go_music(struct play_music_queue *play_queue , int online_sock_fd , int client_sock_fd , struct music_shell *order_struct);	//为随机播放准备的

int get_music_format(char *file_name);						//直接返回音乐格式码
void mp3_to_wav(char *music_file );
