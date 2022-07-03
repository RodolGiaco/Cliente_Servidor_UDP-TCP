/////////// Fifo_UDP /////////////////////
Tres procesos (A,B,C) NO relacionados se comunican de la siguiente forma:

A y B se comunican por medio de un socket datagrama
A y C se comunican por medio de una FIFO

1) El proceso B crea el socket datagrama servidor
2) El proceso A crea el socket datagrama cliente y la FIFO
3) El proceso A cuando recibe la señal SIGUSR1 escribe en el socket "0123456789" 
4) El proceso A cuando recibe la señal SIGUSR2 escribe en la FIFO "ABCDEFGHIJ"
5) El proceso B lee el socket y lo recibido lo muestra en pantalla
6) El proceso C lee la FIFO y lo recibido lo muestra en pantalla

El proceso B debe ser el primero en comenzar en una consola
El proceso A debe ser el segundo en comenzar en otra consola
El proceso C debe ser el tercero en comenzar en otra consola
Las señales se envían desde otra consola

![image](https://user-images.githubusercontent.com/62679123/177046239-eea06906-5f26-4383-aea2-f814bd6b9b50.png)

