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

int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	puts("CPU procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra, logger);

	if (tipoNotificacion == NEW_CONNECTION) {

	} else if (tipoNotificacion == TERMINAL_MESSAGE) {
		//comando auxiliar para reconectar al planificador manualmente
		//escribir cp en la terminal y enter (cp= conectar planificador)
		if (string_starts_with(buffer, "cp")) {
			int socketPlanificador;
			conectar(configuracion->vg_ipPlanificador, string_itoa(configuracion->vg_puertoPlanificador), &socketPlanificador);
			dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));
		}
	} else if (tipoNotificacion == MESSAGE) {

		if(header->tipoMensaje == CONTEXTO_MPROC) {
			t_pcb* pcbPlanificador = (t_pcb*) buffer;
			printf("Ruta recibida del planificador: %s\n", pcbPlanificador->rutaArchivoMcod);
		}
//		if (header->tipoMensaje == STRING) {
//
//			char* mensaje = malloc(header->tamanioMensaje);
//			recibirPorSocket(socket, mensaje, header->tamanioMensaje);
//			printf("Recibi el mensaje: %s\n", mensaje);
//		}
		/*		if(string_starts_with(buffer, "correr programa")) {
		 char* socketCPU = (char*)dictionary_get(conexiones, "Memoria");
		 puts("Enviando \"correr programa\" a la Memoria");
		 enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
		 // de aca para abajo serian las conexiones

		 }*/
	} else if (tipoNotificacion == HANG_UP) {

	}

	return 0;
}


