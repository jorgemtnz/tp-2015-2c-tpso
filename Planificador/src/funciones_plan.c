/*
 * funciones_plan.c
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#include "Planificador.h"


void leerArchivoDeConfiguracion(int argc, char *argv[]) {

	char* logMsg = NULL;

	if(argc < 2) {
		logMsg = string_from_format("Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./Planificador /home/utnso/tp-2015-2c-tpso/Planificador/config_plan.cfg\n");
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	}

	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
	int result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		logMsg = string_from_format("Archivo de configuracion no encontrado. Parametro especificado: %s\n", nombreArchivoConfig);
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	} else {
		t_config* archivoConfig = config_create(nombreArchivoConfig);

		configuracion = (t_configuracion*) malloc(sizeof(t_configuracion));
		configuracion->puertoEscucha = strdup(config_get_string_value(archivoConfig, "PUERTO_ESCUCHA"));
		configuracion->algorimoPlanificacion = strdup(config_get_string_value(archivoConfig, "ALGORITMO_PLANIFICACION"));
		configuracion->quantum = config_get_int_value(archivoConfig, "QUANTUM");

		logMsg = string_from_format("Archivo de configuracion leido correctamente\n");
		puts(logMsg);
		log_error(logger, logMsg);

		config_destroy(archivoConfig);
	}
}
