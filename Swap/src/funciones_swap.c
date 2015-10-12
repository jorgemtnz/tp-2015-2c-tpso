#include "Swap.h"
//escribir las funciones aqui

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
		log_error(logger, logMsg);

		config_destroy(archivoConfig);
	}

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

	char* espacioVacio = string_repeat('\0', configuracion->tamanioPagina);
	escribirEnEspacioDatos(espacioDatos, espacioVacio, offset, configuracion->tamanioPagina);

}

void enviarResultadoIniciarOK(int socket, void* estructura) {
	enviarStruct(socket, RESUL_INICIAR_PROC_OK, estructura);
}

void enviarResultadoIniciarERROR(int socket, void* estructura) {
	enviarStruct(socket, RESUL_INICIAR_PROC_ERROR, estructura);
}

t_respuesta_iniciar_o_finalizar* iniciar(t_iniciar_swap* estructuraIniciar, t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados) {

	l_procesosCargados* procesoAInsertarEnLista;
	l_espacioLibre* espacioLibre;
	l_espacioLibre* espacioLibreAInsertar;
	t_respuesta_iniciar_o_finalizar* estructura;
	estructura = crearDevolucionIniciarOFinalizar();

	procesoAInsertarEnLista = crearProceso();
	espacioLibre = crearEspacioLibre();
	espacioLibreAInsertar = crearEspacioLibre();
	int a, paginasLibresRestantes;
	char* msjDeRta = string_new();

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
				procesoAInsertarEnLista->ubicacion = espacioLibre->ubicacion;
				procesoAInsertarEnLista->cantPagsUso = estructuraIniciar->cantidadPaginas;
				list_add(listaDeProcesosCargados, procesoAInsertarEnLista);
				paginasLibresRestantes = espacioLibre->cantPagsLibres - estructuraIniciar->cantidadPaginas;
				list_remove(listaDeEspaciosLibres, a);
				espacioLibreAInsertar->cantPagsLibres = paginasLibresRestantes;
				espacioLibreAInsertar->ubicacion = espacioLibre->ubicacion + estructuraIniciar->cantidadPaginas;
				list_add_in_index(listaDeEspaciosLibres, a, espacioLibreAInsertar);
				a = list_size(listaDeEspaciosLibres) + 1;
			}

			estructura->PID = procesoAInsertarEnLista->PID;
			estructura->resultado = OK;
			/*enviarResultadoIniciarOK(socket, estructura);

			 string_append(&msjDeRta,"mProc ");
			 string_append(&msjDeRta,string_itoa(procesoAInsertarEnLista->PID));
			 string_append(&msjDeRta," - Iniciado");
			 puts(msjDeRta);
			 log_info(logger, msjDeRta);*/

			return estructura;

		}
	} else {
		estructura->PID = estructuraIniciar->PID;
		estructura->resultado = ERROR;

		/*enviarResultadoIniciarERROR(socket, estructura);

		 string_append(&msjDeRta,"mProc ");
		 string_append(&msjDeRta,string_itoa(estructuraIniciar->PID));
		 string_append(&msjDeRta," - Fallido");
		 puts(msjDeRta);
		 log_error(logger, msjDeRta);*/
		return estructura;

	}

}
/*
 void enviarResultadoEscribirOK(int socket, void* estructura){
 enviarStruct(socket, RESUL_ESCRIBIR_OK, estructura);
 }

 void enviarResultadoEscribirERROR(int socket, void* estructura){
 enviarStruct(socket, RESUL_ESCRIBIR_ERROR, estructura);
 }*/

t_devolucion_escribir_o_leer* escribir(t_list* listaDeProcesosCargados, t_contenido_pagina* procesoAEscribir) {
	sleep(configuracion->retardo_swap);
	l_procesosCargados* unProceso;
	unProceso = crearProceso();
	t_devolucion_escribir_o_leer* respuestaDeEscribir;
	respuestaDeEscribir = crearDevolucionEscribirOLeer();
	int nuevaPagina;
	char* msjDeRta = string_new();
	int a, ubicacion;
	for (a = 0; a <= list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
		unProceso = list_get(listaDeProcesosCargados, a);
		if (unProceso->PID == procesoAEscribir->PID) {

			ubicacion = unProceso->ubicacion;
			a = list_size(listaDeProcesosCargados) + 1; //salgo del for
		}
	}
	if (string_length(procesoAEscribir->contenido) == 0) {
		nuevaPagina = (configuracion->tamanioPagina) * (ubicacion + procesoAEscribir->numeroPagina);
		escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, nuevaPagina, configuracion->tamanioPagina);
	} else {
		int longitud = string_length(procesoAEscribir->contenido);
		nuevaPagina = (configuracion->tamanioPagina) * (ubicacion + procesoAEscribir->numeroPagina);
		escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, nuevaPagina, longitud);
	}

	respuestaDeEscribir->PID = unProceso->PID;
	respuestaDeEscribir->numeroPagina = procesoAEscribir->numeroPagina;
	respuestaDeEscribir->contenido = procesoAEscribir->contenido;
	respuestaDeEscribir->resultado = OK;
	return respuestaDeEscribir;
	/*enviarResultadoEscribirOK(socket, respuestaDeEscribir);

	 string_append(&msjDeRta,"Mproc ");
	 string_append(&msjDeRta,string_itoa(unProceso->PID));
	 string_append(&msjDeRta," - Pagina ");
	 string_append(&msjDeRta,string_itoa(nuevaPagina));
	 string_append(&msjDeRta," escrita: ");
	 string_append(&msjDeRta,procesoAEscribir->contenido);
	 puts(msjDeRta);
	 */
}
/*
 void enviarResultadoLeerOK(int socket, void* estructura){
 enviarStruct(socket, RESUL_LEER_OK, estructura);
 }

 void enviarResultadoLeerERROR(int socket, void* estructura){
 enviarStruct(socket, RESUL_LEER_ERROR, estructura);
 }
 */
