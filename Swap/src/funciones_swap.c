#include "Swap.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfig;
	t_config* archivoConfig = NULL;
	int result, fd_archivo = 0;
	result = 0;

	nombreArchivoConfig = strdup(
			"/home/utnso/tp-2015-2c-tpso/Swap/config_swap.cfg");
	result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {
		archivoConfig = config_create(nombreArchivoConfig);

		configuracion->puertoMemoria = config_get_int_value(archivoConfig,
				"PUERTO_ESCUCHA");
		configuracion->nombreSwap = strdup(
				config_get_string_value(archivoConfig, "NOMBRE_SWAP"));
		configuracion->cantidadPaginas = config_get_int_value(archivoConfig,
				"CANTIDAD_PAGINAS");
		configuracion->tamanioPagina = config_get_int_value(archivoConfig,
				"TAMANIO_PAGINA");
		configuracion->retardo = config_get_int_value(archivoConfig,
				"RETARDO_COMPACTACION");
		log_info(logger,
				"[INFO]: Archivo de configuracion leido correctamente");
	}
	config_destroy(archivoConfig);
	close(fd_archivo);
}
