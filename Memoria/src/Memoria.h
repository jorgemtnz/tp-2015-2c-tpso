/*
 * Memoria.h
 *
 *  Created on: 29/8/2015
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

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
#include "../Test/test.h"


// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================



// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
//=======================================================================================

typedef struct {

	uint32_t puertoEscucha;
	uint32_t puertoSwap;
	char* ipSwap;
	char* nombreMemoria;
	uint8_t maximosMarcosPorProceso;
	uint8_t cantidadMarcos;
	uint8_t tamanioMarcos;
	uint8_t entradasTlb;
	uint8_t tlbHabilitada;
	uint8_t retardoMemoria;

} t_configuracion;

typedef struct { // estructura que se carga en la lista de memoria principal
	int idMarco; // la memoria identificara a cada marco a traves de este id
	char* contenido; // el texto que tendra esa posicion
} t_marco;

typedef struct {
	int idProc;
	int paginaDelProceso;// supongo que las paginas del proceso arrancan desde 1
	int idMarco; // si esta vacio va -1, lo que indica que se tiene que ir a buscar al swap
	int bitPagModificada; // si esta en memoria ver si fue modificada
} t_TLB;

typedef struct {
	int idProc;
	int paginaDelProceso;// supongo que las paginas del proceso arrancan desde 1
	int idMarco; // si esta vacio va -1, lo que indica que se tiene que ir a buscar al swap
	int bitPagModificada; // si esta en memoria ver si fue modificada
} t_TablaDePaginas;

typedef struct {
	int idProc;
	int CantPag;
}t_iniciarProc;

typedef struct {
	int idProc;
	int Pag;
	char* contenido;
}t_escrituraProc;

typedef struct {
	int idProc;
	int pagIn;
	int pagFin;
	char* contenido;
}t_lectura;

typedef struct {
	int idProc;
}t_finalizarProc;

// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
t_iniciar_swap* crearEstructuraIniciar();
t_PID* crearEstructuraFinalizar();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================


// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion();
void iniciar(pid_t idProc, uint16_t cantPag, int socketCPU);
void leer(int idProc, int pag, int socketSwap, int socketCPU);
void finalizar(int idProc);
void inicializadoCorrecto(int idProc, int cantPag);
t_TablaDePaginas* iniciarTablaDePaginas();
t_escrituraProc* iniciarEscrituraProc();
t_TLB* iniciarTLB();
t_marco * iniciarMarco();
t_config* iniciarArchivoConfig();
void* interpretarPaquete(Paquete* unPaquete, int fdReceptor);
int buscarSiEstaEnMemoria(int idProc, int nroPag); // retorna o el id o un -1 si no esta en memoria
void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido);
void enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap);
void enviarFinalizarASwap(t_PID *estructura, int socketSwap);
void traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag,int socketSwap);
void cargarNuevoMarcoAMemoria(char* contenido);
bool llegoAlMaximoDelProcesoLaMemoria(int idProc);
void sacarAlPrimeroDeMemoriaDelProceso(int idProc);
void sacarAlPrimeroDeMemoria();
char* traerContenidoDeMarco(int idMarco);
void enviarACPUContenidoPaginaDeUnProceso(t_contenido_pagina* lecturaMandarCpu, int socketCPU);
bool estaLlenaLaMemoria();
void verificarBitDeModificada(int idMarco,char* contenido);
t_list* buscarLosIdDeProceso(int idProc);
void eliminarDeMemoria(int id);
void enviarASwapEliminarProceso(int idProc);
void enviarASwapContenidoPaginaDesactualizada(int idProc, int pagina, char* contenido) ;
void enviarRtaIniciarFalloCPU (t_PID * estructura, int socketCPU);
void respuestaTraerDeSwapUnaPaginaDeUnProceso(int idProc, int pag, char* contenido,int socketCPU);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++

void inicializacionDesdeCero();

//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++



//===========================================================================================
t_configuracion* configuracion;
t_log* logger;
t_dictionary* conexiones;
// ----------- Contadores -------- //
int contadorPagTP,variableIdMarco; // contador de paginas de la tabla de paginas

// ----------- Listas ------------ //
t_list* listaMemoria;
t_list* listaTLB;
t_list* listaTablaDePag;

// ----------- Semaforos ---------- //
pthread_mutex_t mutexProcesadorMensajes;
pthread_mutex_t mutexParaInicializado;

#endif /* MEMORIA_H_ */