t_devolucion_escribir_o_leer* leer(t_leerDeProceso *procesoRecibido, t_list* listaDeProcesosCargados) {
	sleep(configuracion->retardo_swap);
	int a, x;
	char* datosLeidos;
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
	if (procesoRecibido->numeroPaginaFin - procesoRecibido->numeroPaginaInicio != 0) {
		for (x = 0; x <= procesoRecibido->numeroPaginaFin - procesoRecibido->numeroPaginaInicio; x++) {

			datosLeidos = leerEspacioDatos(espacioDatos, ((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio + x) * (configuracion->tamanioPagina)),
					(configuracion->tamanioPagina));
			string_append(&datosLeidosFinal, datosLeidos);
			string_append(&datosLeidosFinal, " ");

		}

	} else {
		datosLeidosFinal = leerEspacioDatos(espacioDatos, ((procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio) * (configuracion->tamanioPagina)),
				configuracion->tamanioPagina);
	}

	respuestaDeLeer->PID = procesoAleer->PID;
	respuestaDeLeer->contenido = datosLeidosFinal;
	respuestaDeLeer->numeroPagina = procesoRecibido->numeroPaginaInicio;
	//enviarResultadoLeerOK(socket, respuestaDeLeer);
	respuestaDeLeer->resultado = OK;
	return respuestaDeLeer;
}
/*
 void enviarResultadoFinalizarOK(int socket, void* estructura){
 enviarStruct(socket, RESUL_FIN_OK, estructura);
 }

 void enviarResultadoFinalizarERROR(int socket, void* estructura){
 enviarStruct(socket, RESUL_FIN_ERROR, estructura);
 }
 */
t_respuesta_iniciar_o_finalizar* finalizar(uint8_t pid, t_list* listaDeProcesosCargados, t_list* listaDeEspaciosLibres) {
	int a, b;
	l_procesosCargados* unProceso;
	l_espacioLibre* espacioLibre;
	t_contenido_pagina* procesoAEscribir;
	procesoAEscribir = crearEscribirEnProceso();
	espacioLibre = crearEspacioLibre();
	unProceso = crearProceso();
	t_respuesta_iniciar_o_finalizar* respuestaDeFinalizar;
	respuestaDeFinalizar = crearDevolucionIniciarOFinalizar();

	for (a = 0; a <= list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA

		unProceso = list_get(listaDeProcesosCargados, a);

		if (unProceso->PID == (pid)) {

			espacioLibre->ubicacion = unProceso->ubicacion;

			espacioLibre->cantPagsLibres = unProceso->cantPagsUso;

			agregarEnLaPosicionAdecuada(espacioLibre, listaDeEspaciosLibres);

			//BORRAR DEL ESPACIO DE DATOS
			char* espacioVacio = string_repeat('\0', configuracion->tamanioPagina);

			for (b = 0; b < unProceso->cantPagsUso; b++) {
				procesoAEscribir->PID = unProceso->PID;
				procesoAEscribir->contenido = espacioVacio;
				procesoAEscribir->numeroPagina = b;

				escribir(listaDeProcesosCargados, procesoAEscribir);

			}

			list_remove(listaDeProcesosCargados, a);
			a = list_size(listaDeProcesosCargados) + 1; //PARA SALIR DEL FOR CUANDO LO ENCONTRE
		}
	}
	respuestaDeFinalizar->PID = unProceso->PID;
	respuestaDeFinalizar->resultado = OK;
	//enviarResultadoFinalizarOK(socket, respuestaDeFinalizar);
	return respuestaDeFinalizar;
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
	//free(espacioA);
	//free(espacioB);
	//free(espacioC);
}

void compactarMemoria(t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados) {
	sleep(configuracion->retardo_compactacion);
	log_error(logger, "ERROR: La memoria será compactada por fragmentación externa");
	//ORDENAR LISTA POR UBICACION
	//list_sort(listaDeProcesosCargados, comparador(list_get(listaDeProcesosCargados,i)));
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
	nuevoEspacioLibre->ubicacion = ultimoLugarOcupado + 1;
	nuevoEspacioLibre->cantPagsLibres = configuracion->cantidadPaginas - ultimoLugarOcupado;
	list_clean(listaDeEspaciosLibres);
	list_add(listaDeEspaciosLibres, nuevoEspacioLibre);
	log_info(logger, "Memoria compactada correctamente");
	free(espacioProcAux);
	free(espacioProcSig);
	free(nuevoEspacioLibre);
}
_Bool comparador(l_procesosCargados proc1, l_procesosCargados proc2) {
	int ub1 = proc1.ubicacion;
	int ub2 = proc2.ubicacion;
	return (ub1 < ub2);
}

void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres) {
	l_espacioLibre* espacioA;
	espacioA = crearEspacioLibre();
	l_espacioLibre* espacioB;
	espacioB = crearEspacioLibre();
	int a;

	if (list_size(listaDeEspaciosLibres) > 1) {
		for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {

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
