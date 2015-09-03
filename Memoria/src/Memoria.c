

#include "Memoria.h"

int main(int argc, char *argv[]) {
	printf("a\n");
	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc,argv);
	printf("a\n");

	return EXIT_SUCCESS;
}
