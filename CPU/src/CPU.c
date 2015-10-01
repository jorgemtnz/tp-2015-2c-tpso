#include "CPU.h"

t_dictionary* conexiones;
int main(void) {
	conexiones = dictionary_create();

	logger = log_create("LOG_CPU.log", "CPU", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion();

	int socketPlanificador;
	int socketMemoria;
	int resultConexion_mem = 0;
	int resultConexion_planif = 0;

	resultConexion_planif = conectar(configuracion->vg_ipPlanificador, string_itoa(configuracion->vg_puertoPlanificador), &socketPlanificador);
	if (resultConexion_planif == -1)
		log_error(logger, "[ERROR]no se conecto el CPU al Planificador");

	dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));

	resultConexion_mem = conectar(configuracion->vg_ipMemoria, string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);
	if (resultConexion_mem == -1)
		log_error(logger, "[ERROR]no se conecto el CPU a la memoria");

	dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));

	escucharConexiones("0", socketPlanificador, socketMemoria, 0, procesarMensajes, NULL, logger);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	int resultConexion_planif = 0;

	puts("CPU procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra, logger);

	if (tipoNotificacion == NEW_CONNECTION) {
		log_info(logger, "se realizo nueva conección");
	} else if (tipoNotificacion == TERMINAL_MESSAGE) {
		//comando auxiliar para reconectar al planificador manualmente
		//escribir cp en la terminal y enter (cp= conectar planificador)
		if (string_starts_with(buffer, "cp")) {
			int socketPlanificador;
			resultConexion_planif = conectar(configuracion->vg_ipPlanificador, string_itoa(configuracion->vg_puertoPlanificador), &socketPlanificador);
			if (resultConexion_planif == -1)
				log_error(logger, "[ERROR]no se reconecto el CPU al Planificador");
			dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));
		}
	} else if (tipoNotificacion == MESSAGE) {

		if (header->tipoMensaje == CONTEXTO_MPROC) {
			log_info(logger, "llega mensaje CONTEXTO_MPROC ");
			t_pcb* pcbPlanificador = (t_pcb*) buffer;
			printf("Ruta recibida del planificador: %s\n", pcbPlanificador->rutaArchivoMcod);
			preparaCPU(pcbPlanificador, socket);
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

