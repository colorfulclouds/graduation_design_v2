#ifndef DOWNLOAD_QUEUE
#define DOWNLOAD_QUEUE
#include "download_queue.h"
#endif

void init_download_queue(struct download_queue *queue)
{
	queue->front = 0;
	queue->rear = 0;
	queue->count = 0;
	queue->download_pointer = 0;	//初试没有下载任务
}

void insert_download_queue(struct download_queue *queue , int music_id , char *music_name , char *music_dir , char *user_id)
{
	queue->music_download[queue->rear].music_id = music_id;
	strcpy(queue->music_download[queue->rear].music_name , music_name);
	strcpy(queue->music_download[queue->rear].music_dir , music_dir);
	queue->music_download[queue->rear].download_flag = 'n';
	
	if(user_id != NULL)
		strcpy(queue->music_download[queue->rear].user_id , user_id);
	
	queue->rear ++;
	queue->count ++;
}

//下载完毕 删除
void delete_download_queue(struct download_queue *queue)
{
	queue->music_download[queue->download_pointer].download_flag = 'y';
//	queue->front ++;	//因为有download_flag标记 所以不能进行删除
	queue->download_pointer ++;
	queue->count --;	//count记录的是未下载的音乐
}

int display_download_queue(struct download_queue *queue)
{
	int i;
	
	printf("\tmusic_id\tmusic_name\tmusic_dir\tdownload_flag\n");
	for(i = queue->front ; i<queue->rear ; i ++)
	{
		printf("\t%d\t%s\t%s\t%c\n", queue->music_download[i].music_id , queue->music_download[i].music_name , queue->music_download[i].music_dir , queue->music_download[i].download_flag );
	}
	printf("there are(is) %d music(s) will were(was) downloaded\n", queue->count);
	return queue->count;
}