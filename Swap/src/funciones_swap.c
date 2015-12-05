#include "Swap.h"
//escribir las funciones aqui

void inicializarListas() {

	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();
	contadorLecturasYEscrituras = list_create();
}
void leerArchivoDeConfiguracion(int argc, char *argv[]) {

	char* logMsg = NULL;

	if (argc < 2) {
		logMsg =
				string_from_format(
						"Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./Swap /home/utnso/tp-2015-2c-tpso/Swap/config_swap.cfg\n");
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	}

	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
	int result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		logMsg = string_from_format("Archivo de configuracion no encontrado. Parametro especificado: %s\n", nombreArchivoConfig);
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	} else {
		t_config* archivoConfig;
		archivoConfig = config_create(nombreArchivoConfig);
		configuracion = malloc(sizeof(t_configuracion));
		configuracion->puertoEscucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
		configuracion->nombreSwap = strdup(config_get_string_value(archivoConfig, "NOMBRE_SWAP"));
		configuracion->cantidadPaginas = config_get_int_value(archivoConfig, "CANTIDAD_PAGINAS");
		configuracion->tamanioPagina = config_get_int_value(archivoConfig, "TAMANIO_PAGINA");
		configuracion->retardo_compactacion = config_get_int_value(archivoConfig, "RETARDO_COMPACTACION");
		configuracion->retardo_swap = config_get_int_value(archivoConfig, "RETARDO_SWAP");

		logMsg = string_from_format("Archivo de configuracion leido correctamente\n");
		puts(logMsg);
		log_info(logger, logMsg);

		config_destroy(archivoConfig);
	}

	free(logMsg);

}

void crearArchivo() {

	char *pathArchivo = string_new();
	string_append(&pathArchivo, "/home/utnso/tp-2015-2c-tpso/Swap/");
	string_append(&pathArchivo, configuracion->nombreSwap);

	abrirOCrearArchivoLecturaEscritura(pathArchivo, logger);

	int tamanioArchivo = configuracion->cantidadPaginas * configuracion->tamanioPagina;
	char* tamanioArchivoString = string_new();
	tamanioArchivoString = string_itoa(tamanioArchivo);
	crearArchivoMmap(pathArchivo, tamanioArchivoString);

	int fdEspacioDatos = abrirArchivoEspacioDatos(pathArchivo, logger);

	int offset = 0;

	espacioDatos = crearEspacioDeDatos(fdEspacioDatos, tamanioArchivo, logger);

	char* espacioVacio = string_new();
	espacioVacio = string_repeat('\0', configuracion->tamanioPagina);
	escribirEnEspacioDatos(espacioDatos, espacioVacio, offset, configuracion->tamanioPagina);
	free(espacioVacio);
	free(pathArchivo);
	free(tamanioArchivoString);

}

