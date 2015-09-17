#include "CPU.h"

t_dictionary* conexiones;
int main(void) {
	conexiones = dictionary_create();

	logger = log_create("LOG_CPU.log", "CPU", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion();

	int socketPlanificador;
	int socketMemoria;

	conectar(configuracion->vg_ipPlanificador, string_itoa(configuracion->vg_puertoPlanificador), &socketPlanificador);
	dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));
	conectar(configuracion->vg_ipMemoria, string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);
	dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));

	escucharConexiones("0", socketPlanificador, socketMemoria, 0, procesarMensajes, NULL, logger);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("CPU procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, nuevaConexion, extra, logger);

	if(nuevaConexion) {

	} else {
		if(header->tipoMensaje == STRING) {

			char* mensaje = malloc(header->tamanioMensaje);
			recibirPorSocket(socket, mensaje, header->tamanioMensaje);
			printf("Recibi el mensaje: %s\n", mensaje);
		}

/*		if(string_starts_with(buffer, "correr programa")) {
			char* socketCPU = (char*)dictionary_get(conexiones, "Memoria");
			puts("Enviando \"correr programa\" a la Memoria");
			enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
			// de aca para abajo serian las conexiones

		}*/
	}

	return 0;
}


