#include "Memoria.h"


int main(int argc, char *argv[]) {
	conexiones = dictionary_create();
	listaMemoria = list_create();
	listaTLB = list_create();
	listaTablaDePag = list_create();
	contadorPagTP = 0;
	variableIdMarco = 0;

	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger

	leerArchivoDeConfiguracion(argc, argv);

	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap), &socketSwap);
	dictionary_put(conexiones, "Swap", string_itoa(socketSwap));
	t_iniciar_swap* estructura;
//	estructura = crearEstructuraIniciar();
//	estructura->PID = 2;
//	estructura->cantidadPaginas = 14;
//	enviarIniciarASwap(estructura, socketSwap);
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, socketSwap, procesarMensajes, NULL, logger);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	pthread_mutex_lock(&mutexProcesadorMensajes);
	puts("Memoria procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra, logger);
	if (tipoNotificacion == NEW_CONNECTION) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
	} else {
		if (tipoNotificacion == MESSAGE) {
			if (header->tipoMensaje == RESUL_INICIAR_PROC_OK) {

				//ACA VA LO QUE TIENE QUE HACER MEMORIA CON EL CUANDO ESTA OK EL INICIAR EN SWAP
			} else {
				if (header->tipoMensaje == RESUL_INICIAR_PROC_ERROR) {
					//ACA VA LO QUE TIENE QUE HACER MEMORIA CON EL CUANDO ESTA EN ERROR EL INICIAR EN SWAP
				} else {
					if (header->tipoMensaje == RESUL_ESCRIBIR) {
						//ACA VA LO QUE TIENE QUE HACER MEMORIA CUANDO SWAP LE DICE QUE ESCRIBIR SE HIZO BIEN
					} else {
						if (header->tipoMensaje == RESUL_FIN) {
							//ACA VA LO QUE TIENE QUE HACER MEMORIA CUANDO SWAP LE DICE QUE FINALIZO EL PROCESO(LO BORRO)
						} else {
							if (header->tipoMensaje == INICIAR_PROC_SWAP) {//TODO HACK usar un tipo de mensaje especifico

								//TODO HACK usar un tipo especifico
								t_iniciar_swap* datosDesdeCPU = (t_iniciar_swap*) buffer;

								t_iniciar_swap* estructura;
								estructura = crearEstructuraIniciar();
								estructura->PID = datosDesdeCPU->PID;
								estructura->cantidadPaginas = datosDesdeCPU->cantidadPaginas;
								int socketSwap = atoi((char*) dictionary_get(conexiones, "Swap"));
								enviarIniciarASwap(estructura, socketSwap);
							}
						}
					}
				}
			}
		}
	}
	//printf("Recibi el mensaje: %s\n", buffer);
	if (tipoNotificacion == TERMINAL_MESSAGE) {
		if (string_starts_with(buffer, "correr programa")) {
			char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
			/* EJEMPLO
			 puts("Enviando \"correr programa\" al Swap");
			 enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
			 puts("Enviado al Swap");
			 */
			int buff, idProc, cantPag, nroPag, pagIn, pagFin; // seria el buffer ques e deba poner
			char* textoAEscribir;
			switch (buff) {
			case 1: // va a ser iniciar este
				iniciar(idProc, cantPag, socketCPU);
				break;
			case 2: // va a ser escribir este
				escribir(idProc, nroPag, textoAEscribir);
				break;
			case 3: // va a ser leer este
				leer(idProc, pagIn, pagFin);
				break;
			case 4: // va a ser finalizar este
				finalizar(idProc);
				break;
			}
		}
	}

	return 0;
	pthread_mutex_unlock(&mutexProcesadorMensajes);
}

