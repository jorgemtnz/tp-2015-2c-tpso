#include "Swap.h"

int main(int argc, char *argv[]) {
	procesarParametros(argc, argv);
	l_procesosCargados* proceso = crearProceso();
	l_espacioLibre* espacioLibre = crearEspacioLibre();
	t_contador* contador = crearContador();
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
	while (list_size(contadorLecturasYEscrituras) > 0) {
		contador = list_get(contadorLecturasYEscrituras, 0);
		list_remove(contadorLecturasYEscrituras, 0);
		free(contador);
	}

	//free(listaDeEspaciosLibres);
	free(conexiones);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	//puts("Swap procesar mensajes");
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
			t_contador* contador = crearContador();
			resultado = crearDevolucionIniciarOFinalizar();



			resultado = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
			estructuraAEnviar->PID = resultado->PID;
			estructuraAEnviar->cantidadPaginas = estructuraIniciar->cantidadPaginas;
			if (resultado->resultado == OK) {

				enviarStruct(socket, RESUL_INICIAR_PROC_OK, estructuraAEnviar);
				contador->PID = estructuraIniciar->PID;
				list_add(contadorLecturasYEscrituras, contador);
				printf("\n");
				printf("Estado de la partición swap: \n\n");
				imprimirListaProcesos();
				imprimirListaDeLibres();
				printf("*------------------------------------------*\n");

			} else {
				enviarStruct(socket, RESUL_INICIAR_PROC_ERROR, estructuraAEnviar);
			}

			free(estructuraIniciar);
			free(resultado);
			break;
		}
		case (ESCRIBIR_SWAP): {
			//conviene hacerlo en la misma linea

			t_sobreescribir_swap* procesoAEscribir = (t_sobreescribir_swap*) buffer;
			t_devolucion_escribir_o_leer* resultado = crearDevolucionEscribirOLeer();
			t_contenido_pagina* paginaAEnviar = crearContenidoPagina();
			t_contador* contador = crearContador();
			int a, bit;
			//warning no se usa variable, entonces comento
//			t_contenido_pagina* paginaEnBlanco = crearContenidoPagina();

			borrarContenidoPagina(procesoAEscribir);
			bit = 1;
			resultado = escribir(listaDeProcesosCargados, procesoAEscribir, bit);
			paginaAEnviar->PID = procesoAEscribir->PIDAResponderleAMemoria;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;

			if (resultado->resultado == OK) {
				if (procesoAEscribir->PIDAResponderleAMemoria != 236) {
					enviarStruct(socket, RESUL_ESCRIBIR_OK, paginaAEnviar);
				}
				for (a = 0; a < list_size(contadorLecturasYEscrituras); a++) {
					contador = list_get(contadorLecturasYEscrituras, a);
					if (contador->PID == procesoAEscribir->PIDAReemplazar) {
						contador->escrituras++;
						list_replace(contadorLecturasYEscrituras, a, contador);
						a = list_size(contadorLecturasYEscrituras) + 1;
					}
				}
			} else {
				log_info(logger, "FALLO EL ESCRIBIR \n\n");
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
			t_contador* contador = crearContador();
			paginaAEnviar = crearContenidoPagina();
			int a;
			resultado = crearDevolucionEscribirOLeer();
			resultado = leer(procesoRecibido, listaDeProcesosCargados);

			paginaAEnviar->PID = resultado->PID;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;
			if (resultado->resultado == OK) {

				enviarStruct(socket, RESUL_LEER_OK, paginaAEnviar);

				for (a = 0; a < list_size(contadorLecturasYEscrituras); a++) {
					contador = list_get(contadorLecturasYEscrituras, a);
					if (procesoRecibido->PID == contador->PID) {
						contador->lecturas++;
						list_replace(contadorLecturasYEscrituras, procesoRecibido->PID, contador);
						a = list_size(contadorLecturasYEscrituras) + 1;
					}
				}

			} else {
				log_info(logger, "FALLO EL LEER \n\n");
			}
			free(procesoRecibido);
			free(resultado);
			free(paginaAEnviar);
			break;
		}
		case (LEER_SWAP_POR_ESCRIBIR): {
			t_leerDeProcesoPorEscribir* procesoRecibido = (t_leerDeProcesoPorEscribir*) buffer;
			t_devolucion_escribir_o_leer* resultado;
			t_contenido_pagina* paginaAEnviar;
			t_leerDeProceso* procesoALeer;
			t_contador* contador = crearContador();
			int a;
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
				for (a = 0; a < list_size(contadorLecturasYEscrituras); a++) {
					contador = list_get(contadorLecturasYEscrituras, a);
					if (procesoRecibido->PID == contador->PID) {
						contador->lecturas++;
						list_replace(contadorLecturasYEscrituras, procesoRecibido->PID, contador);
						a = list_size(contadorLecturasYEscrituras) + 1;
					}
				}
			} else {
				log_info(logger, "FALLO EL LEER \n\n");
			}
			//	free(procesoRecibido);
			//free(resultado);
			//free(paginaAEnviar);
			break;
		}
		case (FIN_PROCESO_SWAP): {
			t_PID* estructuraFinalizar = (t_PID*) buffer;
			t_respuesta_iniciar_o_finalizar* resultado;
			t_contador *contador = crearContador();
			int a;
			resultado = crearDevolucionIniciarOFinalizar();

			resultado = finalizar(estructuraFinalizar->PID, listaDeProcesosCargados, listaDeEspaciosLibres);

			pid_a_enviar->PID = estructuraFinalizar->PID;
			if (resultado->resultado == OK) {
				enviarStruct(socket, RESUL_FIN_OK, pid_a_enviar);
				for (a = 0; a < list_size(contadorLecturasYEscrituras); a++) {
					contador = list_get(contadorLecturasYEscrituras, a);
					if (estructuraFinalizar->PID == contador->PID) {
						printf("FIN DEL MPROC %i \nCANTIDAD DE LECTURAS REALIZADAS: %i \nCANTIDAD DE ESCRITURAS REALIZADAS: %i\n", estructuraFinalizar->PID,
								contador->lecturas, contador->escrituras);
						printf("*------------------------------------------*\n");
						list_remove(contadorLecturasYEscrituras, a);
						a = list_size(contadorLecturasYEscrituras) + 1;
					}
				}

			} else {
				log_info(logger, "FALLO EL FINALIZAR \n\n");
			}
			//free(estructuraFinalizar);
			//free(resultado);
			break;
		}
		case (SOBREESCRIBIR_SWAP): {

			t_sobreescribir_swap* procesoAEscribir = (t_sobreescribir_swap*) buffer;
			t_devolucion_escribir_o_leer* resultado;
			t_contenido_pagina* paginaAEnviar;
			t_contenido_pagina* paginaEnBlanco;
			t_contador* contador = crearContador();
			paginaEnBlanco = crearContenidoPagina();
			paginaAEnviar = crearContenidoPagina();
			resultado = crearDevolucionEscribirOLeer();
			int a;
			int bit = 1;

			borrarContenidoPagina(procesoAEscribir);

			//ESCRIBO EL CONTENIDO NUEVO EN LA PAGINA

			resultado = escribir(listaDeProcesosCargados, procesoAEscribir, bit);
			paginaAEnviar->PID = procesoAEscribir->PIDAResponderleAMemoria;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;

			if (resultado->resultado == OK) {
				if (procesoAEscribir->PIDAResponderleAMemoria != 236) {
					enviarStruct(socket, RESUL_SOBREESCRIBIR_OK, paginaAEnviar);
				}
				for (a = 0; a < list_size(contadorLecturasYEscrituras); a++) {
					contador = list_get(contadorLecturasYEscrituras, a);
					if (contador->PID == procesoAEscribir->PIDAReemplazar) {
						contador->escrituras++;
						list_replace(contadorLecturasYEscrituras, a, contador);
						a = list_size(contadorLecturasYEscrituras) + 1;
					}
				}
			} else {
				log_info(logger, "FALLO EL SOBREESCRIBIR \n\n");
			}
			free(procesoAEscribir);
			free(resultado);
			free(paginaAEnviar);
			free(paginaEnBlanco);
			break;
		}
		default:
			printf("Mensaje no válido");

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
