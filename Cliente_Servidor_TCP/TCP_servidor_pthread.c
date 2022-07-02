/* 
programa SOCKET SERVIDOR Stream (TCP) el servidor envia al socket
lo que recibe del socket y muestra en pantalla lo recibido en 
el socket: Cliente:--> datos rx socket
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
#include <unistd.h>
#include <wait.h>
#include <arpa/inet.h> 


int t=0;
int desc[5]={0};
int SockEscucha,SockConexion;
int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};


void *hilos(void *nro)
{
      int numero=*(int*)nro; //Nº de socket que paso a la funcion hilos que se manda como &desc[t]
      if (cont==0) 
	  {
           printf ("Desbloqueo de accept, entro conexion: %d\n",numero);
           send (numero ,"Bienvenido al servidor\n", 23,0 ); 
           cont=1;  
	  }   

          while (( rx_socket = read(numero, buffer_rx, sizeof (buffer_rx))) > 0) {  //lee del socket    
                  write (numero,buffer_rx, rx_socket);         //escribe en socket
                  write (STDOUT_FILENO , "cliente:--> ", 12);         //escribe leyenda en pantalla
                  write (STDOUT_FILENO , buffer_rx, rx_socket);      //escribe lo leido del socket
          }
     

}
int main(int argc, const char *argv[])      {

	
   pthread_t hilo[5];
   int rc;
	
   if (argc != 2){
       argv[1]="2000"; } 


   ////////(1)-Crear el socket ///////////
   SockEscucha=socket(AF_INET, SOCK_STREAM, 0);
   printf ("Paso 1: Servidor creo socket\n");

   ////////(1.1)-Preparo la direccion///////
   struct_direccion.sin_family = AF_INET;
   struct_direccion.sin_addr.s_addr = htonl(INADDR_ANY);  // asigna una IP de la maquina
   struct_direccion.sin_port = htons(atoi (argv[1]));     // puerto

   ////////(2)-Asocio al socket a la direccion///////
   
   if (bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in))<0) 
   {
       printf ("ERROR en funcion bind()\n");
       exit(-1);     
   } 
   printf ("Paso 2: Asociar bind() \n");

   ////////(3)-Permitir una conexion pendiente//////
   if ((listen(SockEscucha, 1))<0) 
   {
       printf ("ERROR en funcion listen()\n");
       exit(-1);  
   } 
   printf ("Paso 3: Permitir conexiones listen()\n");

  
   while(1) 
   {
	    ////////(4)-Bloquea hasta que entre una conexion//////
   		printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");
   		cont=0;     
   		desc[t]=accept(SockEscucha, NULL, 0);	
  
	   if (desc[t] >=0) 
	   {  
			rc=pthread_create(&hilo[t],NULL,hilos,(void*)(&desc[t]));
				if(rc!=0)
 				{
			    	printf("Error al crear hilo\n");
				    exit(-1);
				}
                t++;
       }
       else { printf ("Error en la conexion\n"); }
 
  }
  ////////(5)-Cierro la conexion/////////// 
  close(SockConexion); 
  return 0;
}  