t_respuesta_iniciar_o_finalizar* iniciar(t_iniciar_swap* estructuraIniciar, t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados) {

	l_procesosCargados* procesoAInsertarEnLista;
	l_espacioLibre* espacioLibre;
	l_espacioLibre* espacioLibreAInsertar;
	//según estudie conviene hacerlo en la misma linea
	t_respuesta_iniciar_o_finalizar* estructura = crearDevolucionIniciarOFinalizar();

	procesoAInsertarEnLista = crearProceso();
	espacioLibre = crearEspacioLibre();
	espacioLibreAInsertar = crearEspacioLibre();
	int a, paginasLibresRestantes;

	int cantidadDePagLibres = 0;
	espacioLibre->ubicacion = 0;
	espacioLibre->cantPagsLibres = configuracion->cantidadPaginas;
	if (list_size(listaDeEspaciosLibres) != 0) {
		for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
			espacioLibre = list_get(listaDeEspaciosLibres, a);

			cantidadDePagLibres += espacioLibre->cantPagsLibres;

		}

	} else { //SI ESTAN TODAS LAS PAG LIBRES
		cantidadDePagLibres = configuracion->cantidadPaginas;
		espacioLibre->ubicacion = 0;
		espacioLibre->cantPagsLibres = configuracion->cantidadPaginas;
		list_add(listaDeEspaciosLibres, espacioLibre);
	}

	if (cantidadDePagLibres >= estructuraIniciar->cantidadPaginas) {

		acomodarEspaciosLibres(listaDeEspaciosLibres); // POR SI QUEDARON DON BLOQUES LIBRES CONTINUOS

		for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
			espacioLibre = list_get(listaDeEspaciosLibres, a);

			if (espacioLibre->cantPagsLibres >= estructuraIniciar->cantidadPaginas) { // SI ENCUENTRO UN ESPACIO EN EL QE ENTRE LO METO y actualizo la lista de espacios libres

				procesoAInsertarEnLista->PID = estructuraIniciar->PID;
				procesoAInsertarEnLista->ubicacion = espacioLibre->ubicacion;
				procesoAInsertarEnLista->cantPagsUso = estructuraIniciar->cantidadPaginas;
				list_add(listaDeProcesosCargados, procesoAInsertarEnLista);
				paginasLibresRestantes = espacioLibre->cantPagsLibres - estructuraIniciar->cantidadPaginas;
				list_remove(listaDeEspaciosLibres, a);
				espacioLibreAInsertar->cantPagsLibres = paginasLibresRestantes;
				espacioLibreAInsertar->ubicacion = espacioLibre->ubicacion + estructuraIniciar->cantidadPaginas;
				list_add_in_index(listaDeEspaciosLibres, a, espacioLibreAInsertar);
				a = list_size(listaDeEspaciosLibres) + 1; //SI YA ENCONTRE UN ESPACIO NO BUSCO MAS
			} else {

				compactarMemoria(listaDeEspaciosLibres, listaDeProcesosCargados);

				procesoAInsertarEnLista->PID = estructuraIniciar->PID;
				espacioLibre = list_get(listaDeEspaciosLibres, 0);
				procesoAInsertarEnLista->ubicacion = espacioLibre->ubicacion;
				procesoAInsertarEnLista->cantPagsUso = estructuraIniciar->cantidadPaginas;
				list_add(listaDeProcesosCargados, procesoAInsertarEnLista);
				paginasLibresRestantes = espacioLibre->cantPagsLibres - estructuraIniciar->cantidadPaginas;
				list_remove(listaDeEspaciosLibres, 0);
				espacioLibreAInsertar->cantPagsLibres = paginasLibresRestantes;
				espacioLibreAInsertar->ubicacion = espacioLibre->ubicacion + estructuraIniciar->cantidadPaginas;
				list_add_in_index(listaDeEspaciosLibres, 0, espacioLibreAInsertar);
				a = list_size(listaDeEspaciosLibres) + 1;
			}

			//LOGUEO
			int byteInicial, tamanioEnBytes;
			if (procesoAInsertarEnLista->ubicacion == 0) {
				byteInicial = 0;
			} else {
				byteInicial = procesoAInsertarEnLista->ubicacion * configuracion->tamanioPagina;
			}
			tamanioEnBytes = procesoAInsertarEnLista->cantPagsUso * configuracion->tamanioPagina;
			char* contenidoLogger = string_new();
			string_append(&contenidoLogger,
					string_from_format("\nProceso mProc asignado, el PID es: %i, el byte inicial es: %i, el tamanio en bytes es: %i \n",
							procesoAInsertarEnLista->PID, byteInicial, tamanioEnBytes));
			log_info(logger, contenidoLogger);

			estructura->PID = procesoAInsertarEnLista->PID;
			estructura->resultado = OK;

			return estructura;
			free(procesoAInsertarEnLista);
			free(espacioLibre);
			free(espacioLibreAInsertar);
			free(estructura);

		}
	} else {

		char* contenidoLogger = string_new();
		string_append(&contenidoLogger, string_from_format("\n Proceso mProc rechazado por falta de espacio (PID: %i) \n", estructuraIniciar->PID));
		log_info(logger, contenidoLogger);

		estructura->PID = estructuraIniciar->PID;
		estructura->resultado = ERROR;

		return estructura;
		free(procesoAInsertarEnLista);
		free(espacioLibre);
		free(espacioLibreAInsertar);
		free(estructura);
	}
	//warninig: no hay retorno, entonces se agrega la estructura
	return estructura;
}

