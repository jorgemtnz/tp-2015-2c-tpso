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
	uint8_t cantidadPaginas;
	uint8_t tamanioPagina;
	uint8_t retardo;
	char* tamanioArchivo;
} t_configuracion;

struct espacioLibre{
	int ubicacion;
	int cantPagsLibres;
	struct espacioLibre *sig;
};
struct procesosCargados{
	pid_t PID;
	int ubicacion;
	int cantPagsUso;
	struct procesosCargados *sig;
};
// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================

// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion(int argc, char *argv[]);
char* crearArchivo();

//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================
t_configuracion* configuracion;
t_log* logger;

#endif /* SWAP_H_ */
