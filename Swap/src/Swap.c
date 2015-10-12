#include "Swap.h"

t_list* listaDeProcesosCargados;
t_list* listaDeEspaciosLibres;
t_dictionary* conexiones;
int main(int argc, char *argv[]) {



	conexiones = dictionary_create();

	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	crearArchivo();
	if (hayQueEjecutarTests(argc, argv)) {
			return ejecutarTests();
		}

	//empieza prueba(NO BORRAR)
/*
	int a;
	l_procesosCargados* proceso;
	l_espacioLibre* espacioLibre;
	l_espacioLibre* espacioLibre2;
	l_espacioLibre* espacioLibre3;
	t_iniciar_swap* estructuraIniciar;
	t_iniciar_swap* estructuraIniciar3;
	t_iniciar_swap* estructuraIniciar4;
	t_iniciar_swap* estructuraIniciar5;
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar3 = crearEstructuraIniciar();
	estructuraIniciar4 = crearEstructuraIniciar();
	estructuraIniciar5 = crearEstructuraIniciar();
	proceso = crearProceso();
	espacioLibre = crearEspacioLibre();
	espacioLibre2 = crearEspacioLibre();
	espacioLibre3 = crearEspacioLibre();

	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();
	estructuraIniciar->PID = 2;
	estructuraIniciar3->PID = 3;
	estructuraIniciar4->PID = 5;
	estructuraIniciar5->PID = 6;
	estructuraIniciar->cantidadPaginas = 8;
	estructuraIniciar3->cantidadPaginas = 5;
	estructuraIniciar4->cantidadPaginas = 15;
	estructuraIniciar5->cantidadPaginas = 3;
	t_contenido_pagina* procesoAEscribir;
	procesoAEscribir = crearEscribirEnProceso();
	t_leerDeProceso *procesoRecibido;
	procesoRecibido = crearLeerDeProceso();
*/
	t_iniciar_swap* estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
	t_list* listaDeEspaciosLibres;
	t_list* listaDeProcesosCargados;
	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();
	estructuraIniciar->PID = 0;
	estructuraIniciar->cantidadPaginas = 6;

	iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar2;
	estructuraIniciar2 = crearEstructuraIniciar();
	estructuraIniciar2->PID = 1;
	estructuraIniciar2->cantidadPaginas = 10;

	iniciar(estructuraIniciar2, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar3;
	estructuraIniciar3 = crearEstructuraIniciar();
	estructuraIniciar3->PID = 18;
	estructuraIniciar3->cantidadPaginas = 6;

	iniciar(estructuraIniciar3, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar5;
	estructuraIniciar5 = crearEstructuraIniciar();
	estructuraIniciar5->PID = 8;
	estructuraIniciar5->cantidadPaginas = 28;

	iniciar(estructuraIniciar5, listaDeEspaciosLibres, listaDeProcesosCargados);

	finalizar(0, listaDeProcesosCargados, listaDeEspaciosLibres);

	finalizar(18, listaDeProcesosCargados, listaDeEspaciosLibres);


	t_iniciar_swap* estructuraIniciar4;
	estructuraIniciar4 = crearEstructuraIniciar();
	estructuraIniciar4->PID = 5;
	estructuraIniciar4->cantidadPaginas = 10;

	iniciar(estructuraIniciar4, listaDeEspaciosLibres, listaDeProcesosCargados);

	int a;
		l_procesosCargados* proceso;
		l_espacioLibre* espacioLibre;
		proceso = crearProceso();
		espacioLibre = crearEspacioLibre();

	//escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);
	/*
	 prueba para borrar el contenido mapeado cuando se va un proceso
	 iniciar(cantidadPaginas, listaDeEspaciosLibres, listaDeProcesosCargados, pid);
	 procesoAEscribir->PID = pid;
	 procesoAEscribir->contenido = "PASO USTED POR MI CASA , POR SU CASA YO PASE";
	 procesoAEscribir->numeroPagina = 1;

	 escribir(listaDeProcesosCargados, procesoAEscribir);
	 finalizar(pid, listaDeProcesosCargados, listaDeEspaciosLibres);
	 procesoRecibido->numeroPaginaFin = 2;
	 procesoRecibido->numeroPaginaInicio = 1;
	 procesoRecibido->PID = pid;


	 char* datosLeidos4 = leer(procesoRecibido, listaDeProcesosCargados);

	 printf("los datos leidos : %s\n", datosLeidos4);

	 */
	/*
	 iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	 iniciar(estructuraIniciar3, listaDeEspaciosLibres, listaDeProcesosCargados);

	 iniciar(estructuraIniciar4, listaDeEspaciosLibres, listaDeProcesosCargados);

	 finalizar(&estructuraIniciar3->PID, listaDeProcesosCargados, listaDeEspaciosLibres);

	 iniciar(estructuraIniciar5, listaDeEspaciosLibres, listaDeProcesosCargados);

	 finalizar(&estructuraIniciar5->PID, listaDeProcesosCargados, listaDeEspaciosLibres);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "HOLA DON PEPITO HOLA DON JOSE";
	 procesoAEscribir->numeroPagina = 5;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "PASO USTED POR MI CASA , POR SU CASA YO PASE";
	 procesoAEscribir->numeroPagina = 6;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "los wachiturros";
	 procesoAEscribir->numeroPagina = 10;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "que tp del ortooo";
	 procesoAEscribir->numeroPagina = 9;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 finalizar(&estructuraIniciar4->PID, listaDeProcesosCargados, listaDeEspaciosLibres);

	 iniciar(estructuraIniciar4, listaDeEspaciosLibres, listaDeProcesosCargados);

	 procesoRecibido->numeroPaginaFin = 10;
	 procesoRecibido->numeroPaginaInicio = 10;
	 procesoRecibido->PID = estructuraIniciar4->PID;

	 char* datosLeidos5 = leer(procesoRecibido, listaDeProcesosCargados);

	 printf("los datos leidos5 : %s\n", datosLeidos5);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "my name is juan";
	 procesoAEscribir->numeroPagina = 11;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoRecibido->PID = estructuraIniciar4->PID;
	 procesoRecibido->numeroPaginaFin = 12;
	 procesoRecibido->numeroPaginaInicio = 11;

	 char* datosLeidos = leer(procesoRecibido, listaDeProcesosCargados);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "lalalalallalalalallaa";
	 procesoAEscribir->numeroPagina = 6;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "nose que poneer";
	 procesoAEscribir->numeroPagina = 7;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoAEscribir->PID = estructuraIniciar4->PID;
	 procesoAEscribir->contenido = "pepepepeppepee";
	 procesoAEscribir->numeroPagina = 8;

	 escribir(listaDeProcesosCargados, procesoAEscribir);

	 procesoRecibido->numeroPaginaFin = 9;
	 procesoRecibido->numeroPaginaInicio = 6;

	 char* datosLeidos2 = leer(procesoRecibido, listaDeProcesosCargados);

	 procesoRecibido->numeroPaginaFin = 6;
	 procesoRecibido->numeroPaginaInicio = 6;

	 char* datosLeidos3 = leer(procesoRecibido, listaDeProcesosCargados);

	 procesoRecibido->numeroPaginaFin = 11;
	 procesoRecibido->numeroPaginaInicio = 10;

	 char* datosLeidos4 = leer(procesoRecibido, listaDeProcesosCargados);

	 printf("los datos leidos : %s\n", datosLeidos);
	 printf("los datos leidos2 : %s\n", datosLeidos2);
	 printf("los datos leidos3 : %s\n", datosLeidos3);
	 printf("los datos leidos4 : %s \n", datosLeidos4);
*/
	printf("\nEMPIEZA LISTA DE PROCESOS\n\n");
	for (a = 0; a < list_size(listaDeProcesosCargados); a++) {
		proceso = list_get(listaDeProcesosCargados, a);
		printf("\n\n");
		printf("el pid :  %i\n", proceso->PID);
		printf("la ubicacion es : %i\n", proceso->ubicacion);
		printf("cantidad paginas : %i \n", proceso->cantPagsUso);

	}
	for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
		espacioLibre = list_get(listaDeEspaciosLibres, a);
		printf("\nempieza espacio libre\n\n\n");
		printf("ubicacion espacio libre : %i \n", espacioLibre->ubicacion);
		printf("cant pag libres :  %i\n", espacioLibre->cantPagsLibres);

	}

	//termina prueba

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
			printf("el pid recibido %i  y cantidad de pag %i \n\n", estructuraIniciar->PID, estructuraIniciar->cantidadPaginas);
			t_respuesta_iniciar_o_finalizar* resultado;
			resultado = crearDevolucionIniciarOFinalizar();
			resultado = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
			pid_a_enviar->PID = resultado->PID;
			if (resultado == OK) {

				enviarStruct(socket, RESUL_INICIAR_PROC_OK, pid_a_enviar);
			} else {
				enviarStruct(socket, RESUL_INICIAR_PROC_ERROR, pid_a_enviar);
			}

			//EMPIEZA PRUEBA
			int a;
			l_procesosCargados* proceso;
			l_espacioLibre* espacioLibre;

			espacioLibre = crearEspacioLibre();

			printf("\nEMPIEZA LISTA DE PROCESOS\n\n");

			for (a = 0; a < list_size(listaDeProcesosCargados); a++) {
				proceso = list_get(listaDeProcesosCargados, a);
				printf("\n\n");
				printf("el pid :  %i\n", proceso->PID);
				printf("la ubicacion es : %i\n", proceso->ubicacion);
				printf("cantidad paginas : %i \n", proceso->cantPagsUso);

			}
			printf("lista %i \n", list_size(listaDeEspaciosLibres));
			for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
				espacioLibre = list_get(listaDeEspaciosLibres, a);

				printf("\nempieza espacio libre\n\n\n");
				printf("ubicacion espacio libre : %i \n", espacioLibre->ubicacion);
				printf("cant pag libres :  %i\n", espacioLibre->cantPagsLibres);

			}
			//TERMINA PRUEBA
			break;
		}
		case (ESCRIBIR_SWAP): {
			t_contenido_pagina* procesoAEscribir = (t_contenido_pagina*) buffer;
			t_devolucion_escribir_o_leer* resultado;
			t_contenido_pagina* paginaAEnviar;
			paginaAEnviar = crearContenidoPagina();
			resultado = crearDevolucionEscribirOLeer();
			resultado = escribir(listaDeProcesosCargados, procesoAEscribir);
			paginaAEnviar->PID = resultado->PID;
			paginaAEnviar->contenido = resultado->contenido;
			paginaAEnviar->numeroPagina = resultado->numeroPagina;

			if (resultado->resultado == OK) {

				enviarStruct(socket, RESUL_ESCRIBIR_OK, paginaAEnviar);
			} else {
				log_info(logger, "FALLO EL ESCRIBIR");
			}
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
			if(resultado->resultado == OK){
				enviarStruct(socket, RESUL_LEER_OK, paginaAEnviar);
			}else{
				log_info(logger, "FALLO EL LEER");
			}
			break;
		}
		case (FIN_PROCESO_SWAP): {
			t_PID* estructuraFinalizar = (t_PID*) buffer;
			t_respuesta_iniciar_o_finalizar* resultado;
			resultado = crearDevolucionIniciarOFinalizar();
			resultado = finalizar(estructuraFinalizar->PID, listaDeProcesosCargados, listaDeEspaciosLibres);
			pid_a_enviar->PID = resultado->PID;
			if(resultado->resultado == OK){
				enviarStruct(socket, RESUL_FIN_OK, pid_a_enviar);
			}else{
				log_info(logger , "FALLO EL FINALIZAR");
			}

			break;
		}

		}
		break;
	}
	case (HANG_UP): {
		break;
	}
	}
	return 0;
}

char* decirHolaMundo() {
	return "Hola Mundo";
}
