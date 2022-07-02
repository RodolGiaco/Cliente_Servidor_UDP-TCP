#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 


#define FIFO_PATH "/tmp/MI_FIFO"

int main()
{
    int fd,leido,i;
    char buff[1024];
	fd = open(FIFO_PATH, O_RDONLY, 0); 
	for(i=0;i<1;i++)
	{
	leido = read(fd, buff, sizeof(buff));
	 
	  ///Escribimos por consola
	  write(1,"\nSe leyo de la FIFO: ", sizeof("\nSe leyo de la FIFO: "));
	  write(1,buff,leido-1);
	  write(1,"\n", sizeof("\n"));
	}
   close(fd);
}