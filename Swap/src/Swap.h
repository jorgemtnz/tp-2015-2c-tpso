/*
 * Swap.h
 *
 *  Created on: 29/8/2015
 *      Author: utnso
 */

#ifndef SWAP_H_
#define SWAP_H_

// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include "../test/test.h"
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
#include <utiles/sockets.h>
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

typedef enum {
	OK, ERROR,
} t_resultado_funcion;

typedef struct {
	uint8_t PID;
	t_resultado_funcion resultado;
} t_respuesta_iniciar_o_finalizar;

typedef struct {
	uint8_t PID;
	char* contenido;
	uint8_t numeroPagina;
	t_resultado_funcion resultado;
} t_devolucion_escribir_o_leer;

// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
l_procesosCargados* crearProceso();
l_espacioLibre* crearEspacioLibre();
t_contenido_pagina* crearEscribirEnProceso();
t_leerDeProceso* crearLeerDeProceso();
t_iniciar_swap* crearEstructuraIniciar();
t_PID* crearEstructuraPid();
t_respuesta_iniciar_o_finalizar* crearDevolucionIniciarOFinalizar();
t_devolucion_escribir_o_leer* crearDevolucionEscribirOLeer();
t_contenido_pagina* crearContenidoPagina();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion(int argc, char *argv[]);
void crearArchivo();
void acomodarEspaciosLibres(t_list* listaDeEspaciosLibres);
void compactarMemoria(t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados);
void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres);
t_respuesta_iniciar_o_finalizar* iniciar(t_iniciar_swap* estructuraIniciar, t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados);
t_devolucion_escribir_o_leer* escribir(t_list* listaDeProcesosCargados, t_contenido_pagina* procesoAEscribir);
t_devolucion_escribir_o_leer* leer(t_leerDeProceso *procesoRecibido, t_list* listaDeProcesosCargados);
t_respuesta_iniciar_o_finalizar* finalizar(uint8_t pid, t_list* listaDeProcesosCargados, t_list* listaDeEspaciosLibres);
void enviarResultadoIniciarERROR(int socket, void* estructura);
void enviarResultadoIniciarOK(int socket, void* estructura);
void enviarResultadoLeerERROR(int socket, void* estructura);
void enviarResultadoLeerOK(int socket, void* estructura);
void enviarResultadoEscribirERROR(int socket, void* estructura);
void enviarResultadoEscribirOK(int socket, void* estructura);
void enviarResultadoFinalizarERROR(int socket, void* estructura);
void enviarResultadoFinalizarOK(int socket, void* estructura);
void inicializarListas();
t_devolucion_escribir_o_leer* borrarContenidoPagina(t_contenido_pagina* procesoAEscribir);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
int procesarMensajesDeMemoria(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);

//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================
t_configuracion* configuracion;
char *espacioDatos;
t_list* listaDeProcesosCargados;
t_list* listaDeEspaciosLibres;
t_dictionary* conexiones;

//test
char* decirHolaMundo();

#endif /* SWAP_H_ */
