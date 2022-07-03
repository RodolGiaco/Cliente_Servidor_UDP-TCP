/*
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:
A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una cola de mensajes

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la cola de mensajes
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la cola de mensaje el mensaje "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la cola de mensaje y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola


 ---      ---------------       ---      --------  
|   |--->|Socket datagrma |--->| B |--->|Pantalla|  
| A |     ----------------      ---      --------
|   |     ----------------      ---      --------  
|   |--->|Cola de mensajes|--->| C |--->|Pantalla|  
 ---      ----------------      ---      --------
*/
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

#define MENSAJE_A "0123456789"
#define MENSAJE_B "ABCDEFGHIJ"
#define MQ_PATH "/Rodol" 

int sockfd, aux1, aux2;
mqd_t fd_queue;

struct sockaddr_in direccion={};
struct mq_attr attr, attr_rcv;  


void mA(int a)
{
	////ESCRIBO EN EL SOCKET MENSAJE_A////
	printf("El valor de a es : %d\n",a);
    sendto(sockfd, MENSAJE_A, strlen(MENSAJE_A)+1, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
	printf("MENSAJE_A enviado al proceso B\n");
	aux1 = 1;
}

void mB(int b)
{
	////ESCRIBO EN LA QUEUE MENSAJE_B/////
	printf("El valor de b es : %d\n",b);
    mq_send(fd_queue, MENSAJE_B, strlen(MENSAJE_B)+1,1);
	printf("MENSAJE_B enviado al proceso C\n");
	aux2 = 1;
}

int main(int argc, const char *argv[]) 
{
	
	
signal(SIGUSR1,mA);
signal(SIGUSR2,mB);
	
printf ("Proceso ciente= %d\n ", getpid());

////////////CREO QUEUE///////////
   attr.mq_msgsize = 8192;
   attr.mq_maxmsg = 5;
   fd_queue = mq_open(MQ_PATH,O_RDWR | O_CREAT, 0777,&attr);
      if(fd_queue < 0)
      {
    	printf("Error en crear la queue\b");
		exit(-1);
      }
	
////////////CREO SOCKET CLIENTE////////////
   if (argc != 2)
   {
       argv[1]="2000";
   } 

   //////////Crear el socket ///////////
   sockfd=socket(AF_INET, SOCK_DGRAM, 0);
   printf ("Se creo socket cliente\n");

   //////////Preparo la direccion///////
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
   
   ///////Convierto IP en caracteres////////
   inet_aton("127.0.0.1", &direccion.sin_addr); //Servidor esta en el mismo host usamos Localhost = 127.0.0.1
   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",argv[1]);
	
   ////////(4)-Cierro la conexion///////////
	
   while(aux1 == 0 || aux2 == 0){}
   
   close(sockfd);
   return 0;
}
	
  
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
