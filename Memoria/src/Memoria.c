#include "Memoria.h"

int main(int argc, char *argv[]) {
	conexiones = dictionary_create();
	inicializacionDesdeCero();
	asignarSeniales();

	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger

	leerArchivoDeConfiguracion(argc, argv);
	if (hayQueEjecutarTests(argc, argv)) {
		return ejecutarTests();
	}
	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap),
			&socketSwap);
	dictionary_put(conexiones, "Swap", string_itoa(socketSwap));
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0,
			socketSwap, procesarMensajes, NULL, logger);

	//levantarConsola();
	return EXIT_SUCCESS;
}
t_list* listaSocketsCPU;
//int socketCPU;
int procesarMensajes(int socket, t_header* header, char* buffer,
		t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	if(listaSocketsCPU == NULL) {
		listaSocketsCPU = list_create();
	}

	//puts("Memoria procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra,
			logger);
	int socketSwap;
	socketSwap = atoi((char*) dictionary_get(conexiones, "Swap"));
	t_iniciar_swap * estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
	if (tipoNotificacion == NEW_CONNECTION) {
		list_add(listaSocketsCPU, string_itoa(socket));
	} else {
		if (tipoNotificacion == MESSAGE) {

			//no se puede sacar este warning, porque no hace falta contemplar todos los casos del enum
			switch (header->tipoMensaje) {
			case (RESUL_INICIAR_PROC_OK): {
				t_iniciar_swap* datosDesdeSwap = (t_iniciar_swap*) buffer;
				estructuraIniciar->PID = datosDesdeSwap->PID;
				estructuraIniciar->cantidadPaginas =
						datosDesdeSwap->cantidadPaginas;
				iniciar(estructuraIniciar->PID,
						estructuraIniciar->cantidadPaginas, getSocketCPU(estructuraIniciar->PID));
				char* textoLogger = string_new();
				string_append(&textoLogger,string_from_format("Proceso mProc creado,  PID: %i ,cantidad de páginas asignadas: %i\n",datosDesdeSwap->PID,datosDesdeSwap->cantidadPaginas));
				my_log_info(textoLogger);
				break;
			}
			case (RESUL_INICIAR_PROC_ERROR): {
				t_PID* rtaDesdeSwap = (t_PID*) buffer;
				int socketCPU = getSocketCPU(rtaDesdeSwap->PID);
				enviarRtaIniciarFalloCPU(rtaDesdeSwap, socketCPU);
				break;
			}
			case (RESUL_ESCRIBIR_OK): {
				t_contenido_pagina* datosdesdeSwap =
						(t_contenido_pagina*) buffer;
				enviarRtaEscribirACPU(datosdesdeSwap, getSocketCPU(datosdesdeSwap->PID));

				break;
			}
			case (RESUL_FIN_OK): {
				t_PID * datosDesdeSwap = (t_PID*) buffer;
				int socketCPU = getSocketCPU(datosDesdeSwap->PID);
				enviarFinalizarACPU(datosDesdeSwap, socketCPU);



				break;
			}
			case (INICIAR_PROCESO_MEM): {
				t_iniciar_swap* datosDesdeCPU = (t_iniciar_swap*) buffer;
				estructuraIniciar->PID = datosDesdeCPU->PID;
				estructuraIniciar->cantidadPaginas =
						datosDesdeCPU->cantidadPaginas;
				usleep(configuracion->retardoMemoria * 1000);
				registrarPidCpu(socket, datosDesdeCPU->PID);
				enviarIniciarASwap(estructuraIniciar, socketSwap);
				break;
			}
			case (FIN_PROCESO_MEM): {
				t_PID* datosDesdeCPU = (t_PID*) buffer;
				t_PID* estructuraFinalizar;
				estructuraFinalizar = crearPID();
				estructuraFinalizar->PID = datosDesdeCPU->PID;
				usleep(configuracion->retardoMemoria * 1000);
				registrarPidCpu(socket, datosDesdeCPU->PID);

				t_TablaDePaginas* campoTablaDePag;

				campoTablaDePag = iniciarTablaDePaginas();

				char* textoALoggear = string_new();
				string_append(&textoALoggear, "TABLA DE PAGINAS FINAL: ");
				int a, b;
				int pag, id;

				t_marco* campoMemoria;
				campoMemoria = iniciarMarco();
				for (a = 0; a < list_size(listaMemoria); a++) {
					campoMemoria = list_get(listaMemoria, a);
					for (b = 0; b < list_size(listaTablaDePag); b++) {
						campoTablaDePag = list_get(listaTablaDePag, b);
						if (campoMemoria->idMarco == campoTablaDePag->idMarco) {
							pag = campoTablaDePag->paginaDelProceso;
							id = campoTablaDePag->idMarco;
							string_append(&textoALoggear, string_from_format("Marco: %i, Pagina: %i ;", id, pag));
						}

					}
				}
				my_log_info(textoALoggear);

				finalizar(estructuraFinalizar, socketSwap);
				break;
			}
			case (LEER_MEM): {
				t_contenido_pagina* datosDesdeCPU = (t_contenido_pagina*) buffer;
				aux =1;
				my_log_info("leer pag %d del proceso %d\n",
						datosDesdeCPU->numeroPagina, datosDesdeCPU->PID);
				registrarPidCpu(socket, datosDesdeCPU->PID);
				leer(datosDesdeCPU->PID, datosDesdeCPU->numeroPagina,
						socketSwap, getSocketCPU(datosDesdeCPU->PID));

				break;
			}
			case (RESUL_LEER_OK): {
				t_contenido_pagina * datosDesdeSwap =
						(t_contenido_pagina*) buffer;
				t_contenido_pagina* estructuraRtaLeer;
				estructuraRtaLeer = iniciarContenidoPagina();
				estructuraRtaLeer = datosDesdeSwap;
				int flagEscritura = 0;
				respuestaTraerDeSwapUnaPaginaDeUnProceso(estructuraRtaLeer->PID,
						estructuraRtaLeer->numeroPagina,
						estructuraRtaLeer->contenido, flagEscritura, getSocketCPU(estructuraRtaLeer->PID),
						socketSwap);

				break;
			}
			case (RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR): {
				t_contenido_pagina * datosDesdeSwap =
						(t_contenido_pagina*) buffer;
				t_contenido_pagina* estructuraRtaLeerPorEscribir;
				estructuraRtaLeerPorEscribir = iniciarContenidoPagina();

				string_append(&estructuraRtaLeerPorEscribir->contenido,datosDesdeSwap->contenido);

				estructuraRtaLeerPorEscribir->PID = datosDesdeSwap->PID;
				estructuraRtaLeerPorEscribir->numeroPagina = datosDesdeSwap->numeroPagina;

				int flagEscritura = 1;
				respuestaTraerDeSwapUnaPaginaDeUnProceso(
						estructuraRtaLeerPorEscribir->PID,
						estructuraRtaLeerPorEscribir->numeroPagina,
						estructuraRtaLeerPorEscribir->contenido, flagEscritura,
						getSocketCPU(datosDesdeSwap->PID), socketSwap);
				break;
			}
			case (ESCRIBIR_MEM): {
				t_contenido_pagina* datosDesdeCPU = (t_contenido_pagina*) buffer;
				aux =0;
				my_log_info("leer pag %d del proceso %d\n",
						datosDesdeCPU->numeroPagina, datosDesdeCPU->PID);
				t_contenido_pagina* estructuraEscribir;
				estructuraEscribir = iniciarContenidoPagina();
				estructuraEscribir->PID = datosDesdeCPU->PID;
				estructuraEscribir->numeroPagina = datosDesdeCPU->numeroPagina;
				estructuraEscribir->contenido = datosDesdeCPU->contenido;
				registrarPidCpu(socket, datosDesdeCPU->PID);
				int socketCPU = getSocketCPU(datosDesdeCPU->PID);
				escribir(estructuraEscribir->PID,estructuraEscribir->numeroPagina,
						estructuraEscribir->contenido, socketSwap, socketCPU);

				break;
			}
			case (RESUL_SOBREESCRIBIR_OK): {
				/* solo se usa en las funciones de sacar a un marco de memoria entonces
				 en la respuesta de sobreescribir a swap, se va a mandar a cpu el contenido,
				 que es lo que se manda en el caso que no haya que sacar alguno */
				t_contenido_pagina* datosDesdeCPU = (t_contenido_pagina*) buffer;
				my_log_info("resultado sobreescribir ok de pag %d del proceso %d\n",
						datosDesdeCPU->numeroPagina, datosDesdeCPU->PID);
				t_contenido_pagina* lecturaMandarCpu;
				lecturaMandarCpu = iniciarContenidoPagina();
				lecturaMandarCpu = datosDesdeCPU;
				enviarACPUContenidoPaginaDeUnProcesoPorLeer(lecturaMandarCpu,
						getSocketCPU(datosDesdeCPU->PID));
				break;
			}
			default:
				printf("mensaje no valido");

			}

		} else if (tipoNotificacion == TERMINAL_MESSAGE) {
			procesarMensajesConsola(socket,header,buffer);
		}
	}

	return 0;
}

