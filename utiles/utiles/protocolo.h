/*
 * protocolo.h
 *
 *  Created on: 8/9/2015
 *      Author: utnso
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
	HEADER=1,
	STRING,

	CONTEXTO_MPROC,
	RESULT_EJECUCION_OK,
	RESULT_EJECUCION_ERROR,
	RESUL_INSTR_EJEC,
	INICIAR_PROC_SWAP,
	LEER_SWAP,
	ESCRIBIR_SWAP,
	FIN_PROCESO_SWAP,
	INICIAR_PROCESO_MEM,
	LEER_MEM,
	ESCRIBIR_MEM,
	FIN_PROCESO_MEM,
	ENTRADA_SALIDA,
	TIEMPO_CPU,
	RESULT_TIEMPO_CPU,
	RESUL_ENT_SAL,
	RESUL_OK,
	RESUL_ERROR,
	RESUL_LEER,
	RESUL_FIN,
	RESUL_ESCRIBIR,
	RESUL_INICIAR_PROC,
	RESUL_INICIAR_PROC_OK,
	RESUL_INICIAR_PROC_NO_OK_CPU,
	RESUL_INICIAR_PROC_OK_CPU,
	RESUL_INICIAR_PROC_ERROR_CPU,
	RESUL_INICIAR_PROC_ERROR,
	RESUL_ESCRIBIR_OK,
	RESUL_ESCRIBIR_ERROR,
	RESUL_LEER_OK,
	RESUL_LEER_ERROR,
	TRAER_PAG_SWAP,
	RESUL_TRAER_PAG_SWAP_OK,
	RESUL_TRAER_PAG_SWAP_NO_OK,
	RESUL_FIN_OK,
	RESUL_FIN_ERROR,
} t_tipo_mensaje;

typedef struct PaqueteEnvio{
 uint32_t tamanioMensaje;
	void* mensaje;
 }PaqueteEnvio;

typedef struct Paquete{
	uint32_t tipoDeMensaje;
	void* payLoad;
}Paquete;

typedef struct PCB {
	uint8_t pid;
	char* rutaArchivoMcod;
	bool tieneDesalojo;
	uint16_t tamanioRafaga;
	uint16_t proximaInstruccion;
} t_pcb;

typedef struct Planificacion  {
	char* nombreAlgoritmo;
	bool tieneDesalojo;
	uint16_t tamanioRafaga;
} t_planificacion;

typedef struct CPU_REF {
	int socket;
	char* nombre;
	bool conectada;
} t_cpu_ref;


//SWAP Y MEMORIA

typedef struct IniciarSwap {
	uint8_t PID;
	uint16_t cantidadPaginas;
} t_iniciar_swap;

typedef struct {
	uint8_t PID;
}t_rta_iniciar_CPU;


typedef struct {
	uint8_t PID;
} t_finalizar_swap;

typedef struct{
	uint8_t PID;
}t_respuesta_iniciar;

typedef struct {
	uint8_t PID;
	char* contenido;
	int numeroPagina;
} t_escribirEnProceso;

typedef struct {
	uint8_t PID;
	char* contenido;
	uint8_t numeroPagina;
} t_respuesta_escribir;

typedef struct {
	uint8_t PID;
	int numeroPaginaInicio;
	int numeroPaginaFin;
} t_leerDeProceso;

typedef struct {
	uint8_t PID;
	char* contenido;
	uint8_t numeroPagina;
} t_respuesta_leer;

typedef struct {
	uint8_t PID;
} t_respuesta_finalizar;

typedef struct{
	uint8_t PID;
	uint8_t pagina;
	char* texto;
}t_escribirMem;

typedef struct{
	uint8_t PID;
	uint8_t pagina;
	char* texto;
}t_leerMem;

typedef struct{
	uint8_t PID;
	char* expresion;
} t_entrada_salida;

typedef struct{
	t_pcb* pcb; //aca dentro ya esta el PID del proceso
	t_list* resultadosInstrucciones;
	bool finalizoOk;
} t_respuesta_ejecucion;

typedef struct{
	t_tipo_mensaje tipoMensaje;
	char* comandoInstruccion;
	char* expresion;
} t_resultado_instruccion;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++ FUNCIONES  ++++++++++++++++++++++++++++++++++++++++++++++

PaqueteEnvio* serializar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad);
Paquete* deserializar(void* buffer,int tamanioMensaje);

void* serializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje, t_pcb* estructura);
t_pcb* deserializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_string(int fdCliente, char* estructura);
char* deserializar_string(int fdCliente);

void* serializar_int16_t(int fdCliente, int16_t estructura);
int16_t deserializar_int16_t(int fdCliente);

void* serializar_int8_t(int fdCliente, int8_t estructura);
int8_t deserializar_int8_t(int fdCliente);

void* serializar_bool(int fdCliente, bool estructura);
bool deserializar_bool(int fdCliente);

void* serializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje, t_iniciar_swap* estructura);
t_iniciar_swap* deserializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_pid_t(int fdCliente, pid_t estructura);
pid_t deserializar_pid_t(int fdCliente);

void* serializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) ;

void* serializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje) ;

void* serializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_iniciar* estructura);
t_rta_iniciar_CPU* deserializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_t_rta_iniciar_no_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_iniciar* estructura);
t_rta_iniciar_CPU* deserializar_t_rta_iniciar_no_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje);
RSUL_TRAER_PAG_SWAP_OK

void* serializar_t_respuesta_iniciar(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_iniciar* estructura);
t_respuesta_iniciar* deserializar_t_respuesta_iniciar(int fdCliente, t_tipo_mensaje tipoMensaje);


void* serializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) ;
void* serializar_RESUL_ESCRIBIR_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje) ;
void* deserializar_RESUL_ESCRIBIR_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_t_respuesta_escribir(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_escribir* estructura);
t_respuesta_escribir* deserializar_t_respuesta_escribir(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_t_lecturaProc_desde_CPU(int fdCliente, t_tipo_mensaje tipoMensaje,t_respuesta_finalizar* estructura);
t_lecturaProc_desde_CPU* deserializar_t_lecturaProc_desde_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) ;

void* serializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* serializar_RESUL_LEER_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_LEER_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje);
void* deserializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_t_respuesta_leer(int fdCliente, t_tipo_mensaje tipoMensaje,t_respuesta_leer* estructura) ;
t_respuesta_leer* deserializar_t_respuesta_leer(int fdCliente, t_tipo_mensaje tipoMensaje);

void* serializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* serializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura);
void* deserializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) ;
void* deserializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje);
void* serializar_t_respuesta_finalizar(int fdCliente, t_tipo_mensaje tipoMensaje,t_respuesta_finalizar* estructura);
t_respuesta_finalizar* deserializar_t_respuesta_finalizar(int fdCliente, t_tipo_mensaje tipoMensaje);

#endif /* UTILES_PROTOCOLO_H_ */
