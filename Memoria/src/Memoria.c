#include "Memoria.h"

int main(int argc, char *argv[]) {
	conexiones = dictionary_create();
	inicializacionDesdeCero();

	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger

	leerArchivoDeConfiguracion(argc, argv);
	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap),
			&socketSwap);
	dictionary_put(conexiones, "Swap", string_itoa(socketSwap));
	// warning variable no usada, entonces se comenta
//	t_iniciar_swap* estructura;
//	estructura = crearEstructuraIniciar();
//	estructura->PID = 2;
//	estructura->cantidadPaginas = 14;
//	enviarIniciarASwap(estructura, socketSwap);
	if (hayQueEjecutarTests(argc, argv)) {
		return ejecutarTests();
	}
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0,
			socketSwap, procesarMensajes, NULL, logger);

	return EXIT_SUCCESS;
}

int socketCPU;
int procesarMensajes(int socket, t_header* header, char* buffer,
		t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	pthread_mutex_lock(&mutexProcesadorMensajes);
	puts("Memoria procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra,
			logger);
	int socketSwap;
	socketSwap = atoi((char*) dictionary_get(conexiones, "Swap"));
//warning variable no usada, se esta declarando en cada case del switch, entonces la comento
//	t_iniciar_swap* datosDesdeCPU = (t_iniciar_swap*) buffer;
	t_iniciar_swap * estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();

	if (tipoNotificacion == NEW_CONNECTION) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
		socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
	} else {
		if (tipoNotificacion == MESSAGE) {
			//no se puede sacar este warning, porque no hace falta contemplar todos los casos del enum
			switch (header->tipoMensaje) {
			case (RESUL_INICIAR_PROC_OK): {
				t_iniciar_swap* datosDesdeSwap = (t_iniciar_swap*) buffer;
				estructuraIniciar->PID = datosDesdeSwap->PID;
				estructuraIniciar->cantidadPaginas =
						datosDesdeSwap->cantidadPaginas;
				printf("%i\n", estructuraIniciar->cantidadPaginas);
				iniciar(estructuraIniciar->PID,
						estructuraIniciar->cantidadPaginas, socketCPU);
				break;
			}
			case (RESUL_INICIAR_PROC_ERROR): {
				t_PID* rtaDesdeSwap = (t_PID*) buffer;
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				enviarRtaIniciarFalloCPU(rtaDesdeSwap, socketCPU);
				break;
			}
			case (RESUL_ESCRIBIR_OK): {
				t_contenido_pagina* datosdesdeSwap =
						(t_contenido_pagina*) buffer;
				enviarRtaEscribirACPU(datosdesdeSwap, socketCPU);

				break;
			}
			case (RESUL_FIN_OK): {
				t_PID * datosDesdeSwap = (t_PID*) buffer;
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				enviarFinalizarACPU(datosDesdeSwap, socketCPU);
				break;
			}
			case (INICIAR_PROCESO_MEM): {
				t_iniciar_swap* datosDesdeCPU = (t_iniciar_swap*) buffer;
				estructuraIniciar->PID = datosDesdeCPU->PID;
				estructuraIniciar->cantidadPaginas =
						datosDesdeCPU->cantidadPaginas;
				//sleep(configuracion->retardoMemoria);
				printf("mando a swap %i\n", estructuraIniciar->cantidadPaginas);
				enviarIniciarASwap(estructuraIniciar, socketSwap);
				break;
			}
			case (FIN_PROCESO_MEM): {
				t_PID* datosDesdeCPU = (t_PID*) buffer;
				t_PID* estructuraFinalizar;
				estructuraFinalizar = crearPID();
				estructuraFinalizar->PID = datosDesdeCPU->PID;
				//sleep(configuracion->retardoMemoria);
				finalizar(estructuraFinalizar, socketSwap);
				break;
			}
			case (LEER_MEM): {
				t_contenido_pagina* datosDesdeCPU = (t_contenido_pagina*) buffer;
				log_info(logger, "leer pag %d del proceso %d\n",
						datosDesdeCPU->numeroPagina, datosDesdeCPU->PID);
				t_leerDeProceso* estructuraLeer;
				estructuraLeer = crearEstructuraLeer();
				leer(estructuraLeer->PID, estructuraLeer->numeroPaginaInicio,
						socketSwap, socketCPU);

				break;
			}
			case (RESUL_TRAER_PAG_SWAP_OK): {
				t_contenido_pagina * datosDesdeSwap =
						(t_contenido_pagina*) buffer;
				t_contenido_pagina* estructuraRtaLeer;
				estructuraRtaLeer = iniciarContenidoPagina();
				estructuraRtaLeer = datosDesdeSwap;
				int flagEscritura = 0;
				respuestaTraerDeSwapUnaPaginaDeUnProceso(estructuraRtaLeer->PID,
						estructuraRtaLeer->numeroPagina,
						estructuraRtaLeer->contenido, flagEscritura, socketCPU,
						socketSwap);

				break;
			}
			case (RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR): {
				t_contenido_pagina * datosDesdeSwap =
						(t_contenido_pagina*) buffer;
				t_contenido_pagina* estructuraRtaLeerPorEscribir;
				estructuraRtaLeerPorEscribir = iniciarContenidoPagina();
				estructuraRtaLeerPorEscribir = datosDesdeSwap;
				int flagEscritura = 1;
				respuestaTraerDeSwapUnaPaginaDeUnProceso(
						estructuraRtaLeerPorEscribir->PID,
						estructuraRtaLeerPorEscribir->numeroPagina,
						estructuraRtaLeerPorEscribir->contenido, flagEscritura,
						socketCPU, socketSwap);
				break;
			}
			case (ESCRIBIR_MEM): {
				t_contenido_pagina* datosDesdeCPU = (t_contenido_pagina*) buffer;
				log_info(logger, "leer pag %d del proceso %d\n",
						datosDesdeCPU->numeroPagina, datosDesdeCPU->PID);
				t_contenido_pagina* estructuraEscribir;
				estructuraEscribir = iniciarContenidoPagina();
				estructuraEscribir->PID = datosDesdeCPU->PID;
				estructuraEscribir->numeroPagina = datosDesdeCPU->numeroPagina;
				estructuraEscribir->contenido = datosDesdeCPU->contenido;
				socketCPU = atoi((char*) dictionary_get(conexiones, "CPU"));
				escribir(estructuraEscribir->PID,
						estructuraEscribir->numeroPagina,
						estructuraEscribir->contenido, socketSwap, socketCPU);

				break;
			}
			case (RESUL_SOBREESCRIBIR_OK): {
				/* solo se usa en las funciones de sacar a un marco de memoria entonces
				 en la respuesta de sobreescribir a swap, se va a mandar a cpu el contenido,
				 que es lo que se manda en el caso que no haya que sacar alguno */
				t_contenido_pagina* datosDesdeCPU = (t_contenido_pagina*) buffer;
				log_info(logger,
						"resultado sobreescribir ok de pag %d del proceso %d\n",
						datosDesdeCPU->numeroPagina, datosDesdeCPU->PID);
				t_contenido_pagina* lecturaMandarCpu;
				lecturaMandarCpu = iniciarContenidoPagina();
				lecturaMandarCpu = datosDesdeCPU;
				enviarACPUContenidoPaginaDeUnProcesoPorLeer(lecturaMandarCpu,
						socketCPU);
				break;
			}
			default:
				printf("mensaje no valido");

			}

		} else if (tipoNotificacion == TERMINAL_MESSAGE) {

		}
	}

	return 0;
	pthread_mutex_unlock(&mutexProcesadorMensajes);
}

char* decirHolaMundo() {
	return "Hola Mundo";
}

char* getNombre() {
	return "Memoria";
}