t_devolucion_escribir_o_leer* escribir(t_list* listaDeProcesosCargados, t_sobreescribir_swap* procesoAEscribir, int bit) {

	uretardo(configuracion->retardo_swap);
	l_procesosCargados* unProceso;
	unProceso = crearProceso();
	t_devolucion_escribir_o_leer* respuestaDeEscribir;
	respuestaDeEscribir = crearDevolucionEscribirOLeer();
	int nuevaPagina;
	int a, ubicacion;
	for (a = 0; a <= list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
		unProceso = list_get(listaDeProcesosCargados, a);
		if (unProceso->PID == procesoAEscribir->PIDAReemplazar) {

			ubicacion = unProceso->ubicacion;
			a = list_size(listaDeProcesosCargados) + 1; //salgo del for
		}
	}

	//LOGUEO
	int byteInicial, tamanioEnBytes;
	char* contenidoLogger = string_new();
	if (bit == 1) {

		if ((ubicacion + procesoAEscribir->numeroPagina) == 0) {
			byteInicial = 0;
		} else {
			byteInicial = (ubicacion + procesoAEscribir->numeroPagina) * configuracion->tamanioPagina;
		}
		tamanioEnBytes = configuracion->tamanioPagina; //SIEMPRE SE ESCRIBE DE A UNA PAGINA

		string_append(&contenidoLogger,
				string_from_format("\nEscritura solicitada, el PID es: %i, el byte inicial es: %i, el tamanio en bytes es: %i, el contenido es: %s \n",
						procesoAEscribir->PIDAReemplazar, byteInicial, tamanioEnBytes, procesoAEscribir->contenido));
		log_info(logger, contenidoLogger);
	}

	if (string_length(procesoAEscribir->contenido) == 0) {
		nuevaPagina = (configuracion->tamanioPagina) * (ubicacion + procesoAEscribir->numeroPagina);
		if (nuevaPagina != 0) {
			escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, nuevaPagina, configuracion->tamanioPagina);
		} else {
			escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, configuracion->tamanioPagina, configuracion->tamanioPagina);
		}
	} else {
		int longitud = string_length(procesoAEscribir->contenido);
		nuevaPagina = (configuracion->tamanioPagina) * (ubicacion + procesoAEscribir->numeroPagina);
		if (nuevaPagina != 0) {
			escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, nuevaPagina, longitud);
		} else {
			escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, configuracion->tamanioPagina, longitud);
		}
	}

	respuestaDeEscribir->PID = unProceso->PID;
	respuestaDeEscribir->numeroPagina = procesoAEscribir->numeroPagina;
	respuestaDeEscribir->contenido = procesoAEscribir->contenido;
	respuestaDeEscribir->resultado = OK;
	if (bit == 1) {
		string_append(&contenidoLogger,
				string_from_format("Finalizo el escribir correctamente el PID es: %i, el byte inicial es: %i, el tamanio en bytes es: %i, el contenido es: %s\n",
						procesoAEscribir->PIDAReemplazar, byteInicial, tamanioEnBytes, procesoAEscribir->contenido));
		log_info(logger, contenidoLogger);
	}
	return respuestaDeEscribir;
	//free(unProceso);
	//free(respuestaDeEscribir);

}

