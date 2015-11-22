/*Protocolo
 *
 *
 */
#ifndef UTILES_PROTOCOLO_H_
#define UTILES_PROTOCOLO_H_
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++ INCLUDE   ++++++++++++++++++++++++++++++++++++++++++++++++++

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <commons/collections/list.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++  DEFINE   +++++++++++++++++++++++++++++++++++++++++++++++++++
#define TAMANIO_TEXTO 512

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++  ESTRUCTURAS   ++++++++++++++++++++++++++++++++++++++++

typedef enum {
	HEADER = 1, STRING,  //mensaje 1er checkpoint

	CONTEXTO_MPROC, //solicitud del planificador para que se levante el proceso desde el path dado.
	RESUL_EJECUCION_OK,
//	RESUL_EJECUCION_ERROR,
	RESUL_INSTR_EJEC,
	//+++memoria+++++++++++++++++++
	INICIAR_PROC_SWAP,
	LEER_SWAP,
	LEER_SWAP_POR_ESCRIBIR,
	RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR,
	ESCRIBIR_SWAP,
	FIN_PROCESO_SWAP,
	RESUL_TRAER_PAG_SWAP_OK,
	RESUL_TRAER_PAG_SWAP_NO_OK,
	SOBREESCRIBIR_SWAP,

	//+++++++++++origen:cpu  destino: memoria++++++++++++
	INICIAR_PROCESO_MEM, //solicitud desde el CPU a memoria
	LEER_MEM,    //solicitud desde el CPU a memoria
	ESCRIBIR_MEM,   //solicitud desde el CPU a memoria
	FIN_PROCESO_MEM,  //solicitud desde el CPU a memoria
	//++++++++++++++++cpu+++++++++++++++++++
	ENTRADA_SALIDA,  //CPU le devuelve el proceso a planificador
	TIEMPO_CPU,     //solicitud del planificador al cpu
	TIEMPO_CPU_RESUL,
	RESULT_TIEMPO_CPU,
	RESUL_ENT_SAL,

	RESUL_INICIAR_PROC_OK_CPU, //lo usa el CPU para recibir de memoria el ok al iniciar
	RESUL_INICIAR_PROC_NO_OK_CPU, //lo usa CPU para recibir error por no poder iniciar desde memoria
	RESUL_INICIAR_PROC_ERROR_CPU,
	RESUL_INICIAR_PROC,
	RESUL_INICIAR_PROC_OK,
	RESUL_INICIAR_PROC_ERROR,

	RESUL_OK,
//	RESUL_ERROR,
	RESUL_ESCRIBIR,  //lo usa CPU para recibir lo escrito desde la CPU
	RESUL_ESCRIBIR_OK,
	//	RESUL_ESCRIBIR_ERROR,
	RESUL_LEER_OK,
	RESUL_LEER_OK_CPU, //lo usa el CPU para recibir desde la memoria lo leido.
	RESUL_FIN_OK,
	RESUL_FIN, // lo usa CPU para recibir de memoria el fin de proceso
//	RESUL_FIN_ERROR,
//	RESUL_LEER_ERROR,

//+++++++++swap++++++++++++++++++
	 RESUL_SOBREESCRIBIR_OK,
	 NOTIFICACION_HILO_ENTRADA_SALIDA

} t_tipo_mensaje;

typedef struct PaqueteEnvio {
	uint32_t tamanioMensaje;
	void* mensaje;
} PaqueteEnvio;

typedef struct Paquete {
	uint32_t tipoDeMensaje;
	void* payLoad;
} Paquete;

typedef struct PCB {
	uint8_t pid;
	char* rutaArchivoMcod;
	bool tieneDesalojo;
	uint16_t tamanioRafaga;
	uint16_t proximaInstruccion;
	uint16_t instruccionFinal;
	bool finalizar;
} t_pcb;

typedef struct Planificacion {
	char* nombreAlgoritmo;
	bool tieneDesalojo;
	uint16_t tamanioRafaga;
} t_planificacion;

