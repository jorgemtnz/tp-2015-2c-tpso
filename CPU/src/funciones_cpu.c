#include "CPU.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfig;
	t_config* archivoConfig = NULL;
    int result, fd_archivo=0; result=0;

	nombreArchivoConfig =strdup( "/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg");
	result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {
		archivoConfig = config_create(nombreArchivoConfig);
		configuracion = malloc(sizeof(t_configuracion));
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
		log_info(logger,
				"[INFO]: Archivo de configuracion leido correctamente");
	}
	config_destroy(archivoConfig);
//	close(fd_archivo);
}
// se debe agregar instruccion que mape el archivo -> devuelva estructura

// se debe agregar funcion que intreprente instruccion ya mapeada

//se debe agregar funcion que envie mensaje.