t_devolucion_escribir_o_leer* leer(t_leerDeProceso *procesoRecibido, t_list* listaDeProcesosCargados) {
	uretardo(configuracion->retardo_swap);
	int a, x;
	char* datosLeidos = string_new();
	char* datosLeidosFinal = string_new();
	l_procesosCargados* unProceso;
	l_procesosCargados* procesoAleer;
	unProceso = crearProceso();
	procesoAleer = crearProceso();
	t_devolucion_escribir_o_leer* respuestaDeLeer;
	respuestaDeLeer = crearDevolucionEscribirOLeer();
	for (a = 0; a < list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
		unProceso = list_get(listaDeProcesosCargados, a);
		if (unProceso->PID == procesoRecibido->PID) {
			procesoAleer->PID = unProceso->PID;
			procesoAleer->cantPagsUso = unProceso->cantPagsUso;
			procesoAleer->ubicacion = unProceso->ubicacion;
			a = list_size(listaDeProcesosCargados) + 1;
		}
	}

	//LOGUEO
	int byteInicial, tamanioEnBytes;
	if ((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio) == 0) {
		byteInicial = 0;
	} else {
		byteInicial = (procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio) * configuracion->tamanioPagina;
	}
	if ((procesoRecibido->numeroPaginaFin - procesoRecibido->numeroPaginaInicio) == 0) {
		tamanioEnBytes = configuracion->tamanioPagina;
	} else {
		tamanioEnBytes = (procesoRecibido->numeroPaginaFin - procesoRecibido->numeroPaginaInicio) * configuracion->tamanioPagina;

	}
	char* contenidoLogger = string_new();
	string_append(&contenidoLogger,
			string_from_format("\nLectura solicitada, el PID es: %i, el byte inicial es: %i, el tamanio en bytes es: %i \n", procesoRecibido->PID, byteInicial,
					tamanioEnBytes));
	log_info(logger, contenidoLogger);

	if (procesoRecibido->numeroPaginaFin - procesoRecibido->numeroPaginaInicio != 0) {
		for (x = 0; x <= procesoRecibido->numeroPaginaFin - procesoRecibido->numeroPaginaInicio; x++) {
			if ((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio + x) != 0) {
				datosLeidos = leerEspacioDatos(espacioDatos,
						((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio + x) * (configuracion->tamanioPagina)), (configuracion->tamanioPagina));
			} else {
				datosLeidos = leerEspacioDatos(espacioDatos, (configuracion->tamanioPagina), (configuracion->tamanioPagina));
			}
			string_append(&datosLeidosFinal, datosLeidos);
			string_append(&datosLeidosFinal, " ");

		}

	} else {

		if ((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio) != 0) {
			datosLeidosFinal = leerEspacioDatos(espacioDatos,
					((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio) * (configuracion->tamanioPagina)), configuracion->tamanioPagina);
		} else {
			datosLeidosFinal = leerEspacioDatos(espacioDatos, (configuracion->tamanioPagina), configuracion->tamanioPagina);
		}

	}

	respuestaDeLeer->PID = procesoAleer->PID;
	respuestaDeLeer->contenido = datosLeidosFinal;
	respuestaDeLeer->numeroPagina = procesoRecibido->numeroPaginaInicio;
	//enviarResultadoLeerOK(socket, respuestaDeLeer);
	respuestaDeLeer->resultado = OK;

	string_append(&contenidoLogger,
			string_from_format("Finalizo leer correctamente , el PID es: %i, el byte inicial es: %i, el tamanio en bytes es: %i , el contenido es: %s \n",
					procesoRecibido->PID, byteInicial, tamanioEnBytes, respuestaDeLeer->contenido));
	log_info(logger, contenidoLogger);

	return respuestaDeLeer;

//	free(unProceso);
//	free(respuestaDeLeer);
//	free(procesoAleer);

}

t_respuesta_iniciar_o_finalizar* finalizar(uint8_t pid, t_list* listaDeProcesosCargados, t_list* listaDeEspaciosLibres) {
	int a, b;
	l_procesosCargados* unProceso;
	l_espacioLibre* espacioLibre;
	t_sobreescribir_swap* procesoAEscribir;
	procesoAEscribir = crearEstructuraReemplazar();
	espacioLibre = crearEspacioLibre();
	unProceso = crearProceso();
	t_respuesta_iniciar_o_finalizar* respuestaDeFinalizar;
	respuestaDeFinalizar = crearDevolucionIniciarOFinalizar();

	for (a = 0; a < list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA

		unProceso = list_get(listaDeProcesosCargados, a);

		if (unProceso->PID == (pid)) {

			espacioLibre->ubicacion = unProceso->ubicacion;

			espacioLibre->cantPagsLibres = unProceso->cantPagsUso;

			agregarEnLaPosicionAdecuada(espacioLibre, listaDeEspaciosLibres);

			//BORRAR DEL ESPACIO DE DATOS
			char* espacioVacio = string_new();
			espacioVacio = string_repeat('\0', configuracion->tamanioPagina);
			int bit = 0;
			for (b = 0; b < unProceso->cantPagsUso; b++) {
				procesoAEscribir->PIDAReemplazar = unProceso->PID;
				procesoAEscribir->contenido = espacioVacio;
				procesoAEscribir->numeroPagina = b;

				escribir(listaDeProcesosCargados, procesoAEscribir, bit);

			}

			list_remove(listaDeProcesosCargados, a);
			a = list_size(listaDeProcesosCargados) + 1; //PARA SALIR DEL FOR CUANDO LO ENCONTRE
		}
	}
	respuestaDeFinalizar->PID = unProceso->PID;
	respuestaDeFinalizar->resultado = OK;

	int byteInicial = unProceso->ubicacion * configuracion->tamanioPagina;
	int tamanioEnBytes = unProceso->cantPagsUso * configuracion->tamanioPagina;
	char* contenidoLogger = string_new();
	string_append(&contenidoLogger,
			string_from_format("\nProceso mProc liberado , el PID es: %i, el byte inicial es: %i, el tamanio en bytes liberado es: %i\n", respuestaDeFinalizar->PID,
					byteInicial, tamanioEnBytes));
	log_info(logger, contenidoLogger);

	return respuestaDeFinalizar;
	//free(respuestaDeFinalizar);
	//free(unProceso);
	//free(espacioLibre);
	//free(procesoAEscribir);
}

