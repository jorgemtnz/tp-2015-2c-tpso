#include "Memoria.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();

	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);


	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap), &socketSwap);
	dictionary_put(conexiones, "Swap", string_itoa(socketSwap));
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, socketSwap, procesarMensajes, NULL, logger);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("Memoria procesar mensajes");
	defaultProcesarMensajes(socket, buffer, nuevaConexion, extra, logger);
	if(nuevaConexion) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
	} else {
		printf("Recibi el mensaje: %s\n", buffer);
		if(string_starts_with(buffer, "correr programa")) {
			char* socketCPU = (char*)dictionary_get(conexiones, "Swap");
			puts("Enviando \"correr programa\" al Swap");
			enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
			puts("Enviado al Swap");
		}
	}


	return 0;
}
