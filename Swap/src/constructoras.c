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

t_contenido_pagina* crearEscribirEnProceso(){

	t_contenido_pagina* contenido = malloc(sizeof(t_contenido_pagina));
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

t_PID* crearEstructuraPid(){

	t_PID* contenido = malloc(sizeof(t_PID));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	return contenido;
}


t_devolucion_escribir_o_leer* crearDevolucionEscribirOLeer(){
	t_devolucion_escribir_o_leer* contenido = malloc(sizeof(t_devolucion_escribir_o_leer));
		if (contenido == NULL) {
			perror("[ERROR] No se reservo memoria para Swap>..>contenido");
			log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
			exit(-1);
		}
		contenido->PID = 0;
		contenido->contenido = "";
		contenido->numeroPagina = 0;
		contenido->resultado = OK;
		return contenido;

}

t_contenido_pagina* crearContenidoPagina(){
	t_contenido_pagina* contenido = malloc(sizeof(t_contenido_pagina));
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
t_contenido_pagina* crearRespuestaLeer(){

	t_contenido_pagina* contenido = malloc(sizeof(t_contenido_pagina));
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
t_contador * crearContador(){

	t_contador* contenido = malloc(sizeof(t_contador));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->escrituras = 0;
	contenido->lecturas = 0;
	contenido->PID = 0;

	return contenido;

}
t_PID* crearRespuestaFinalizar(){

	t_PID* contenido = malloc(sizeof(t_PID));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	return contenido;
}

t_respuesta_iniciar_o_finalizar* crearDevolucionIniciarOFinalizar(){

	t_respuesta_iniciar_o_finalizar* contenido = malloc(sizeof(t_respuesta_iniciar_o_finalizar));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para Swap>..>contenido");
		log_error(logger, "[ERROR] No se reservo memoria para Swap>..>contenido");
		exit(-1);
	}
	contenido->PID = 0;
	contenido->resultado = OK;

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
	contenido->contenido = string_new();


	return contenido;
}
