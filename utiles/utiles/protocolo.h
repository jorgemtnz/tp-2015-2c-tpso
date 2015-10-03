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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++  DEFINE   +++++++++++++++++++++++++++++++++++++++++++++++++++



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++  ESTRUCTURAS   ++++++++++++++++++++++++++++++++++++++++

typedef enum {
	HEADER,
	STRING,

	CONTEXTO_MPROC,
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
	RESUL_INICIAR_PROC,
	RESUL_INICIAR_PROC_OK,
	RESUL_INICIAR_PROC_ERROR,
	RESUL_ESCRIBIR,
	RESUL_LEER,
	RESUL_FIN,
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
	int numeroPaginaInicio;
	int numeroPaginaFin;
} t_leerDeProceso;

typedef struct{
	uint8_t PID;
	uint8_t pagina;
	char *texto;
}t_escribirMem;

typedef struct{
	uint8_t PID;
	uint8_t pagina;
	char* texto;
}t_leerMem;

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

void* serializar_t_respuesta_iniciar(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_iniciar* estructura);
t_respuesta_iniciar* deserializar_t_respuesta_iniciar(int fdCliente, t_tipo_mensaje tipoMensaje);


#endif /* UTILES_PROTOCOLO_H_ */
