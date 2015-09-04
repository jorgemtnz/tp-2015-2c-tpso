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

typedef struct {
	t_list memoriaPrincipal;
	int limiteMP;
}t_MP;

typedef struct { // estructura que se carga en la lista de memoria principal
	int idMarco; // la memoria identificara a cada marco a traves de este id
	char* contenido; // el texto que tendra esa posicion
	bool libre;
	t_list paginasAsociada; // permite asociar el pedido de "escribir X pagina" con el marco. Como pueden ser varias hice una lista
}t_marco;

typedef struct {
	int idProc;
	int pagina; // si esta vacia va -1 (o puede que no, no se)
	int marco; // si esta vacio va -1 (o puede que no, no se)
	int bitPresencia; // para ver si se encuentra en un marco (1) o en una pagina (0)
} t_TLB;

typedef struct {
	int idProc;
	int pagina; // si esta vacia va -1 (o puede que no, no se)
	int marco; // si esta vacio va -1 (o puede que no, no se)
	int bitPresencia; // para ver si se encuentra en un marco (1) o en una pagina (0)
} t_TablaDePaginas;

typedef struct {
	int operacion; // 0 leer, 1 escribir ( esto hay que verlo igual )
	char* contenido;
	int idProc;
	int pagina; // se va a tener que ver si esta pagina esta en un marco de la MP o en la SWAP
}t_atenderInstruccion;


// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================

// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================


// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion();

//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================
t_configuracion* configuracion;
t_log* logger;


#endif /* MEMORIA_H_ */
