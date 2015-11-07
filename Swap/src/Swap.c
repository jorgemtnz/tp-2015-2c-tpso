#include "Swap.h"

int main(int argc, char *argv[]) {
	l_procesosCargados* proceso;
	proceso = crearProceso();
	l_espacioLibre* espacioLibre;
	espacioLibre = crearEspacioLibre();
	conexiones = dictionary_create();

	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);
	inicializarListas();
	crearArchivo();
	if (hayQueEjecutarTests(argc, argv)) {
		return ejecutarTests();
	}

	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);

	free(configuracion);
	free(logger);
	while (list_size(listaDeProcesosCargados) > 0) {
		proceso = list_get(listaDeProcesosCargados, 0);
		list_remove(listaDeProcesosCargados, 0);
		free(proceso);
	}
	while (list_size(listaDeEspaciosLibres) > 0) {
		espacioLibre = list_get(listaDeEspaciosLibres, 0);
		list_remove(listaDeEspaciosLibres, 0);
		free(espacioLibre);
	}

	free(listaDeEspaciosLibres);
	free(conexiones);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	puts("Swap procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra, logger);
	t_PID* pid_a_enviar;
	pid_a_enviar = crearEstructuraPid();

	switch (tipoNotificacion) {
	case (NEW_CONNECTION): {
		break;
	}
	case (TERMINAL_MESSAGE): {
		break;
	}

	case (MESSAGE): {
		switch (header->tipoMensaje) {
		case (INICIAR_PROC_SWAP): {

			t_iniciar_swap* estructuraIniciar = (t_iniciar_swap*) buffer;
			//printf("el pid recibido %i  y cantidad de pag %i  \n\n", estructuraIniciar->PID, estructuraIniciar->cantidadPaginas);
			t_respuesta_iniciar_o_finalizar* resultado;
			t_iniciar_swap* estructuraAEnviar = crearEstructuraIniciar();
			resultado = crearDevolucionIniciarOFinalizar();
			resultado = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
			estructuraAEnviar->PID = resultado->PID;
			estructuraAEnviar->cantidadPaginas = estructuraIniciar->cantidadPaginas;
			if (resultado->resultado == OK) {

				enviarStruct(socket, RESUL_INICIAR_PROC_OK, estructuraAEnviar);
			} else {
				enviarStruct(socket, RESUL_INICIAR_PROC_ERROR, estructuraAEnviar);
			}

			free(estructuraIniciar);
			free(resultado);
			break;
		}
		case (ESCRIBIR_SWAP): {
			//conviene hacerlo en la misma linea
			t_contenido_pagina* procesoAEscribir = (t_contenido_pagina*) buffer;
			t_devolucion_escribir_o_leer* resultado = crearDevolucionEscribirOLeer();
			t_contenido_pagina* paginaAEnviar=  crearContenidoPagina();
			//warning no se usa variable, entonces comento
//			t_contenido_pagina* paginaEnBlanco = crearContenidoPagina();

			borrarContenidoPagina(procesoAEscribir);
			resultado = escribir(listaDeProcesosCargados, procesoAEscribir);
			paginaAEnviar->PID = resultado->PID;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;

			if (resultado->resultado == OK) {

				enviarStruct(socket, RESUL_ESCRIBIR_OK, paginaAEnviar);
			} else {
				log_info(logger, "FALLO EL ESCRIBIR");
			}
			free(procesoAEscribir);
			free(resultado);
			free(paginaAEnviar);
			break;
		}
		case (LEER_SWAP): {
			t_leerDeProceso* procesoRecibido = (t_leerDeProceso*) buffer;
			t_devolucion_escribir_o_leer* resultado;
			t_contenido_pagina* paginaAEnviar;
			paginaAEnviar = crearContenidoPagina();
			resultado = crearDevolucionEscribirOLeer();
			resultado = leer(procesoRecibido, listaDeProcesosCargados);
			paginaAEnviar->PID = resultado->PID;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;
			if (resultado->resultado == OK) {
				enviarStruct(socket, RESUL_LEER_OK, paginaAEnviar);
			} else {
				log_info(logger, "FALLO EL LEER");
			}
			free(procesoRecibido);
			free(resultado);
			free(paginaAEnviar);
			break;
		}
		case(LEER_SWAP_POR_ESCRIBIR):{
			t_leerDeProcesoPorEscribir* procesoRecibido = (t_leerDeProcesoPorEscribir*) buffer;
					t_devolucion_escribir_o_leer* resultado;
					t_contenido_pagina* paginaAEnviar;
					t_leerDeProceso* procesoALeer;
					procesoALeer = crearLeerDeProceso();
					procesoALeer->PID = procesoRecibido->PID;
					procesoALeer->numeroPaginaFin = procesoRecibido->numeroPaginaFin;
					procesoALeer->numeroPaginaInicio = procesoRecibido->numeroPaginaInicio;
					paginaAEnviar = crearContenidoPagina();
					resultado = crearDevolucionEscribirOLeer();
					resultado = leer(procesoALeer, listaDeProcesosCargados);
					paginaAEnviar->PID = resultado->PID;
					paginaAEnviar->contenido = procesoRecibido->textoAEscribir;
					paginaAEnviar->numeroPagina = resultado->numeroPagina;
					if (resultado->resultado == OK) {
						enviarStruct(socket, RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR, paginaAEnviar);
					} else {
						log_info(logger, "FALLO EL LEER");
					}
					free(procesoRecibido);
					free(resultado);
					free(paginaAEnviar);
			break;
		}
		case (FIN_PROCESO_SWAP): {
			t_PID* estructuraFinalizar = (t_PID*) buffer;
			t_respuesta_iniciar_o_finalizar* resultado;
			resultado = crearDevolucionIniciarOFinalizar();
			resultado = finalizar(estructuraFinalizar->PID, listaDeProcesosCargados, listaDeEspaciosLibres);
			pid_a_enviar->PID = resultado->PID;
			if (resultado->resultado == OK) {
				enviarStruct(socket, RESUL_FIN_OK, pid_a_enviar);
			} else {
				log_info(logger, "FALLO EL FINALIZAR");
			}
			free(estructuraFinalizar);
			free(resultado);
			break;
		}
		case (SOBREESCRIBIR_SWAP): {
			t_contenido_pagina* procesoAEscribir = (t_contenido_pagina*) buffer;
			t_devolucion_escribir_o_leer* resultado;
			t_contenido_pagina* paginaAEnviar;
			t_contenido_pagina* paginaEnBlanco;
			paginaEnBlanco = crearContenidoPagina();
			paginaAEnviar = crearContenidoPagina();
			resultado = crearDevolucionEscribirOLeer();

			borrarContenidoPagina(procesoAEscribir);

			//ESCRIBO EL CONTENIDO NUEVO EN LA PAGINA

			resultado = escribir(listaDeProcesosCargados, procesoAEscribir);
			paginaAEnviar->PID = resultado->PID;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;

			if (resultado->resultado == OK) {

				enviarStruct(socket, RESUL_SOBREESCRIBIR_OK, paginaAEnviar);
			} else {
				log_info(logger, "FALLO EL SOBREESCRIBIR");
			}
			free(procesoAEscribir);
			free(resultado);
			free(paginaAEnviar);
			free(paginaEnBlanco);
			break;
		}
		default: printf("mensaje no valido");

		}
		break;
	}
	case (HANG_UP): {
		break;
	}
	}

	free(pid_a_enviar);
	return 0;
}

char* decirHolaMundo() {
	return "Hola Mundo";
}

char* getNombre() {
	return "Swap";
}
