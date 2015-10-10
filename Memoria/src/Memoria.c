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
	int socketSwap,socketCPU;
	socketSwap = atoi((char*) dictionary_get(conexiones, "Swap"));

	t_iniciar_swap* datosDesdeCPU = (t_iniciar_swap*) buffer;
	t_iniciar_swap * estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();

	if (tipoNotificacion == NEW_CONNECTION) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
	} else {
		if (tipoNotificacion == MESSAGE) {
			switch (header->tipoMensaje) {
			case (RESUL_INICIAR_PROC_OK) :{
				t_iniciar_swap* datosDesdeSwap = (t_iniciar_swap*) buffer;
				estructuraIniciar->PID = datosDesdeSwap->PID;
				estructuraIniciar->cantidadPaginas = datosDesdeSwap->cantidadPaginas;
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				iniciar(estructuraIniciar->PID, estructuraIniciar->cantidadPaginas,socketCPU);
				break;
			}
			case (RESUL_INICIAR_PROC_ERROR) : {
				t_PID* rtaDesdeSwap = (t_PID*) buffer;
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				enviarRtaIniciarFalloCPU(rtaDesdeSwap,socketCPU);
				break;
			}
			case (RESUL_ESCRIBIR_OK): {
				break;
			}
			case (RESUL_FIN_OK) : {
				t_PID * datosDesdeSwap = (t_PID*) buffer;
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				enviarFinalizarACPU(datosDesdeSwap, socketCPU);
				break;
			}
			case (INICIAR_PROCESO_MEM) : {
				t_iniciar_swap* datosDesdeCPU = (t_iniciar_swap*) buffer;
				estructuraIniciar->PID = datosDesdeCPU->PID;
				estructuraIniciar->cantidadPaginas = datosDesdeCPU->cantidadPaginas;
				enviarIniciarASwap(estructuraIniciar, socketSwap);
				break;
			}
			case (FIN_PROCESO_MEM) : {
				t_PID* datosDesdeCPU = (t_PID*) buffer;
				t_PID* estructuraFinalizar;
				estructuraFinalizar = crearEstructuraFinalizar();
				estructuraFinalizar->PID = datosDesdeCPU->PID;
				enviarFinalizarASwap(estructuraFinalizar, socketSwap);
				break;
			}
			case (LEER_MEM): {
				t_lectura_Swap* datosDesdeCPU = (t_lectura_Swap*) buffer;
				log_info(logger, "leer pag %d\n", datosDesdeCPU->numeroPagina);
				t_leerDeProceso* estructuraLeer;
				estructuraLeer = crearEstructuraLeer();

				/*leer(estructuraLeer->PID,estructuraLeer->numeroPaginaInicio,socketSwap,socketCPU);
				*/
				enviarStruct(socketSwap, LEER_SWAP, estructuraLeer);
				break;
			}
			case (RESUL_TRAER_PAG_SWAP_NO_OK): {
				break;

			}
			case (RESUL_TRAER_PAG_SWAP_OK): {
				t_lectura_Swap * datosDesdeSwap = (t_lectura_Swap*) buffer;
				t_contenido_pagina* estructuraRtaLeer;
				estructuraRtaLeer->PID = datosDesdeSwap->idProc;
				estructuraRtaLeer->numeroPagina= datosDesdeSwap->numeroPagina;
				estructuraRtaLeer->contenido = datosDesdeSwap->contenido;
				/*respuestaTraerDeSwapUnaPaginaDeUnProceso(estructuraRtaLeer->idProc, estructuraRtaLeer->pag, estructuraRtaLeer->contenido, socketCPU);
				*/
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				enviarACPUContenidoPaginaDeUnProceso(estructuraRtaLeer,socketCPU);


				break;
			}
			case (ESCRIBIR_MEM): {
				break;
			}

			}

		} else if (tipoNotificacion == TERMINAL_MESSAGE) {

		}
	}

	return 0;
	pthread_mutex_unlock(&mutexProcesadorMensajes);
}

