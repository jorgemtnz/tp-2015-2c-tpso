

#include "Memoria.h"


int main(void) {
	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion();
	printf("a\n");

	return EXIT_SUCCESS;
}
