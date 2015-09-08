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
		configuracion->retardo = config_get_int_value(archivoConfig, "RETARDO_COMPACTACION");
		configuracion->tamanioArchivo = strdup(config_get_string_value(archivoConfig, "TAMANIO_ARCHIVO"));

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

	crearArchivoMmap(pathArchivo, configuracion->tamanioArchivo);

	int fdEspacioDatos = abrirArchivoEspacioDatos(pathArchivo, logger);

	int offset = 0;

	espacioDatos = crearEspacioDeDatos(fdEspacioDatos, configuracion->tamanioArchivo, logger);

	char *contenido = "\0";
	escribirEnEspacioDatos(espacioDatos, contenido, offset);

}

void iniciar(int cantidadPaginas, t_list* listaDeEspaciosLibres, t_list* listaDeProcesosCargados, l_espacioLibre* espacioLibre, pid_t pid,
		l_procesosCargados* procesoAInsertarEnLista, l_espacioLibre* espacioLibreAInsertar) {
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
	if (cantidadDePagLibres >= cantidadPaginas) {

		acomodarEspaciosLibres(listaDeEspaciosLibres); // POR SI QUEDARON DON BLOQUES LIBRES CONTINUOS
		for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
			espacioLibre = list_get(listaDeEspaciosLibres, a);
			if (espacioLibre->cantPagsLibres >= cantidadPaginas) { // SI ENCUENTRO UN ESPACIO EN EL QE ENTRE LO METO y actualizo la lista de espacios libres
				procesoAInsertarEnLista->PID = pid;
				procesoAInsertarEnLista->ubicacion = espacioLibre->ubicacion;
				procesoAInsertarEnLista->cantPagsUso = cantidadPaginas;
				list_add(listaDeProcesosCargados, procesoAInsertarEnLista);
				paginasLibresRestantes = espacioLibre->cantPagsLibres - cantidadPaginas;
				list_remove(listaDeEspaciosLibres, a);
				espacioLibreAInsertar->cantPagsLibres = paginasLibresRestantes;
				espacioLibreAInsertar->ubicacion = espacioLibre->ubicacion + cantidadPaginas;
				list_add_in_index(listaDeEspaciosLibres, a, espacioLibreAInsertar);
			} else {
				//HACER ALGORITMO COMPACTACION
			}

		/*	char* socketMemoria = (char*) dictionary_get(conexiones, "Memoria");
			puts("Enviando \"respuesta de iniciar\" a Memoria");
			enviar(atoi(socketMemoria), "OK INICIAR", strlen("OK INICIAR"));
*/
		}
	} else {
		/*char* socketMemoria = (char*) dictionary_get(conexiones, "Memoria");
		puts("Enviando \"respuesta de iniciar\" a Memoria");
		enviar(atoi(socketMemoria), "ERROR INICIAR", strlen("ERROR INICIAR"));*/
	}
}

void escribir(t_list* listaDeProcesosCargados, l_procesosCargados* unProceso, t_escribirEnProceso* procesoAEscribir) {

	int a, ubicacion;
	for (a = 0; a <= list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
		unProceso = list_get(listaDeProcesosCargados, a);
		if (unProceso->PID == procesoAEscribir->PID) {

			ubicacion = unProceso->ubicacion;
		}
	}
	escribirEnEspacioDatos(espacioDatos, procesoAEscribir->contenido, (ubicacion + procesoAEscribir->numeroPagina));
/*	char* socketMemoria = (char*) dictionary_get(conexiones, "Memoria");
	puts("Enviando \"respuesta de escribir\" a Memoria");
	enviar(atoi(socketMemoria), "OK ESCRIBIR", strlen("OK ESCRIBIR"));*/
}

void leer(t_leerDeProceso *procesoRecibido, t_list* listaDeProcesosCargados, l_procesosCargados* unProceso, l_procesosCargados* procesoAleer) {
	int a;
	for (a = 0; a <= list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
		unProceso = list_get(listaDeProcesosCargados, a);
		if (unProceso->PID == procesoRecibido->PID) {
			procesoAleer->PID = unProceso->PID;
			procesoAleer->cantPagsUso = unProceso->cantPagsUso;
			procesoAleer->ubicacion = unProceso->ubicacion;
		}
	}
	char* datosLeidos = leerEspacioDatos(espacioDatos, (procesoAleer->ubicacion + procesoRecibido->numeroPaginaInicio),
			(procesoAleer->ubicacion + procesoRecibido->numeroPaginaFin));
	//MANDAR A MEMORIA DATOS LEIDOS
}

void finalizar(pid_t pid, t_list* listaDeProcesosCargados, l_procesosCargados* unProceso, l_espacioLibre* espacioLibre, t_list* listaDeEspaciosLibres) {
	int a;
	for (a = 0; a <= list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
		unProceso = list_get(listaDeProcesosCargados, a);
		if (unProceso->PID == pid) {
			list_remove(listaDeProcesosCargados, a);

			espacioLibre->ubicacion = unProceso->ubicacion;
			espacioLibre->cantPagsLibres = unProceso->cantPagsUso;
			agregarEnLaPosicionAdecuada(espacioLibre, listaDeEspaciosLibres);
			//BORRAR DEL ESPACIO DE DATOS
		}
	}
	/*char* socketMemoria = (char*) dictionary_get(conexiones, "Memoria");
	puts("Enviando \"respuesta de finalizar\" a Memoria");
	enviar(atoi(socketMemoria), "OK FINALIZAR", strlen("OK FINALIZAR"));*/
}
void acomodarEspaciosLibres(t_list* listaDeEspaciosLibres) {
	l_espacioLibre* espacioA;
	espacioA = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	l_espacioLibre* espacioB;
	espacioB = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	l_espacioLibre* espacioC;
	espacioC = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	int a;
	for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {

		espacioA = list_get(listaDeEspaciosLibres, a);
		espacioB = list_get(listaDeEspaciosLibres, a + 1);

		if ((espacioA->ubicacion + espacioA->cantPagsLibres) == (espacioB->ubicacion - 1)) {
			espacioC->ubicacion = espacioA->ubicacion;
			espacioC->cantPagsLibres = (espacioA->cantPagsLibres + espacioB->cantPagsLibres);
			list_remove(listaDeEspaciosLibres, a);
			list_remove(listaDeEspaciosLibres, a + 1);
			list_add_in_index(listaDeEspaciosLibres, a, espacioC);
			a--;
		}
	}

}

void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres) {
	l_espacioLibre* espacioA;
	espacioA = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	l_espacioLibre* espacioB;
	espacioB = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	int a;
	for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {

		espacioA = list_get(listaDeEspaciosLibres, a);
		espacioB = list_get(listaDeEspaciosLibres, a + 1);

		if ((espacioA->ubicacion + espacioA->cantPagsLibres) != (espacioB->ubicacion - 1)) {
			if ((espacioLibre->ubicacion + espacioLibre->cantPagsLibres) == (espacioB->ubicacion - 1)) {
				list_add_in_index(listaDeEspaciosLibres, a + 1, espacioLibre);
			}
		}
	}
}
