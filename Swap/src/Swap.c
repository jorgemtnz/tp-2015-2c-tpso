#include "Swap.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();


	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	crearArchivo();
	l_espacioLibre *listaEspLibre;
	listaEspLibre = (l_espacioLibre*) malloc( sizeof(l_espacioLibre) );
	l_procesosCargados *listaProc;
	listaProc = (l_procesosCargados*) malloc( sizeof(l_procesosCargados) );



	int socketMemoria, prueba;
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);
	//escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajesDeMemoria, NULL, logger);
//	recv(socketMemoria, &prueba, sizeof(int), 0);
//	printf("%i\n", prueba);
//	prueba = 5;
//	send(socketMemoria, &prueba, sizeof(int), 0);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("Swap procesar mensajes");
	defaultProcesarMensajes(socket, buffer, nuevaConexion, extra, logger);
	if(nuevaConexion) {

	} else {
		printf("Recibi el mensaje: %s\n", buffer);
	}
	return 0;
}
