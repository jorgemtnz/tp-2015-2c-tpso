#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // read write
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <commons/log.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <utiles/sockets/sockets.h>
#include <utiles/configExtras.h>
#include <utiles/files.h>
// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================
#define NUMEROFUNCIONESCONSOLA 4


// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
typedef struct {
	char* puertoEscucha;
	char* algorimoPlanificacion;
	uint32_t quantum;
} t_configuracion;
//=======================================================================================



// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================

// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================


// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
void crearLogger();
void leerArchivoDeConfiguracion(int argc, char *argv[]);
int putsConsola (const char *msg);
int printConsola(const char *formato, ...);
//============================================================================


// ++++++++++++++++++++++++++++++++++++++Funciones de Consola
//============================================================================



// +++++++++++++++++++++++++++++++++++++++++++++Consola implementacion
//==================================================================================
int idFuncion(char* funcion);
void aplicarFuncion(int idFuncion);
void levantarConsola();
void mostrarComandos();
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, bool nuevaConexion, void* extra, t_log* logger);
int procesarMensajesConsola(int socket, t_header* header, char* buffer);
int correrPrograma(int socket, t_header* header, char* buffer);
int correrPath(int socket, t_header* header, char* buffer);
int finalizarPid(int socket, t_header* header, char* buffer);
int ps(int socket, t_header* header, char* buffer);
int cpu(int socket, t_header* header, char* buffer);
//========================================================================================


// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++

t_configuracion* configuracion;
t_log* logger;
t_dictionary* conexiones;

//===========================================================================================




t_log* logger; //VG del logger

#endif /* PLANIFICADOR_H_ */
