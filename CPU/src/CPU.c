#include "CPU.h"


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
        int resultConexion_Memoria = 0;
        int resultadoMensajeToken = 3 ; 
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
		
		if (string_starts_with(buffer, "cm")) {
			int socketMemoria;
			resultConexion_Memoria = conectar(configuracion->vg_ipMemoria, string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);
			if (resultConexion_Memoria == -1)
				log_error(logger, "[ERROR]no se reconecto el CPU a la Memoria");
			dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));
		}
		
	} else if (tipoNotificacion == MESSAGE) {
		
		//como espero varios resultados, entonces los deje en una sola fucnion
		// sino es un resultado no devuelve  EXIT_SUCCES entonces es otra cosa
		// se encuentra en enviaryrecibir.c
		resultadoMensajeToken = recibirMensajeVarios(socket,header->tipoMensaje, buffer , extra );
		
		 if(resultadoMensajeToken != EXIT_SUCCESS)
		 log_error(logger, "[ERROR] el Tipo Mensaje no corresponde");
		
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
           log_error(logger, "[ERROR] se desconecto un proceso");
	}

	return 0;
}

