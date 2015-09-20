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
#define CONTEXTO_MPROC  1
#define RESUL_INSTR_EJEC  2
#define INICIAR_PROC    3
#define LEER            4
#define ESCRIBIR        5
#define FIN_PROCESO     6
#define RESUL_OK        7
#define RESUL_ERROR     8
#define RESUL_INICIAR_PROC 9
#define RESUL_ESCRIBIR  10
#define RESUL_LEER      11
#define RESUL_FIN       12
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++  ESTRUCTURAS   ++++++++++++++++++++++++++++++++++++++++

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
	uint32_t proximaInstruccion;
} t_pcb;

typedef struct Planificacion  {
	char* nombreAlgoritmo;
	bool tieneDesalojo;
	uint16_t tamanioRafaga;
} t_planificacion;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++ FUNCIONES  ++++++++++++++++++++++++++++++++++++++++++++++

PaqueteEnvio* serializar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad);
Paquete* deserializar(void* buffer,int tamanioMensaje);

#endif /* UTILES_PROTOCOLO_H_ */
