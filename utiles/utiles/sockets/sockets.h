#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //memset
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>      //perror
#include <arpa/inet.h>  //INADDR_ANY
#include <unistd.h>     //close  usleep
#include <netdb.h> 		//gethostbyname
#include <netinet/in.h>
#include<fcntl.h> //fcntl

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct t_paquete {
			int8_t tipoMensaje;
			int16_t payloadTamanio;
		} Header;

		typedef struct{
			char* ip;
			int puerto;
		} t_equipo;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Funciones
int  crearSocket();
int  enviarPorSocket(int fdCliente,const void *msg, int *len);
int  recibirPorSocket(int fdCliente, void *buf, int len);		// Retorna la cantidad de bytes recibidos
void comprobarDesconexion(int bytes);
void cerrarSocket(int sockfd);
void seleccionarSocket(int maxNumDeFD, fd_set *fdListoLectura, fd_set *fdListoEscritura,
					   fd_set *fdListoEjecucion, int* segundos, int* miliSegundos);
int recv_timeout(int s , int timeout);
struct sockaddr_in especificarSocketInfo(char* direccion, int puerto);
int recibirMensaje(int unSocket, void** buffer);
int recibirHeader(int unSocket, Header* header) ;
int recibirDatos(int unSocket, Header header, void** buffer);
int mandarMensaje(int unSocket, int8_t tipo, int tamanio, void *buffer);


// Solo para el servidor
void asociarSocket(int sockfd, int puerto);
void escucharSocket(int sockfd, int conexionesEntrantesPermitidas);
int  aceptarConexionSocket(int sockfd);
// Solo para el ciente
int conectarSocket(int sockfd,char* ip_destino, int puerto);
int conectarSocketPorPrimeraVez(int sockfd, char* ip_Destino, int puerto);

#endif /* SOCKETS_H_ */