void asignarSeniales(){
	if (signal(SIGUSR1,atencionSIGUSR1)==SIG_ERR){
		my_log_error("No pudo cambiarse la señal SIGUSR1");
	}
	if(signal(SIGUSR2,atencionSIGUSR2)==SIG_ERR){
		my_log_error("No pudo cambiarse la señal SIGUSR2");
	}
	if(signal(SIGPOLL,volcarMemoria)==SIG_ERR){
		my_log_error("No pudo cambiarse la señal SIGPOLL");
	}

}

char* decirHolaMundo() {
	return "Hola Mundo";
}

char* getNombre() {
	return "Memoria";
}

int getSocketCPU(int pid) {
	return atoi((char*) dictionary_get(conexiones, getKeyPidCpu(pid)));
}

bool hayQueRegistrarPidCpu(int socket) {
	bool esSocketCPU(void* elemento) {
		return string_equals((char*) elemento, string_itoa(socket));
	}

	char* encontrado = list_find(listaSocketsCPU, esSocketCPU);
	return encontrado != NULL;
}

char* getKeyPidCpu(int pid) {
	return string_from_format("CPU-PID:%d", pid);
}

void registrarPidCpu(int socket, int pid) {

	if(hayQueRegistrarPidCpu(socket)){
		char* keyCPU = getKeyPidCpu(pid);
		dictionary_put(conexiones, keyCPU, string_itoa(socket));
	}
}
