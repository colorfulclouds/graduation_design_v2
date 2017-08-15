//no use
//internet
#ifndef WAV_PARSER
#define WAV_PARSER
#include "wav_parser.h"
#endif

int WAV_P_CheckValid(WAVContainer_t *container) 
{ 
    if (container->header.magic != WAV_RIFF || container->header.type != WAV_WAVE || 
        container->format.magic != WAV_FMT  || container->format.fmt_size != LE_INT(16) || 
		(container->format.channels != LE_SHORT(1) && container->format.channels != LE_SHORT(2)) || container->chunk.type != WAV_DATA) 
	{
        fprintf(stderr, "non standard wav file./n"); 
        return -1; 
    } 
 
    return 0; 
} 
 
int WAV_ReadHeader(int fd, WAVContainer_t *container) 
{ 
    assert((fd >=0) && container); 
 
	if( read(fd , &container->header , sizeof(container->header)) != sizeof(container->header) || read(fd,&container->format,sizeof(container->format))!=sizeof(container->format) || read(fd,&container->chunk,sizeof(container->chunk))!=sizeof(container->chunk))
	{
        fprintf(stderr, "Error WAV_ReadHeader/n"); 
        return -1; 
    } 
 
    if (WAV_P_CheckValid(container) < 0) 
        return -1; 
    return 0; 
}
