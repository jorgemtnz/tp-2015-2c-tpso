#include "CPU.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfigFS;
	t_config* archivoConfig = NULL;

	nombreArchivoConfigFS = "/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg";
	if (checkearRutaArchivoConfig(nombreArchivoConfigFS) == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {
		archivoConfig = config_create(nombreArchivoConfigFS);
		configuracion->vg_puertoPlanificador = config_get_int_value(
				archivoConfig, "PUERTO_PLANIFICADOR");
		configuracion->vg_ipPlanificador = strdup(
				config_get_string_value(archivoConfig, "IP_PLANIFICADOR"));
		configuracion->vg_puertoMemoria = config_get_int_value(archivoConfig,
				"PUERTO_MEMORIA");
		configuracion->vg_ipMemoria = strdup(
				config_get_string_value(archivoConfig, "IP_MEMORIA"));
		configuracion->cantidad_hilos = config_get_int_value(archivoConfig,
				"CANTIDAD_HILOS");
		configuracion->retardo = config_get_int_value(archivoConfig, "RETARDO");

	}
}

