

#include "Memoria.h"


int main(void) {
	FILE* archivoMemoria;
	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion();

	archivoMemoria = fopen(configuracion->nombreMemoria, "w");
	fputs("\0", archivoMemoria);
	fclose(archivoMemoria);
	return EXIT_SUCCESS;
}
