#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <mqueue.h>
#define MQ_PATH "/Rodol"

mqd_t fd_queue;
char buf[8192];
int byte_queue,i;
struct mq_attr attr, attr_rcv;  
int main(void) 
{   
   	fd_queue = mq_open(MQ_PATH,O_RDONLY,0777,&attr);	
	mq_getattr(fd_queue, &attr);
	for(i=0;i<1;i++)
	{
	  byte_queue = mq_receive(fd_queue,buf,attr.mq_msgsize,0);
	  printf("Mensaje recibido %s \n",buf);
	}
	mq_close(fd_queue);
	return 0;
} 