

#include "Memoria.h"


int main(void) {
	FILE* archivoMemoria;
	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion();


	return EXIT_SUCCESS;
}
