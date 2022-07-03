//////// Prod-Cons-UDP ///////

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#define MAX_BUFFER 5		//tamanio del buffer
 

pthread_mutex_t mutex;      // mutex de acceso al buffer compartido
int cant,consumidos=0;        // numero de elementos en el buffer 
int buffer[MAX_BUFFER];     // buffer comun 
int DATOS_A_PRODUCIR=20;


void Productor(){     //PRODUCTOR 

int dato, i, pos;
	pos=0;
	dato=100;
	i=0;

	while( i < DATOS_A_PRODUCIR ) 
	{
		pthread_mutex_lock(&mutex);   
		
		while (cant < MAX_BUFFER)
		{ 		//si buffer lleno
		
			buffer[pos] = dato;
			dato++;
			pos = (pos + 1);
	        cant++;
		    i++;
			printf("Productor posicion: %d, dato: %d, cantidad: %d, i: %d\n",pos,dato,cant,i);
	    }
		
		pthread_mutex_unlock(&mutex);	     
	}
	printf("Termino produccion: %d\n",i);
	pthread_exit(0);
}

///////////////////////////////////////////////////////////////

void Consumidor(){    // CONSUMIDOR

int dato, i, pos;
	pos=0;
	i=0;
	
	while( i < DATOS_A_PRODUCIR ) {
	
		pthread_mutex_lock(&mutex); 	
		
		while (cant > 0)
		{ 	// si buffer vacio
			dato = buffer[pos];
			pos = (pos + 1);
			cant--;
			i++;
			printf("Consumidor posicion: %d, dato: %d, cantidad: %d, i: %d\n",pos,dato,cant,i);	 
			consumidos++;
		}
		
		pthread_mutex_unlock(&mutex);
	}
	printf("Termino Consumo: %d\n",i);
	pthread_exit(0);
}
/////////////////////////////////////////////////////////////////

void ClienteUDP(){
	
   int dato, i, pos;
   pos=0;
   i=0;
   struct sockaddr_in direccion={};
   int sockfd,conn;

	
   ////////(1)-Crear el socket ///////////
   sockfd=socket(AF_INET, SOCK_DGRAM, 0);
   printf ("Paso 1: Se creo socket cliente\n");

   ////////(1.1)-Preparo la direccion///////
   direccion.sin_family= AF_INET;
   direccion.sin_port=htons(2000); 
   
   /////Convierto IP en caracteres////////
   inet_aton("127.0.0.1", &direccion.sin_addr); //Servidor esta en el mismo host usamos Localhost = 127.0.0.1
   printf ("Cliente va a enviar datos a IP: 127.0.0.1 Puerto: %d\n", 2000);


  while(i < DATOS_A_PRODUCIR)
  { 
    pthread_mutex_lock(&mutex); 	// bloquear mutex
	
	  while (consumidos > 0)
	  { 	
		  
		    // si buffer vacio
			dato = buffer[pos];
			printf("Envio a Server: %d, dato: %d, cantidad: %d, i: %d\n",pos,dato,cant,i);
			sendto(sockfd, &buffer[pos], sizeof(buffer[pos]), 0, (struct sockaddr *) &direccion, sizeof(direccion)); 
		  
			pos = (pos + 1) ;
			consumidos--;
			i++;
		}
		xpthread_mutex_unlock(&mutex);
   }
   printf("Termino Envio: %d\n",i);

	  
	
  ////////(4)-Cierro la conexion///////////
  close(sockfd);
  pthread_exit(0);  
}
int main(int argc, char *argv[]) 
{

    pthread_t th1, th2, th3;
	pthread_mutex_init(&mutex, NULL);
	
	if (argc == 2){
		printf ("argv[0]= %s , argv[1]= %s ,  argc=%d\n", argv[0], argv[1], argc);
		DATOS_A_PRODUCIR=atoi(argv[1]);
	}
	
	pthread_create(&th1, NULL, (void *)&Productor, NULL);
	pthread_create(&th2, NULL, (void *)&Consumidor, NULL);
	pthread_create(&th3, NULL, (void *)&ClienteUDP, NULL);
	
	pthread_join(th1, NULL);		//espera primero la produtor
	pthread_join(th2, NULL);		//luego espera al consumidor 

	printf("Productor-Consumidor con mutex: termina\n");
	pthread_join(th3, NULL);		//luego espera al consumidor 

	printf("Envio a server UDP: termina\n");
	pthread_exit(0);

}
