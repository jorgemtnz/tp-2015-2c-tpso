#include "Planificador.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();

	puts("Iniciando programa");
	crearLogger();
	leerArchivoDeConfiguracion(argc, argv);
	escucharConexiones(configuracion->puertoEscucha, 0, 0, 0, procesarMensajes, NULL, logger);
	levantarConsola();

	puts("Programa finalizado");
	return EXIT_SUCCESS;
}

void crearLogger() {
	logger = log_create("LOG_PLAN.log", "Planificador", false, LOG_LEVEL_INFO);
}


int procesarMensajes(int socket, t_header* header, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("Planificador procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, nuevaConexion, extra, logger);
	if(nuevaConexion) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
	} else {
		if(socket == STDIN_FILENO && buffer != NULL) {
			char** split = string_split(buffer, "\n");
			buffer = split[0];
		} else {
			printf("Recibi el mensaje: %s\n", buffer);
		}
		if(string_starts_with(buffer, "correr programa")) {
			char* socketCPU = (char*)dictionary_get(conexiones, "CPU");
			puts("Enviando \"correr programa\" a la CPU");
			//enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
			enviarStruct(atoi(socketCPU), STRING, "correr programa");
		}
	}
	return 0;
}
