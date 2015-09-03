#include "Planificador.h"

int main(int argc, char *argv[]) {
	puts("Iniciando programa");
	crearLogger();
	leerArchivoDeConfiguracion(argc, argv);
	escucharConexiones(configuracion->puertoEscucha, 0, 0, 0);
	levantarConsola();

	puts("Programa finalizado");
	return EXIT_SUCCESS;
}

void crearLogger() {
	logger = log_create("LOG_PLAN.log", "Planificador", false, LOG_LEVEL_INFO);
}
