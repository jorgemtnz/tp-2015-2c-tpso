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

#include <stdlib.h>
#include <stdio.h>

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
 uint8_t tamanioMensaje;
	void* mensaje;
 }PaqueteEnvio;

typedef struct Paquete{
	uint8_t tipoDeMensaje;
	void* payLoad;
}Paquete;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++ FUNCIONES  ++++++++++++++++++++++++++++++++++++++++++++++

PaqueteEnvio* serializar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad);
Paquete* deserializar(void* buffer,int tamanioMensaje);

#endif /* UTILES_PROTOCOLO_H_ */
