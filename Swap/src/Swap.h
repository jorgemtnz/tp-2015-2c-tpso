/*
 * Swap.h
 *
 *  Created on: 29/8/2015
 *      Author: utnso
 */

#ifndef SWAP_H_
#define SWAP_H_

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
#include <utiles/espacioDeDatos.h>
// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================

// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
//=======================================================================================

typedef struct {

	uint32_t puertoEscucha;
	char* nombreSwap;
	uint32_t cantidadPaginas;
	uint32_t tamanioPagina;
	uint32_t retardo;
	char* tamanioArchivo;
} t_configuracion;

typedef struct {
	int ubicacion;
	int cantPagsLibres;

} l_espacioLibre;
typedef struct {
	pid_t PID;
	int ubicacion;
	int cantPagsUso;

} l_procesosCargados;

//POSIBLE ESTRUCTURA ENTRE MEMORIA Y SWAP
typedef struct {
	pid_t PID;
	char* contenido;
	int numeroPagina;
} t_escribirEnProceso;

typedef struct {
	pid_t PID;
	int numeroPaginaInicio;
	int numeroPaginaFin;
} t_leerDeProceso;

// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
l_procesosCargados* crearProceso();
l_espacioLibre* crearEspacioLibre();
t_escribirEnProceso* crearEscribirEnProceso();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion(int argc, char *argv[]);
void crearArchivo();
void acomodarEspaciosLibres(t_list* listaDeEspaciosLibres);
void compactarMemoria(t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados);
void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres);
void iniciar(int cantidadPaginas, t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados, pid_t pid);
void escribir(t_list* listaDeProcesosCargados, t_escribirEnProceso* procesoAEscribir);
char* leer(t_leerDeProceso *procesoRecibido, t_list* listaDeProcesosCargados);
void finalizar(pid_t pid, t_list* listaDeProcesosCargados, t_list* listaDeEspaciosLibres);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, char* buffer, bool nuevaConexion, void* extra, t_log* logger);
int procesarMensajesDeMemoria(int socket, char* buffer, bool nuevaConexion, void* extra, t_log* logger);

//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================
t_configuracion* configuracion;
t_log* logger;
char *espacioDatos;

#endif /* SWAP_H_ */
