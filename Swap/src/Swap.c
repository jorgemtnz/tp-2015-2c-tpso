#include "Swap.h"

int main(int argc, char *argv[]) {

	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	crearArchivo();

	int socketMemoria, prueba;
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, socketMemoria);

	recv(socketMemoria, &prueba, sizeof(int), 0);
	printf("%i\n", prueba);
	prueba = 5;
	send(socketMemoria, &prueba, sizeof(int), 0);

	return EXIT_SUCCESS;
}
