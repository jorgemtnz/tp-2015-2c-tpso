#include "Planificador.h"

int main(int argc, char *argv[]) {
	puts("Iniciando programa");
	crearLogger();
	cargarArchivoDeConfiguracion(argc, argv);
	levantarConsola();

	puts("Programa finalizado");
	return EXIT_SUCCESS;
}

void cargarArchivoDeConfiguracion(int argc, char *argv[]) {

	if(argc < 2) {
		printf("[ERROR]: Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./Planificador /home/utnso/tp-2015-2c-tpso/Planificador/config_plan.cfg\n");
		log_error(logger, "[ERROR]: Debe especificar la ruta al archivo de configuracion, por ejemplo: /home/utnso/tp-2015-2c-tpso/Planificador/config_plan.cfg");
		exit(-1);
	}

	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
	int result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		printf("[ERROR]: Archivo de configuracion no encontrado. Parametro especificado: %s\n", nombreArchivoConfig);
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {
		t_config* archivoConfig = config_create(nombreArchivoConfig);

		configuracion = malloc(sizeof(t_configuracion));
		configuracion->puertoEscucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
		configuracion->algorimoPlanificacion = strdup(config_get_string_value(archivoConfig, "ALGORITMO_PLANIFICACION"));
		configuracion->quantum = config_get_int_value(archivoConfig, "QUANTUM");

		printf("[INFO]: Archivo de configuracion leido correctamente\n");
		log_info(logger, "[INFO]: Archivo de configuracion leido correctamente");

		config_destroy(archivoConfig);
	}
}

void crearLogger() {
	logger = log_create("LOG_PLAN.log", "Planificador", false, LOG_LEVEL_INFO);
}