typedef struct CPU_REF {
	int socket;
	char* nombre;
	bool conectada;
	t_pcb* pcb;
	bool ejecutando;
} t_cpu_ref;

//SWAP Y MEMORIA

typedef struct IniciarSwap {
	uint8_t PID;
	uint16_t cantidadPaginas;
} t_iniciar_swap;

typedef struct {
	uint8_t PID;
} t_PID;

//OK
typedef struct {
	uint8_t PID;
	char* contenido;
	uint8_t numeroPagina;
} t_contenido_pagina;

typedef struct {
	uint8_t PID;
	uint8_t numeroPaginaInicio;
	uint8_t numeroPaginaFin;
} t_leerDeProceso;

typedef struct {
	uint8_t PID;
	uint8_t numeroPaginaInicio;
	uint8_t numeroPaginaFin;
	char* textoAEscribir;
} t_leerDeProcesoPorEscribir;

typedef struct {
	t_pcb* pcb; //aca dentro ya esta el PID del proceso
	char* resultadosInstrucciones;
	bool finalizoOk;
	uint8_t cant_entrada_salida;// vale 0 si no hay que hacer entrada salida
} t_respuesta_ejecucion;

typedef struct {
	uint8_t res_porcentaje;
	pthread_t idCpu;
}t_respuesta_porcentaje;

typedef struct {
	t_list* respuestasPorcentaje;
	uint8_t cantidadDeElementos;
}t_porcentajeCPUs;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++ FUNCIONES  ++++++++++++++++++++++++++++++++++++++++++++++

PaqueteEnvio* serializar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad);
Paquete* deserializar(void* buffer, int tamanioMensaje);
//+++ interaccion planificador CPU+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++SE USA para mandar mensaje a CPU 
void* serializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje);
//++++++ SE USA  dentro de serializar_CONTEXTO_MPROC  deserializar_CONTEXTO_MPROC
void* serializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_pcb* estructura);
t_pcb* deserializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje);
//++++ SE USA  como nivel inferior para serializar un string
void serializar_string(int fdCliente, char* estructura);
char* deserializar_string(int fdCliente);
//++++++SE USA como nivel inferior para serializar un int16
void serializar_int16_t(int fdCliente, int16_t estructura);
int16_t deserializar_int16_t(int fdCliente);
//++++++SE USA como nivel inferior para serializar un int8
void serializar_int8_t(int fdCliente, int8_t estructura);
int8_t deserializar_int8_t(int fdCliente);
//++++++SE USA como nivel inferior para serializar un bool
void serializar_bool(int fdCliente, bool estructura);
bool deserializar_bool(int fdCliente);
void serializar_pthread(int fdCliente, pthread_t pthread);
pthread_t deserializar_pthread(int fdCliente);

// +++++++++interaccion CPU - Memoria
void* serializar_INICIAR_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_INICIAR_PROCESO_MEM(int fdCliente,
		t_tipo_mensaje tipoMensaje);
void 	serializar_lista_porcentajes(int fdCliente, t_list* estructura);
t_list* deserializar_lista_porcentajes(int fdCliente,int8_t cantidad);
void serializar_TIEMPO_CPU_RESUL(int fdCliente, t_tipo_mensaje tipoMensaje, t_porcentajeCPUs*  estructura);
void* deserializar_TIEMPO_CPU_RESUL(int fdCliente, t_tipo_mensaje tipoMensaje		);
//--------------------------------------------++++++++++++++ Especificar donde se utiliza
void* serializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_iniciar_swap* estructura);
t_iniciar_swap* deserializar_t_iniciar_swap(int fdCliente,
		t_tipo_mensaje tipoMensaje);

