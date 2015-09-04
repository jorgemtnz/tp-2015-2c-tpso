/*
 * CPU.h
 *
 *  Created on: 29/8/2015
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_
// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include <stdint.h>
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
#define DISPONIBLE 1
#define NO_DISPONIBLE 0
#define CANT_RUTA 256
// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
//=======================================================================================
typedef struct{
	char** bufferInstrucciones; //
	char* ptrCMemoriaMap;// puntero al comienzo de la memoria mapeada
	uint32_t* ptrTamPagina;	// puntero al tamaño de página,este parametro lo necesito para desmapear posteriormente
}t_mapeo;

typedef struct {
	char* vg_ipPlanificador;
	uint32_t vg_puertoPlanificador;
	char* vg_ipMemoria;
	uint32_t vg_puertoMemoria;
	uint8_t cantidad_hilos;
	uint8_t retardo;
} t_configuracion;

typedef struct {
	char path[CANT_RUTA];
	uint8_t ptr_proximaInstruccion;
	uint8_t cantInstruc; // cantidad de instrucciones a ejecutar
} t_PCB;

typedef struct {
	uint8_t idProc;
	t_PCB pcb;
	uint8_t tiempoEjec; // tiempo de ejecucion indicado para este proceso
} t_mProc;

typedef struct {
	uint8_t idCPU;
	uint8_t estado;
	t_mProc proceso;
} t_cpu;

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
t_log* logger; //VG del logger
t_cpu* cpu;

//const CANT_RUTA;
#endif /* CPU_H_ */
