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

	uint32_t puertoCpu;
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
