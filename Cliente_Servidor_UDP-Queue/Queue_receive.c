#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <mqueue.h>
#define MQ_PATH "Rodol"

mqd_t fd_queue;
char buf[8192];
int byte_queue,i;
struct mq_attr attr, attr_rcv;  


int main() 
{   
   	fd_queue = mq_open(MQ_PATH,O_RDONLY,0777,&attr);
	mq_getattr(fd_queue, &attr);
	if(fd_queue == (-1))
	{
		printf("Error\n");
	}
    mq_receive(fd_queue,buf,attr.mq_msgsize,0);
	printf("Mensaje recibido %s \n",buf);
	mq_close(fd_queue);
	return 0;
} 