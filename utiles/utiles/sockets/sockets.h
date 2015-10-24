#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //memset
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include "../protocolo.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>      //perror
#include <arpa/inet.h>  //INADDR_ANY
#include <unistd.h>     //close  usleep
#include <netdb.h> 		//gethostbyname
#include <netinet/in.h>
#include <fcntl.h> //fcntl

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef enum {
	NEW_CONNECTION,
	TERMINAL_MESSAGE,
	MESSAGE,
	HANG_UP
} t_tipo_notificacion;

typedef struct t_paquete {
			int8_t tipoMensaje;
			int16_t payloadTamanio;
		} Header;

		typedef struct{
			char* ip;
			int puerto;
		} t_equipo;

#define TAMANIO_TIPO_MENSAJE 20

#pragma pack(1)
typedef struct {
	int tamanioMensaje;
	t_tipo_mensaje tipoMensaje;
	char* nombre;
}t_header;
#pragma pack(0)


typedef struct {
	t_tipo_mensaje tipoMensaje;
	char* descripcion;
	void* funcionSerializacion;
	void* funcionDeserializacion;
} t_registro_serializacion;

typedef struct {
	void* resultado;
} t_resultado_serializacion;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Funciones
int  crearSocket();
int  enviarPorSocket(int fdCliente,const void *msg, int *len);
int enviarSimple(int fdCliente, void *msg, int len);
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
int recibirStructSegunHeader(int fdCliente, t_header* header, void* buffer, t_resultado_serializacion* resultadoSerializacion);
int mandarMensaje(int unSocket, int8_t tipo, int tamanio, void *buffer);

char* serializarEstructura(t_tipo_mensaje tipoMensaje, void* bufferMensaje);
int deserializarMensajeABuffer(t_tipo_mensaje tipoMensaje, char* bufferMsgSerializado, int tamanioMensaje, void* buffer);
bool string_equals(char* string1, char* string2);
int enviarSimple(int fdCliente, void *msg, int len);
t_header crearHeader(t_tipo_mensaje tipoMensaje, void *msg, int longitudMensaje);
int enviarHeader(int fdCliente, t_tipo_mensaje tipoMensaje, void *msg, int longitudMensaje);
int enviarStruct(int fdCliente, t_tipo_mensaje tipoMensaje, void *estructura);
int enviar(int fdCliente, void *msg, int len);
// Solo para el servidor
void asociarSocket(int sockfd, int puerto);
void escucharSocket(int sockfd, int conexionesEntrantesPermitidas);
int  aceptarConexionSocket(int sockfd);
// Solo para el ciente
int conectarSocket(int sockfd,char* ip_destino, int puerto);
int conectarSocketPorPrimeraVez(int sockfd, char* ip_Destino, int puerto);

int conectar(char* ip, char* puerto, int *sock);


// Para cliente y/o servidor
void escucharConexiones(char* puerto, int socketServer, int socketMemoria, int socketSwap, int (*funcionParaProcesarMensaje)(int, t_header*, char*, t_tipo_notificacion, void*, t_log*), void* extra,  t_log* logger);
int defaultProcesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);

//++++++++++++++++++++++++++++++++++++serializacion +++++++++++++++++++++++++++++++++++++++
void inicializarRegistroSerializadores();
bool existeSerializacion(t_tipo_mensaje tipoMensaje);
char* generarKeySerializacion(t_tipo_mensaje tipoMensaje);
t_registro_serializacion* getSerializacion(t_tipo_mensaje tipoMensaje);
void registrarSerializadores(t_tipo_mensaje tipoMensaje, char* descripcion, void* funcionSerializacion, void* funcionDeserializacion);
char* getNombreTipoMensaje(t_tipo_mensaje tipoMensaje);
int ejecutarSerializacion(void* (*funcion)(int, t_tipo_mensaje, void*), int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
int ejecutarDeserializacion(void* (*funcion)(int, t_tipo_mensaje), int fdCliente, t_tipo_mensaje tipoMensaje, t_resultado_serializacion* resultadoDeserializacion);
int enviarSerializado(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
int recibirSerializado(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura, t_resultado_serializacion* resultadoSerializacion);

//para identificarse
char* getNombre();
#endif /* SOCKETS_H_ */