//++++++SE USA como nivel inferior para serializar un PID
void serializar_pid_t(int fdCliente, pid_t estructura);
pid_t deserializar_pid_t(int fdCliente);
// +++++++++interaccion Memoria - swap
void* serializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);
// +++++++++++++++++++++++++++++++++++++++++++++++++++ resultados de operaciones CPU a planificador
// ++++++++++++++++++++++++++++++++++++++++++desde linea 253 a linea 330
// ++++++++++++++++++++++++++se propone unificar los resultados, para cuando es un tipo de datos similar
void* serializar_RESUL_INICIAR_PROC_OK(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_INICIAR_PROC_OK(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_INICIAR_PROC_ERROR(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_INICIAR_PROC_ERROR(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_FIN_PROCESO_MEM(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_FIN_PROCESO_MEM(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura);
t_PID* deserializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje);
//+++++++++++++++++++++++++++++ SE DUPLICA CON serializar_RESUL_INICIAR_PROC_OK
void* serializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_PID* estructura);
t_PID* deserializar_t_rta_iniciar_ok_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void serializar_t_contenido_pagina(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_contenido_pagina* estructura);
t_contenido_pagina* deserializar_t_contenido_pagina(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura);
t_PID* deserializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_t_rta_iniciar_no_ok_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje, t_PID* estructura);
t_PID* deserializar_t_rta_iniciar_no_ok_CPU(int fdCliente,
		t_tipo_mensaje tipoMensaje);

t_leerDeProcesoPorEscribir* deserializar_t_leerDeProcesoPorEscribir(int fdCliente, t_tipo_mensaje tipoMensaje) ;
void* serializar_t_leerDeProcesoPorEscribir(int fdCliente, t_tipo_mensaje tipoMensaje, t_leerDeProcesoPorEscribir* estructura);


//+++++++++++++++++++++++++++++ FIN DUPLICADO+++++++++++++++++++++++++++++++++
void serializar_TIEMPO_CPU(int fdCliente, t_tipo_mensaje tipoMensaje,t_PID*  estructura);

void* deserializar_TIEMPO_CPU(int fdCliente, t_tipo_mensaje tipoMensaje		);

void serializar_NOTIFICACION_HILO_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje,t_PID* estructura);

void* deserializar_NOTIFICACION_HILO_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_LEER_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_LEER_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* serializar_RESUL_ESCRIBIR_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje);
void* deserializar_RESUL_ESCRIBIR_ERROR(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* serializar_RESUL_LEER_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_LEER_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje);
void* deserializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_FIN(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_FIN(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* serializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje);
void* deserializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_t_PID(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_PID* estructura);
t_PID* deserializar_t_PID(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_SOBREESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_SOBREESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
t_PID* deserializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);

void serializar_RESUL_EJECUCION_OK(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_RESUL_EJECUCION_OK(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_t_respuesta_ejecucion(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
t_respuesta_ejecucion* deserializar_t_respuesta_ejecucion(int fdCliente,
		t_tipo_mensaje tipoMensaje);
//+++++++++se usa en CPU como parte de los resultados a mas bajo nivel
void serializar_t_resultado_instruccion(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura);

//+++++++++++++++++++++++++++Fin de resultados+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++interaccion Memoria -  Swap
void* serializar_FIN_PROCESO_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_FIN_PROCESO_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_LEER_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_LEER_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_LEER_MEM(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_LEER_MEM(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_ESCRIBIR_MEM(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_ESCRIBIR_MEM(int fdCliente, t_tipo_mensaje tipoMensaje);
void serializar_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje,
		void* estructura);
void* deserializar_ENTRADA_SALIDA(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void* serializar_t_leerDeProceso(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_leerDeProceso* estructura);
t_leerDeProceso* deserializar_t_leerDeProceso(int fdCliente,
		t_tipo_mensaje tipoMensaje);

void serializar_t_tipo_mensaje(int fdCliente, t_tipo_mensaje tipoMensaje,
		t_tipo_mensaje estructura);
t_tipo_mensaje deserializar_t_tipo_mensaje(int fdCliente
		);

void* serializar_ESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_ESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_RESUL_SOBREESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_SOBREESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_LEER_SWAP_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_LEER_SWAP_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_RESUL_TRAER_PAG_SWAP_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_TRAER_PAG_SWAP_OK(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_RESUL_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje);


#endif /* UTILES_PROTOCOLO_H_ */
