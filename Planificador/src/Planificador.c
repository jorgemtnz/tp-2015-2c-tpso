#include "Planificador.h"


int main(int argc, char *argv[]) {
	conexiones = dictionary_create();

	putsConsola("Iniciando programa");
	crearLogger();
	leerArchivoDeConfiguracion(argc, argv);
	escucharConexiones(configuracion->puertoEscucha, 0, 0, 0, procesarMensajes, NULL, logger);
	levantarConsola();

	putsConsola("Programa finalizado");
	return EXIT_SUCCESS;
}

void crearLogger() {
	logger = log_create("LOG_PLAN.log", "Planificador", false, LOG_LEVEL_INFO);
}


int procesarMensajes(int socket, t_header* header, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	putsConsola("Planificador procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, nuevaConexion, extra, logger);
	if(nuevaConexion) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
	} else {
		if(socket == STDIN_FILENO) {
			procesarMensajesConsola(socket, header, buffer);
		}
	}
	return 0;
}
