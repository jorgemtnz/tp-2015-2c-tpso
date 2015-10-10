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
	uint32_t retardo_swap;
	uint32_t retardo_compactacion;
} t_configuracion;

typedef struct {
	int ubicacion;
	int cantPagsLibres;

} l_espacioLibre;
typedef struct {
	uint8_t PID;
	int ubicacion;
	int cantPagsUso;

} l_procesosCargados;


// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
l_procesosCargados* crearProceso();
l_espacioLibre* crearEspacioLibre();
t_contenido_pagina* crearEscribirEnProceso();
t_leerDeProceso* crearLeerDeProceso();
t_iniciar_swap* crearEstructuraIniciar();
t_PID* crearRespuestaIniciar();
t_contenido_pagina* crearRespuestaEscribir();
t_contenido_pagina* crearRespuestaLeer();
t_PID* crearRespuestaFinalizar();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion(int argc, char *argv[]);
void crearArchivo();
void acomodarEspaciosLibres(t_list* listaDeEspaciosLibres);
void compactarMemoria(t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados);
void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres);
void iniciar(t_iniciar_swap* estructuraIniciar, t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados, int socket);
void escribir(t_list* listaDeProcesosCargados, t_contenido_pagina* procesoAEscribir, int socket);
char* leer(t_leerDeProceso *procesoRecibido, t_list* listaDeProcesosCargados, int socket);
void finalizar(uint8_t pid, t_list* listaDeProcesosCargados, t_list* listaDeEspaciosLibres, int socket);
void enviarResultadoIniciarERROR(int socket, void* estructura);
void enviarResultadoIniciarOK(int socket, void* estructura);
void enviarResultadoLeerERROR(int socket, void* estructura);
void enviarResultadoLeerOK(int socket, void* estructura);
void enviarResultadoEscribirERROR(int socket, void* estructura);
void enviarResultadoEscribirOK(int socket, void* estructura);
void enviarResultadoFinalizarERROR(int socket, void* estructura);
void enviarResultadoFinalizarOK(int socket, void* estructura);

//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
int procesarMensajesDeMemoria(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);

//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================
t_configuracion* configuracion;
t_log* logger;
char *espacioDatos;

#endif /* SWAP_H_ */
