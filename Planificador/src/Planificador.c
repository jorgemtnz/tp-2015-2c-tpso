#include "Planificador.h"


int main(int argc, char *argv[]) {
	conexiones = dictionary_create();

	putsConsola("Iniciando programa");
	crearLogger();
	listaCPUs = list_create();
	leerArchivoDeConfiguracion(argc, argv);
	escucharConexiones(configuracion->puertoEscucha, 0, 0, 0, procesarMensajes, NULL, logger);
	levantarConsola();

	putsConsola("Programa finalizado");
	return EXIT_SUCCESS;
}

void crearLogger() {
	logger = log_create("LOG_PLAN.log", "Planificador", false, LOG_LEVEL_INFO);
}


int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	putsConsola("Planificador procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra, logger);
	if(tipoNotificacion == NEW_CONNECTION) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
		//falta reconocer si ya estuvo conectada y es una reconexion
		registrarNuevaCPU(socket);

	} else if(tipoNotificacion == TERMINAL_MESSAGE) {
		procesarMensajesConsola(socket, header, buffer);
	} else if(tipoNotificacion == MESSAGE) {

	} else if(tipoNotificacion == HANG_UP) {
		desregistrarCPUConectada(socket);
	}
	return 0;
}
