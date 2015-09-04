#include "Memoria.h"

int main(int argc, char *argv[]) {

	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);


	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap), &socketSwap);
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, socketSwap);
	int prueba = 10;
	send(socketSwap,&prueba,sizeof(int),0);
	recv(socketSwap,&prueba,sizeof(int),0);
	printf("%i\n",prueba);

	return EXIT_SUCCESS;
}
