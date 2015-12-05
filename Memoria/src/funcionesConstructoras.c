/*
 * constructoras.c
 *
 *  Created on: 15/9/2015
 *      Author: utnso
 */

#include "Memoria.h"

t_TablaDePaginas* iniciarTablaDePaginas(){
	t_TablaDePaginas* tablaDePag = malloc(sizeof(t_TablaDePaginas));
	if (tablaDePag == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarTablaDePaginas");
		my_log_error("[ERROR] No se reservo memoria para iniciarTablaDePaginas");
		exit(-1);
	}
	return tablaDePag;
}

t_error* iniciarError(){
	t_error* error = malloc(sizeof(error));
	if (error == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarError");
		my_log_error("[ERROR] No se reservo memoria para iniciarError");
		exit(-1);
	}
	return error;
}


t_TLB* iniciarTLB() {
	t_TLB* TLB = malloc(sizeof(t_TLB));
	if (TLB == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarTLB");
		my_log_error("[ERROR] No se reservo memoria para iniciarTLB");
		exit(-1);
	}
	return TLB;
}

t_marco * iniciarMarco() {
	t_marco* marco = malloc(sizeof(t_marco));
	if (marco == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarMarco");
		my_log_error("[ERROR] No se reservo memoria para iniciarMarco");
		exit(-1);
	}
	return marco;
}

t_marco_con_flag * iniciarMarcoYFlag() {
	t_marco_con_flag* marcoYFlag = malloc(sizeof(t_marco_con_flag));
	marcoYFlag->marco = iniciarMarco();
	if (marcoYFlag == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarMarcoYFlag");
		my_log_error("[ERROR] No se reservo memoria para iniciarMarcoYFlag");
		exit(-1);
	}
	return marcoYFlag;
}

t_marco_con_indice* iniciarMarcoYIndice() {
	t_marco_con_indice* marcoConIndice = malloc(sizeof(t_marco_con_indice));
	marcoConIndice->marco = iniciarMarco();
	if (marcoConIndice == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarMarcoYFlag");
		my_log_error("[ERROR] No se reservo memoria para iniciarMarcoYFlag");
		exit(-1);
	}
	return marcoConIndice;
}

t_configuracion* iniciarArchivoConfig() {
	t_configuracion* archConf = (t_configuracion*)malloc(sizeof(t_configuracion));
	if (archConf == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarArchivoConfig");
		my_log_error("[ERROR] No se reservo memoria para iniciarArchivoConfig");
		exit(-1);
	}
	return archConf;
}

t_lectura* iniciarLectura() {
	t_lectura* lectura = malloc(sizeof(t_lectura));
	if (lectura == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarLectura");
		my_log_error("[ERROR] No se reservo memoria para iniciarLectura");
		exit(-1);
	}
	return lectura;
}

t_marco_y_bit* iniciarMarcoYBit(){
	t_marco_y_bit* marcoYBit = malloc(sizeof(t_marco_y_bit));
	if (marcoYBit == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarMarcoYBit");
		my_log_error("[ERROR] No se reservo memoria para iniciarMarcoYBit");
		exit(-1);
	}
	return marcoYBit;
}


t_contenido_pagina* iniciarContenidoPagina(){
	t_contenido_pagina* contenido = malloc(sizeof(t_contenido_pagina));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarRtaLecturaCpu");
		my_log_error("[ERROR] No se reservo memoria para iniciarRtaLecturaCpu");
		exit(-1);
	}
	contenido->contenido = string_new();
	return contenido;

}
t_iniciar_swap* crearEstructuraIniciar(){
	t_iniciar_swap* contenido = malloc(sizeof(t_iniciar_swap));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearEstructuraIniciar");
		my_log_error("[ERROR] No se reservo memoria para crearEstructuraIniciar");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->cantidadPaginas = 0;


	return contenido;
}

t_sobreescribir_swap* crearEstructuraReemplazar(){
	t_sobreescribir_swap* contenido = malloc(sizeof(t_sobreescribir_swap));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearEstructuraReemplazar");
		my_log_error("[ERROR] No se reservo memoria para crearEstructuraReemplazar");
		exit(-1);
	}
	contenido->PIDAReemplazar = 0;
	contenido->PIDAResponderleAMemoria = 0;
	contenido->numeroPagina = 0;


	return contenido;
}


t_escribir_falso* crearEscribirFalso(){
	t_escribir_falso* contenido = malloc(sizeof(t_escribir_falso));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearEscribirFalso");
		my_log_error("[ERROR] No se reservo memoria para crearEscribirFalso");
		exit(-1);
	}
	return contenido;
}

t_PID* crearPID(){
	t_PID* contenido = malloc(sizeof(t_PID));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearEstructuraFinalizar");
		my_log_error("[ERROR] No se reservo memoria para crearEstructuraFinalizar");
		exit(-1);
	}
	return contenido;
}

t_leerDeProceso* crearEstructuraLeer(){
	t_leerDeProceso* contenido = malloc(sizeof(t_leerDeProceso));
		if (contenido == NULL) {
			perror("[ERROR] No se reservo memoria para crearEstructuraFinalizar");
			my_log_error("[ERROR] No se reservo memoria para crearEstructuraFinalizar");
			exit(-1);
		}
		contenido->PID = 0;
		contenido->numeroPaginaFin = 0;
		contenido->numeroPaginaInicio = 0;
		return contenido;
}

t_leerDeProcesoPorEscribir* crearEstructuraLeerProcesoPorEscribir(){
	t_leerDeProcesoPorEscribir* contenido = malloc(sizeof(t_leerDeProcesoPorEscribir));
		if (contenido == NULL) {
			perror("[ERROR] No se reservo memoria para crearEstructuraLeerProcesoPorEscribir");
			my_log_error("[ERROR] No se reservo memoria para crearEstructuraLeerProcesoPorEscribir");
			exit(-1);
		}
		contenido->PID = 0;
		contenido->numeroPaginaFin = 0;
		contenido->numeroPaginaInicio = 0;
		contenido->textoAEscribir = string_new();
		return contenido;
}
