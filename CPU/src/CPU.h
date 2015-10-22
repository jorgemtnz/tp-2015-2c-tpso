/*
 * CPU.h
 *
 *  Created on: 29/8/2015
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_
// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include "../test/test.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // read write  /* para STDIN_FILENO */
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
	char** instruccion_separada;  //arreglo de instruccion separada en palabras, separado el token del resto de la instruccion
	char* ptrComienzoInstruccion;  // puntero al comienzo de la instruccion

} t_instruccion;

typedef struct {
	char** bufferInstrucciones; // son todas las instrucciones ya separadas por barra n
	char* ptrCMemoriaMap; // puntero al comienzo de la memoria mapeada
	uint16_t ptrTamPagina;	// puntero al tamaño de página,este parametro lo necesito para desmapear posteriormente
	uint8_t cantidadInstrucciones;  // cantidad de instrucciones a ejecutar
	t_respuesta_ejecucion* respEjec; //resultado de las rafagas
} t_mCod;

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
	uint8_t porcentajeUso; //indica el porcentaje de utilizacion del ultimo minuto 60 ints equivale al 100 porciento
	t_pcb* pcbPlanificador;
	uint8_t cantInstEjecutadas; //se activa cuando cambie  de uso a no uso
	uint8_t estadoEjecucion;     //marca el define USO 1 NO_USO 0
	t_mCod* mCodCPU; // para manejar lo relacionado al codigo ejecutado y resultados de rafaga
    char* nombre;
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
t_resultado_instruccion* creaResultadoInstruccion();
t_respuesta_ejecucion* creaRespuestaEjecucion();
t_instruccion* creaInstruccion();
t_mCod* crearmCod();
t_configuracion* crearConfiguracion();
t_cpu* crearCPU();
t_ProcCPU* crearProcCPU();
t_pcb * crearPcb();

// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================

void destmCod(t_mCod* unmCod) ;
void destConfig(t_configuracion* unaConfig);
void destHiloCPU(t_cpu* unHiloCPU);
void destProcCPU(t_ProcCPU* unCPU);
void destInstruccion(t_instruccion* unaInstruccion);
void destVectorInstruccion(char** vectorInstruccion);
void destIniciarSwap(t_iniciar_swap* estructura);
void destEscrMem(t_contenido_pagina* estruc);
void destEntradSalid(t_entrada_salida* entradSalid );
void destResInstruc(t_resultado_instruccion* resultInstrucc);
void destRespEjec(t_respuesta_ejecucion* respEjec);

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares y ejecucion y ejecucionResult
//============================================================================
int reconoceTokenInstruccion(char* string);
char** separaInstruccion(char* instruccionCompleta);
void*  ejecutar(int token, char* separada_instruccion, t_cpu*  cpu);
int descargaProcesoCPU(t_mCod* mCod);
char* pedirRutaArchivo();
int devuelveCantidadElementosArreglo(char** arreglo);

t_iniciar_swap* ejecuta_IniciarProceso(char* separada_instruccion, t_cpu* cpu);
t_contenido_pagina* ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu);
t_contenido_pagina* ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu);
t_PID* ejecuta_FinProcesoMemoria(t_cpu* cpu);
t_entrada_salida*  ejecuta_EntradaSalida(char* separada_instruccion, t_cpu* cpu);
int putsConsola (const char *msg) ;

t_cpu* ejecutarResul_Fin(t_cpu* cpu);

// +++++++++++++++++++++++++++++++++++Funciones
//============================================================================

void leerArchivoDeConfiguracion(int argc, char *argv[]);
int cargaProcesoaCPU(char* dirArchivo, t_mCod* mCodCPU);
void levantarHilosCPU();
int hiloCPU();

void* ejecuta_Instruccion(char* instruccion_origen, t_cpu* cpu);
int preparaCPU(t_pcb* pcbPlanificador);
void* procesaCodigo(t_cpu* cpu);

//++++++++++++++++++++++++++++++++++++funciones envio por comando ejecutado en CPU+++++++++++++++++++++++++++++++++++++++

//========================================================================================
//++++++++++++++++++++++++++++++++++++Funciones recepcion y envio a planificador++++++++++++++++++++
int procesarMensajes(int socket, t_header* , char* buffer, t_tipo_notificacion , void* extra, t_log* );
void* recibirMensajeVarios( t_header*,   char*   buffer,void* extra,t_cpu* cpu );

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================

t_configuracion* configuracion;
t_log* logger; //VG del logger
t_ProcCPU* procCPU;  //proceso CPU que controla la lista de los hilos CPU



t_equipo* un;
t_dictionary* conexiones;

//test
char* decirHolaMundo();

#endif /* CPU_H_ */