void acomodarEspaciosLibres(t_list* listaDeEspaciosLibres) {

	l_espacioLibre* espacioA;
	espacioA = crearEspacioLibre();
	l_espacioLibre* espacioB;
	espacioB = crearEspacioLibre();
	l_espacioLibre* espacioC;
	espacioC = crearEspacioLibre();

	int a;

	if (list_size(listaDeEspaciosLibres) > 1) {

		for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {

			espacioA = list_get(listaDeEspaciosLibres, a);

			espacioB = list_get(listaDeEspaciosLibres, a + 1);

			if ((espacioA->ubicacion + espacioA->cantPagsLibres) == (espacioB->ubicacion)) {

				espacioC->ubicacion = espacioA->ubicacion;
				espacioC->cantPagsLibres = (espacioA->cantPagsLibres + espacioB->cantPagsLibres);
				list_remove(listaDeEspaciosLibres, a + 1);
				list_remove(listaDeEspaciosLibres, a);

				list_add_in_index(listaDeEspaciosLibres, a, espacioC);
				a--;
				if ((a + 3) > list_size(listaDeEspaciosLibres)) { //SI HAY MENOS DE DOS ELEMENTOS EN LA LISTA SALGO DEL FOR
					a = list_size(listaDeEspaciosLibres) + 1;
				}
			} else {

				if ((a + 3) > list_size(listaDeEspaciosLibres)) {

					a = list_size(listaDeEspaciosLibres) + 1;
				}
			}
		}
	}

}

void compactarMemoria(t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados) {
	uretardo(configuracion->retardo_compactacion);
	log_info(logger, "\nCompactacion iniciada por fragmentacion externa\n");
	l_procesosCargados* espacioProcAux;
	espacioProcAux = crearProceso();
	l_procesosCargados* espacioProcSig;
	espacioProcSig = crearProceso();
	l_espacioLibre* nuevoEspacioLibre = crearEspacioLibre();
	int a;
	espacioProcAux = list_get(listaDeProcesosCargados, 0);
	espacioProcAux->ubicacion = 0;
	list_replace(listaDeProcesosCargados, 0, espacioProcAux);

	for (a = 1; a < list_size(listaDeProcesosCargados); a++) {

		espacioProcSig = list_get(listaDeProcesosCargados, a);
		espacioProcSig->ubicacion = espacioProcAux->ubicacion + espacioProcAux->cantPagsUso;
		list_replace(listaDeEspaciosLibres, a, espacioProcSig);
		espacioProcAux = list_get(listaDeProcesosCargados, a);
	}

	int ultimoLugarOcupado = espacioProcAux->ubicacion + espacioProcAux->cantPagsUso;
	nuevoEspacioLibre->ubicacion = ultimoLugarOcupado;
	nuevoEspacioLibre->cantPagsLibres = configuracion->cantidadPaginas - ultimoLugarOcupado;
	list_clean(listaDeEspaciosLibres);

	list_add(listaDeEspaciosLibres, nuevoEspacioLibre);

	log_info(logger, "Compactacion finalizada correctamente\n");

}

