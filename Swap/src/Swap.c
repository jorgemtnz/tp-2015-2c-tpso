#include "Swap.h"

int main(int argc, char *argv[]) {
	FILE* archivoSwap;
	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	archivoSwap = fopen(configuracion->nombreSwap, "w");
	fputs("\0", archivoSwap);
	fclose(archivoSwap);
	return EXIT_SUCCESS;
}
