/////////// Pipe_UDP /////////////////////
/*
Cuatro procesos (A,B,C,D) relacionados de la siguiente forma:
A padre de B y padre de C

Los procesos se comunican de la siguiente forma:
A y B se comunican por medio de un pipe
A y C se comunican por medio de un pipe
A y D se comunican por medio de un socket datagrama

1) El proceso D crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente, crea al proceso B y al proceso C 
3) El proceso B cuando recibe la señal SIGUSR1 escribe en el pipe1 el mensaje "0123456789" y termina.
4) El proceso C cuando recibe la señal SIGUSR2 escribe en el pipe2 el mensaje "ABCDEFGHIJ" y termina.
5) El proceso A espera a que termine el proceso B, lee el pipe1 y lo leído lo envía al socket datagrama servidor
6) El proceso A espera a que termine el proceso C, lee el pipe2 y lo leído lo envía al socket datagrama servidor
7) El proceso D lee el socket datagrama y muestra lo leído en pantalla

El proceso D debe ser el primero en comenzar en una consola
El proceso A comienza luego de D otra consola
Las señales se envían desde otra consola


       --- 
      | B |
       --- 
       |     -------      ---      --------      ---      --------  
       |--->| PIPE1 |--->|   |    | Socket |    |   |    |        |  
       |     -------     | A |--->|Datagama|--->| D |--->|pantalla|
       |     -------     |   |--->|        |--->|   |--->|        | 
       |--->| PIPE2 |--->|   |     --------      ---      --------  
       |     -------      ---
       ---              
      | C |
       --- 
*/

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 

#define FRASE_A "0123456789"
#define FRASE_B "ABCDEFGHIJ"
#define BUFF_SIZE 80


int ipc1[2], ipc2[2],pid1,pid2,i;
int leido;
char buff[BUFF_SIZE]={0};


void pipe1(int a)
{
	  close(ipc1[0]);      
      strncpy(buff, FRASE_A, sizeof(FRASE_A));
      write(ipc1[1], buff, sizeof(FRASE_A));
      exit(0);
}
void pipe2(int b)
{
      close(ipc2[0]);               
      strncpy(buff, FRASE_B, sizeof(FRASE_B)); 
      write(ipc2[1], buff,    sizeof(FRASE_B));
      exit(0); 
}

int main (int argc, const char *argv[])
{
   signal(SIGUSR1, pipe1);
   signal(SIGUSR2, pipe2);
 
   pipe(ipc1);
   pipe(ipc2);
	
	
	
   ///////////////////////////////////CREACION DEL SOCKET///////////////////////////////////////////
  

   if (argc != 2) argv[1]="2000"; 

   struct sockaddr_in direccion={};
   int sockfd;
	
   ////////(1)-Crear el socket ///////////
   sockfd=socket(AF_INET, SOCK_DGRAM, 0);
   printf ("Paso 1: Se creo socket cliente\n");

   ////////(1.1)-Preparo la direccion///////
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
   
   /////Convierto IP en caracteres////////
   inet_aton("127.0.0.1", &direccion.sin_addr); //Servidor esta en el mismo host usamos Localhost = 127.0.0.1
   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n", argv[1]);

   ///////////////////////////////////////////////////////////////////////////////////////////////
   
   pid1 = fork();
   if(pid1 == 0)
   {
	   printf("Hijo 1 PID: %d\n",getpid());
	   while(1){}

   }else{
	   
	   pid2 = fork();
	   if(pid2 == 0)
	   {
		   printf("Hijo 2 PID: %d\n",getpid());
		   while(1){}
		  
	   }else{
		   
           close(ipc1[1]);
		   close(ipc2[1]);
		   for(i=0; i<2; i++)
			{
				
				if(i==0)
				{
					leido = read(ipc1[0], buff, sizeof(buff));
				    //Escribir por consola//
                    write (1,"\nLeido del Pipe 1: ", sizeof("\nLeido del Pipe 1:"));
                    write (1, buff, leido-1);
                    printf("\nPor el proceso padre PID: %d\n", getpid());			
					
					//Enviamos el socket datagrama//
					sendto(sockfd, buff, leido-1, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
				}
				if(i==1)
				{
				  
					leido = read(ipc2[0], buff, sizeof(buff));
				    //Escribir por consola//
                    write (1,"\nLeido del pipe 2: ", sizeof("\nLeido del pipe 2:"));
                    write (1, buff, leido-1);
                    printf("\nPor el proceso padre, PID: %d \n", getpid());
					
					//Enviamos el socket datagrama//
					sendto(sockfd, buff, leido-1, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
				}
               
            }
		    //Se destruye la tuberia//
		    wait(NULL);
            wait(NULL);
		    close(ipc1[0]);
			close(ipc2[0]);
            exit(0);
	   }}
}