void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres) {
	l_espacioLibre* espacioA;
	espacioA = crearEspacioLibre();
	l_espacioLibre* espacioB;
	espacioB = crearEspacioLibre();
	int a;
	int tamanio = list_size(listaDeEspaciosLibres);
espacioA = list_get(listaDeEspaciosLibres,tamanio-1);
//POR SI SE AGREGA EL ESPACIO LIBRE DE UBICACION = CONF.TAMANIOPAGINA Y CANTPAGLIBRES 0
if((tamanio > 1) && (espacioA->ubicacion == configuracion->cantidadPaginas)){
	espacioA = list_get(listaDeEspaciosLibres,tamanio-1);
	espacioB = list_get(listaDeEspaciosLibres,tamanio-2);
	if(espacioB->ubicacion + espacioB->cantPagsLibres == espacioA->ubicacion){
		list_remove(listaDeEspaciosLibres,tamanio-1);
		list_remove(listaDeEspaciosLibres,tamanio-2);
		espacioB->cantPagsLibres = espacioB->cantPagsLibres + espacioA->cantPagsLibres;
		list_add(listaDeEspaciosLibres,espacioB);
	}
}
///



	if (list_size(listaDeEspaciosLibres) > 1) {
		for (a = 0; a < list_size(listaDeEspaciosLibres) - 1; a++) {

			espacioA = list_get(listaDeEspaciosLibres, a);
			espacioB = list_get(listaDeEspaciosLibres, a + 1);

			if ((espacioA->ubicacion + espacioA->cantPagsLibres) != (espacioB->ubicacion)) {

				if ((espacioLibre->ubicacion + espacioLibre->cantPagsLibres) == (espacioB->ubicacion)) {

					list_add_in_index(listaDeEspaciosLibres, a + 1, espacioLibre);
					a = list_size(listaDeEspaciosLibres) + 1; //salgo del for cuando lo encuentro

				} else {

					if (espacioLibre->ubicacion < espacioA->ubicacion) {

						list_add_in_index(listaDeEspaciosLibres, a, espacioLibre);
						a = list_size(listaDeEspaciosLibres) + 1; //salgo del for cuando lo encuentro

					}
					if ((espacioLibre->ubicacion > espacioA->ubicacion) && (espacioLibre->ubicacion < espacioB->ubicacion)) {

						list_add_in_index(listaDeEspaciosLibres, a + 1, espacioLibre);
						a = list_size(listaDeEspaciosLibres) + 1; //salgo del for cuando lo encuentro
					}

				}

			}

		}
	} else {

		espacioA = list_get(listaDeEspaciosLibres, 0);

		if (espacioLibre->ubicacion < espacioA->ubicacion) {

			list_add_in_index(listaDeEspaciosLibres, 0, espacioLibre);

		} else {
			list_add_in_index(listaDeEspaciosLibres, 1, espacioLibre);
		}
	}

}

t_devolucion_escribir_o_leer* borrarContenidoPagina(t_sobreescribir_swap* procesoAEscribir) {
	//BORRO EL CONTENIDO VIEJO DE LA PAGINA
	char* espacioVacio = string_new();
	int bit;
	espacioVacio = string_repeat('\0', configuracion->tamanioPagina);
	t_sobreescribir_swap* paginaEnBlanco;
	paginaEnBlanco = crearEstructuraReemplazar();
	t_devolucion_escribir_o_leer* resultado;
	resultado = crearDevolucionEscribirOLeer();
	paginaEnBlanco->PIDAReemplazar = procesoAEscribir->PIDAReemplazar;
	paginaEnBlanco->PIDAResponderleAMemoria = procesoAEscribir->PIDAResponderleAMemoria;
	paginaEnBlanco->contenido = espacioVacio;
	paginaEnBlanco->numeroPagina = procesoAEscribir->numeroPagina;
	bit = 0;
	resultado = escribir(listaDeProcesosCargados, paginaEnBlanco, bit);
	return resultado;
}

void imprimirListaDeLibres(){
	int a;
	l_espacioLibre * espacioLibre = crearEspacioLibre();
	for(a=0; a< list_size(listaDeEspaciosLibres);a++){
		espacioLibre = list_get(listaDeEspaciosLibres,a);
		printf("ESPACIO LIBRE UBICACION: %i // CANTIDADPAGINAS: %i \n",espacioLibre->ubicacion,espacioLibre->cantPagsLibres);
	}
	printf("\n");
}

void imprimirListaProcesos(){
	int a;
	l_procesosCargados * proceso = crearProceso();
	for(a=0; a< list_size(listaDeProcesosCargados);a++){
		proceso = list_get(listaDeProcesosCargados,a);
		printf("PROCESO:%i // UBICACION: %i // CANTIDADPAGINAS: %i \n",proceso->PID,proceso->ubicacion,proceso->cantPagsUso);
	}
	printf("\n");
}
