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
		log_error(logger, "[ERROR] No se reservo memoria para iniciarTablaDePaginas");
		exit(-1);
	}
	return tablaDePag;
}

t_escrituraProc* iniciarEscrituraProc() {
	t_escrituraProc* escrituraProc = malloc(sizeof(t_escrituraProc));
	if (escrituraProc == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarEscrituraProc");
		log_error(logger, "[ERROR] No se reservo memoria para iniciarEscrituraProc");
		exit(-1);
	}
	return escrituraProc;
}

t_TLB* iniciarTLB() {
	t_TLB* TLB = malloc(sizeof(t_TLB));
	if (TLB == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarTLB");
		log_error(logger, "[ERROR] No se reservo memoria para iniciarTLB");
		exit(-1);
	}
	return TLB;
}

t_marco * iniciarMarco() {
	t_marco* marco = malloc(sizeof(t_marco));
	if (marco == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarMarco");
		log_error(logger, "[ERROR] No se reservo memoria para iniciarMarco");
		exit(-1);
	}
	return marco;
}

t_config* iniciarArchivoConfig() {
	t_config* archConf = malloc(sizeof(t_config));
	if (archConf == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarArchivoConfig");
		log_error(logger, "[ERROR] No se reservo memoria para iniciarArchivoConfig");
		exit(-1);
	}
	return archConf;
}

t_lectura* iniciarLectura() {
	t_lectura* lectura = malloc(sizeof(t_lectura));
	if (lectura == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarLectura");
		log_error(logger, "[ERROR] No se reservo memoria para iniciarLectura");
		exit(-1);
	}
	return lectura;
}



t_rtaLecturaCpu* iniciarRtaLecturaCpu(){
	t_rtaLecturaCpu* rtaLecturaCpu = malloc(sizeof(t_rtaLecturaCpu));
	if (rtaLecturaCpu == NULL) {
		perror("[ERROR] No se reservo memoria para iniciarRtaLecturaCpu");
		log_error(logger, "[ERROR] No se reservo memoria para iniciarRtaLecturaCpu");
		exit(-1);
	}
	return rtaLecturaCpu;

}
t_iniciar_swap* crearEstructuraIniciar(){
	t_iniciar_swap* contenido = malloc(sizeof(t_iniciar_swap));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearEstructuraIniciar");
		log_error(logger, "[ERROR] No se reservo memoria para crearEstructuraIniciar");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->cantidadPaginas = 0;


	return contenido;
}

t_finalizar_swap* crearEstructuraFinalizar(){
	t_finalizar_swap* contenido = malloc(sizeof(t_finalizar_swap));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearEstructuraFinalizar");
		log_error(logger, "[ERROR] No se reservo memoria para crearEstructuraFinalizar");
		exit(-1);
	}
	contenido->PID = 0;
	return contenido;
}

t_rta_iniciar_CPU* crearRespuestaIniciarOkCPU(){
	t_rta_iniciar_CPU* contenido = malloc(sizeof(t_rta_iniciar_CPU));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearRespuestaIniciarOkCPU");
		log_error(logger, "[ERROR] No se reservo memoria para crearRespuestaIniciarOkCPU");
		exit(-1);
	}
	return contenido;
}


