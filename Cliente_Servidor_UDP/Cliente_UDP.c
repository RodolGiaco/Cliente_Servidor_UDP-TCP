/* 
programa SOCKET CLIENTE datagramas (UDP): el cliente envia al socket lo que recibe por teclado
y muestra en pantalla lo recibido en el socket: Servidor:--> datos rx socket
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

int main(int argc , char const * argv[])
{
   char buff_teclado[256], buff_socket[56];
   struct sockaddr_in direccion={};
   int sockfd, lee_teclado, lee_socket, conn;

   if (argc != 2){
       argv[1]="2000";
   } 

	
   ////////(1)-Crear el socket ///////////
   sockfd=socket(AF_INET, SOCK_DGRAM, 0);
   printf ("Paso 1: Se creo socket cliente\n");

   ////////(1.1)-Preparo la direccion///////
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(atoi(argv[1])); 
   
   /////Convierto IP en caracteres////////
   inet_aton("127.0.0.1", &direccion.sin_addr); //Servidor esta en el mismo host usamos Localhost = 127.0.0.1
   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %s\n",  argv[1]);


  while(1)
  { 
 
    if (( lee_teclado = read(STDIN_FILENO, buff_teclado, sizeof (buff_teclado))) > 0) //lee de teclado
	{   
        sendto(sockfd, buff_teclado, lee_teclado, 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
    }

     /////Recibo datos del socket////
     int len=sizeof(direccion);
     lee_socket = recvfrom(sockfd, buff_socket, sizeof (buff_socket),0,(struct sockaddr *) &direccion,&len);
     write(STDOUT_FILENO, "Servidor:--> ", 13);               //escribe leyenda en pantalla
     write(STDOUT_FILENO, buff_socket, lee_socket);           //escribe lo leido del socket

  }
  ////////(4)-Cierro la conexion///////////
  close(sockfd);
  return 0;   
}
