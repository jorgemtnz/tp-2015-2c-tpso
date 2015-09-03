#include "Memoria.h"

int main(int argc, char *argv[]) {
	printf("a\n");
	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);
	printf("a\n");

	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap), &socketSwap);
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, socketSwap);

	return EXIT_SUCCESS;
}
