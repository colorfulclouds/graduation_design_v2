//no use
//internet

//WAV_PARSER

#ifndef HEAD
#define HEAD
#include "head.h"
#endif

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
{ 	//3*sizeof(int)
    uint32_t magic;    //资源交换文件标志	00h-03h
    uint32_t length;   //从下个地址开始到文件尾的总字节数 04h-07h
    uint32_t type;    	//WAV文件标志（WAVE）08h-0bh
} WAVHeader_t; 
 
typedef struct WAVFmt 
{ 
    uint32_t magic;  		//波形格式标志（fmt ），最后一位空格
    uint32_t fmt_size; 		//过滤字节（一般为00000010H），若为00000012H则说明数据头携带附加信息（见“附加信息”）
    uint16_t format;        //格式种类（值为1时，表示数据为线性PCM编码）
    uint16_t channels; 		//通道数，单声道为1，双声道为2
    uint32_t sample_rate;   //采样频率
    uint32_t bytes_p_second;//波形数据传输速率（每秒平均字节数）
    uint16_t blocks_align; 	//DATA数据块长度，字节。
    uint16_t sample_length; //PCM位宽
} WAVFmt_t; 

typedef struct WAVChunkHeader 
{ 
    uint32_t type;      	//data 
    uint32_t length;        //samplecount 
} WAVChunkHeader_t; 
 
typedef struct WAVContainer 
{ 
    WAVHeader_t header; 
    WAVFmt_t format; 
    WAVChunkHeader_t chunk; 
} WAVContainer_t; 

int WAV_P_CheckValid(WAVContainer_t *container);
 
int WAV_ReadHeader(int fd, WAVContainer_t *container); 	//读取本机音乐的信息
