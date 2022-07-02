/* 
programa SOCKET SERVIDOR Stream (TCP) el servidor envia al socket
lo que recibe del socket y muestra en pantalla lo recibido en 
el socket: Cliente:--> datos rx socket
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int des_socket, sck_server, rx_socket, largo, cont;
char buffer_rx[256];
struct sockaddr_in struct_direccion={};
int SockEscucha,SockConexion;

int main(int argc, const char *argv[])      {

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
   bind (SockEscucha, (struct sockaddr *)&struct_direccion,sizeof(struct sockaddr_in));
   printf ("Paso 2: Asociar bind() \n");

   ////////(3)-Permitir una conexion pendiente//////
   if ((listen(SockEscucha, 1))<0) {
       printf ("ERROR en funcion listen()\n");
   exit(-1);     } 

   printf ("Paso 3: Permitir conexiones listen()\n");

   while(1) 
   {
   ////////(4)-Bloquea hasta que entre una conexion//////
   printf ("Paso 4: Bloqueo hasta que entre conexion accept()\n");
   cont=0;     
   SockConexion=accept(SockEscucha, NULL, 0) ;
   printf ("Desbloqueo de accept, entro conexion: %d\n",SockConexion);
   send (SockConexion ,"Bienvenido al servidor\n", 23,0 ); 
      

          while (( rx_socket = read(SockConexion, buffer_rx, sizeof (buffer_rx))) > 0)  //lee del socket  
		  {   
                  write (SockConexion ,buffer_rx, rx_socket);         //escribe en socket
                  write (STDOUT_FILENO , "cliente:--> ", 12);         //escribe leyenda en pantalla
                  write (STDOUT_FILENO , buffer_rx, rx_socket);       //escribe lo leido del socket
          }
   }
   ////////(5)-Cierro la conexion///////////
   close(SockConexion); 
   return 0;
}  
