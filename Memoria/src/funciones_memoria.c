#include "Memoria.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfig;
	t_config* archivoConfig;
	int result, fd_archivo = 0;
	result = 0;

	nombreArchivoConfig = strdup(
			"/home/utnso/tp-2015-2c-tpso/Memoria/config_memoria.cfg");
	result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {

		archivoConfig = config_create(nombreArchivoConfig);

		configuracion = malloc(sizeof(t_configuracion));
		configuracion->puertoCpu = config_get_int_value(archivoConfig,
				"PUERTO_ESCUCHA");
		configuracion->puertoSwap = config_get_int_value(archivoConfig,
				"PUERTO_SWAP");
		configuracion->nombreMemoria = strdup(configuracion->ipSwap =
				config_get_string_value(archivoConfig, "IP_SWAP"));
		configuracion->nombreMemoria = strdup(
				config_get_string_value(archivoConfig, "NOMBRE_MEMORIA"));
		configuracion->maximosMarcosPorProceso = config_get_int_value(
				archivoConfig, "MAXIMO_MARCOS_POR_PROCESO");
		configuracion->cantidadMarcos = config_get_int_value(archivoConfig,
				"CANTIDAD_MARCOS");
		configuracion->tamanioMarcos = config_get_int_value(archivoConfig,
				"TAMANIO_MARCO");
		configuracion->entradasTlb = config_get_int_value(archivoConfig,
				"ENTRADAS_TLB");
		configuracion->retardoMemoria = config_get_int_value(archivoConfig,
				"RETARDO_MEMORIA");
		configuracion->tlbHabilitada = config_get_int_value(archivoConfig,
				"TLB_HABILITADA");
		log_info(logger,
				"[INFO]: Archivo de configuracion leido correctamente");
	}
	config_destroy(archivoConfig);
	close(fd_archivo);
}
