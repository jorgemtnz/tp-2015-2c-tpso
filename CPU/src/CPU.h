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
#include<utiles/espacioDeDatos.h>
#include<utiles/mapeoAMemoria.h>
#include<utiles/protocolo.h>

// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================
#define DISPONIBLE 1
#define NO_DISPONIBLE 0
#define CANT_RUTA 256
#define INST_INICIAR 1
#define INST_ESCRIBIR 2
#define INST_LEER 3
#define INST_ENTRADA_SALIDA 4
#define INST_FINALIZAR 5
#define USO 1
#define NO_USO 0
// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
//=======================================================================================

//--------------------------------estructura para manejar las instrucciones en el CPU----------------
typedef struct {
	char** instruccion_separada;  //arreglo de instruccion separada en palabras
	char* ptrParteLeida;  // puntero para manejar el recorrido del arreglo
	char* ptrComienzoInstruccion;  // puntero al comienzo de la instruccion

} t_instruccion;

typedef struct {
	char** bufferInstrucciones; // son todas las instrucciones ya separadas por barra n
	char* ptrCMemoriaMap; // puntero al comienzo de la memoria mapeada
	uint32_t ptrTamPagina;	// puntero al tamaño de página,este parametro lo necesito para desmapear posteriormente
	uint8_t cantidadInstrucciones;  // cantidad de instrucciones a ejecutar
} t_map;

//--------------------estructura para levantar del archivo de config -----------------------------------------
typedef struct {
	char* vg_ipPlanificador;
	uint32_t vg_puertoPlanificador;
	char* vg_ipMemoria;
	uint32_t vg_puertoMemoria;
	uint8_t cantidad_hilos;
	uint8_t retardo;
} t_configuracion;

//----------------------------------------------estructura del PCB del proceso------------------
//debe llegar desde el planificador en t_pcb

//---------------------------------------------estructura de una CPU, seria la de un hilo--------
typedef struct {
	uint8_t idCPU;
	uint8_t estado; //indica el porcentaje de utilizacion del ultimo minuto 60 ints equivale al 100 porciento
	t_pcb* pcbPlanificador;
	uint8_t cantInstEjecutadas; //se activa cuando cambie  de uso a no uso
	uint8_t estadoEjecucion;     //marca el define USO 1 NO_USO 0
} t_cpu;

//---------------------------------------------estructura principal del proceso CPU--------------
typedef struct {
	uint8_t contadorIdCPU;
	t_list* listaCPU;
} t_ProcCPU;

// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
t_map* crearMapeo();
t_configuracion* crearConfiguracion();
t_cpu* crearCPU();
t_ProcCPU* crearProcCPU();
t_instruccion* creaInstruccion();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================
void destInstruccion(t_instruccion* instruccion);
void destMap(t_map* unMap);
void destHiloCPU(t_cpu* unHiloCPU);
void destProcCPU(t_ProcCPU* unCPU);
void destConfig(t_configuracion* unaConfig);
// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
int reconoceTokenInstruccion(char* string);
char** separaInstruccion(char* instruccionCompleta);
int ejecutar(int token, char* separada_instruccion, t_pcb*  pcbPlanificador, int socket);
int leerInstruccion(char** instruccion, t_pcb* pcbPlanificador, int socket);
int descargaProcesoCPU(t_map* mCod);
char* pedirRutaArchivo();
int devuelveCantidadElementosArreglo(char** arreglo);

void* interpretarPaquete(Paquete* unPaquete, int fdReceptor);
//TODO Conflictua con sockets.h
//void enviar(int tipoDeMensaje, void* t_estructura, int fdDestinatario);
void* recibir(int fdReceptor);

// +++++++++++++++++++++++++++++++++++Funciones
//============================================================================

void leerArchivoDeConfiguracion();
int cargaProcesoaCPU(char* dirArchivo, t_map* mCodCPU);
void levantarHilosCPU();
int hiloCPU();
int ejecutaInstrucciones(t_map* mCodCPU, t_pcb* pcbPlanificador, int socket);
int interpretaInstruccion(char* instruccion_origen, t_pcb* pcbPlanificador, int socket);
int ejecutaInstruccion(int token, char* separada_instruccion, t_pcb* pcbPlanificador, int socket);
int preparaCPU(t_pcb* pcbPlanificador, int socket);
int procesaCodigo(t_pcb* pcbPlanificador, int socket);

//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
int ejecutaIniciarProceso(char* separada_instruccion,  t_pcb*pcbPlanificador, int socket );
int ejecutaEscribirMemoria(char* separada_instruccion,  t_pcb*pcbPlanificador, int socket );
int ejecutaLeerMemoria(char* separada_instruccion,  t_pcb*pcbPlanificador, int socket );
int ejecutaFinProcesoMemoria(char* separada_instruccion,  t_pcb*pcbPlanificador, int socket) ;
int ejecutaEntradaSalida(char* separada_instruccion,  t_pcb*pcbPlanificador, int socket );


//========================================================================================
//++++++++++++++++++++++++++++++++++++Funciones recepcion y envio a planificador++++++++++++++++++++
int recibirMensajeVarios(int socket, int token,  char*   buffer ,void* extra );
int ejecutaResultError(int socket);
int  ejecutaResultEscribir(int socket);
int  ejecutaResulFin(int socket);
int  ejecutaResulIniciarProc(int socket);
int  ejecutaResulInstrEjec(int socket);
int  ejecutaResultLeer(int socket);
int  ejecutaResulOk(int socket);

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================

t_configuracion* configuracion;
t_log* logger; //VG del logger
t_cpu* cpu;
t_map* mCodCPU; // para manejar las instrucciones en el cpu

t_equipo* un;

#endif /* CPU_H_ */
