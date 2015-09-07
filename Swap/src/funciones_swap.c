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

void agregarEnLaPosicionAdecuada(l_espacioLibre *espacioLibre, t_list *listaDeEspaciosLibres){
	l_espacioLibre* espacioA;
		espacioA = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
		l_espacioLibre* espacioB;
		espacioB = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
		int a;
			for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {

				espacioA = list_get(listaDeEspaciosLibres, a);
				espacioB = list_get(listaDeEspaciosLibres, a + 1);

				if((espacioA->ubicacion + espacioA->cantPagsLibres) != (espacioB->ubicacion - 1)){
					if((espacioLibre->ubicacion + espacioLibre->cantPagsLibres) == (espacioB->ubicacion -1)){
						list_add_in_index(listaDeEspaciosLibres,a+1,espacioLibre);
					}
				}
}
}
