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
			perror("[ERROR] No se reservo memoria para Swap>..>proceso");
			log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
			exit(-1);
		}
		return tablaDePag;
}

t_escrituraProc* iniciarEscrituraProc() {
	t_escrituraProc* escrituraProc = malloc(sizeof(t_escrituraProc));
	if (escrituraProc == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>proceso");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
		exit(-1);
	}
	return escrituraProc;
}

t_TLB* iniciarTLB() {
	t_TLB* TLB = malloc(sizeof(t_TLB));
	if (TLB == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>proceso");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
		exit(-1);
	}
	return TLB;
}

t_marco * iniciarMarco() {
	t_marco* marco = malloc(sizeof(t_marco));
	if (marco == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>proceso");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
		exit(-1);
	}
	return marco;
}

t_config* iniciarArchivoConfig() {
	t_config* archConf = malloc(sizeof(t_config));
	if (archConf == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>proceso");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
		exit(-1);
	}
	return archConf;
}

t_lecturaSwap* iniciarLectura() {
	t_lecturaSwap* lectura = malloc(sizeof(t_lecturaSwap));
	if (lectura == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>proceso");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
		exit(-1);
	}
	return lectura;
}



t_rtaLecturaCpu* iniciarRtaLecturaCpu(){
	t_rtaLecturaCpu* rtaLecturaCpu = malloc(sizeof(t_rtaLecturaCpu));
			if (rtaLecturaCpu == NULL) {
				perror("[ERROR] No se reservo memoria para Swap>..>proceso");
				log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
				exit(-1);
			}
			return rtaLecturaCpu;

}
