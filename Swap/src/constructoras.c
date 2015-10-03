/*
 * constructoras.c
 *
 *  Created on: 8/9/2015
 *      Author: utnso
 */

#include "Swap.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------FUNCIONES CONTRUCTORAS-----------------------

l_procesosCargados* crearProceso(){
	l_procesosCargados* proceso = malloc(sizeof(l_procesosCargados));
	if (proceso == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>proceso");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
		exit(-1);
	}
	proceso->PID = 0;
	proceso->cantPagsUso = 0;
	proceso->ubicacion = 0;
	return proceso;
}

l_espacioLibre* crearEspacioLibre(){
	l_espacioLibre* espacio = malloc(sizeof(l_espacioLibre));
	if (espacio == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>espacioLibre");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>espacioLibre");
		exit(-1);
	}
	espacio->cantPagsLibres = 0;
	espacio->ubicacion = 0;
	return espacio;

}

t_escribirEnProceso* crearEscribirEnProceso(){

	t_escribirEnProceso* contenido = malloc(sizeof(t_escribirEnProceso));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->numeroPagina = 0;
	contenido->contenido = '\0';
	return contenido;
}

t_leerDeProceso* crearLeerDeProceso(){
	t_leerDeProceso* contenido = malloc(sizeof(t_leerDeProceso));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->numeroPaginaFin = 0;
	contenido->numeroPaginaInicio = 0;
	return contenido;
}

t_iniciar_swap* crearEstructuraIniciar(){
	t_iniciar_swap* contenido = malloc(sizeof(t_iniciar_swap));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->cantidadPaginas = 0;
	return contenido;
}

t_respuesta_iniciar* crearRespuestaIniciar(){

	t_respuesta_iniciar* contenido = malloc(sizeof(t_respuesta_iniciar));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	return contenido;
}

t_respuesta_escribir* crearRespuestaEscribir(){

	t_respuesta_escribir* contenido = malloc(sizeof(t_respuesta_escribir));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->contenido = "";
	contenido->numeroPagina = 0;
	return contenido;

}

t_respuesta_leer* crearRespuestaLeer(){

	t_respuesta_leer* contenido = malloc(sizeof(t_respuesta_leer));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->contenido = "";
	contenido->numeroPagina = 0;
	return contenido;

}

t_respuesta_finalizar* crearRespuestaFinalizar(){

	t_respuesta_finalizar* contenido = malloc(sizeof(t_respuesta_finalizar));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	return contenido;
}
