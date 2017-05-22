#ifndef UPLOAD_QUEUE
#define UPLOAD_QUEUE
#include "upload_queue.h"
#endif

void init_upload_queue(struct upload_queue *queue)
{
	queue->front = 0;
	queue->rear = 0;
	queue->count = 0;
	
	queue->upload_pointer = 0;
}

void divide_music_name(char *music_dir_name , char *music_dir , char *music_name)
{
	int length = strlen(music_dir_name);
	
	int i;
	
	for(i = length-1 ; i >= 0 ; i --)
	{
		if(music_dir_name[i] == '/')
			break;
	}
	
	strcpy(music_name , music_dir_name + i + 1);
	strcpy(music_dir , music_dir_name);
	music_dir[i+1] = '\0';
}

void insert_upload_queue(struct upload_queue *queue , char *music_dir_name)
{
	strcpy(queue->music_upload[queue->rear].music_dir_name , music_dir_name);
	
	divide_music_name(music_dir_name , queue->music_upload[queue->rear].music_dir , queue->music_upload[queue->rear].music_name);
	
	queue->music_upload[queue->rear].upload_flag = 'n' ;
	
	queue->rear ++;
	queue->count ++;
}

void delete_upload_queue(struct upload_queue *queue)
{
	queue->music_upload[queue->upload_pointer].upload_flag = 'y';
//	queue->front ++;	//不需要	

	queue->upload_pointer ++;
	queue->count --;
}

int display_upload_queue(struct upload_queue *queue)
{
	int i;
	
	printf("\tmusic_dir\tmusic_name\tupload_flag\n");
	for(i = queue->front ; i < queue->rear ; i ++)
	{
		printf("\t%s\t%s\t%c\n", queue->music_upload[i].music_dir , queue->music_upload[i].music_name , queue->music_upload[i].upload_flag);
	}
	
	return queue->count;
}