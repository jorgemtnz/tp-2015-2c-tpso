#include "Memoria.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion(int argc, char *argv[]) {

	char* logMsg = NULL;

	if (argc < 2) {
		logMsg =
				string_from_format(
						"Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./Memoria /home/utnso/tp-2015-2c-tpso/Memoria/config_memoria.cfg\n");
		puts(logMsg);
		my_log_error(logMsg);
		exit(-1);
	}

	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
	int result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		logMsg = string_from_format("Archivo de configuracion no encontrado. Parametro especificado: %s\n", nombreArchivoConfig);
		puts(logMsg);
		my_log_error(logMsg);
		exit(-1);
	} else {
		t_config* archivoConfig;
		archivoConfig = config_create(nombreArchivoConfig);
		//warning asignacion diferentes tipos
		configuracion = iniciarArchivoConfig();
		configuracion->puertoEscucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
		configuracion->puertoSwap = config_get_int_value(archivoConfig, "PUERTO_SWAP");
		configuracion->ipSwap = strdup(config_get_string_value(archivoConfig, "IP_SWAP"));

		char* nombreMemoria = config_get_string_value(archivoConfig, "NOMBRE_MEMORIA");
		configuracion->nombreMemoria = strdup(nombreMemoria != NULL ? nombreMemoria : "NOMBRE_MEMORIA");

		configuracion->maximosMarcosPorProceso = config_get_int_value(archivoConfig, "MAXIMO_MARCOS_POR_PROCESO");
		configuracion->cantidadMarcos = config_get_int_value(archivoConfig, "CANTIDAD_MARCOS");
		configuracion->tamanioMarcos = config_get_int_value(archivoConfig, "TAMANIO_MARCO");
		configuracion->entradasTlb = config_get_int_value(archivoConfig, "ENTRADAS_TLB");
		configuracion->tlbHabilitada = config_get_int_value(archivoConfig, "TLB_HABILITADA");
		configuracion->retardoMemoria = config_get_int_value(archivoConfig, "RETARDO_MEMORIA");
		configuracion->algoritmo_reemplazo = strdup(config_get_string_value(archivoConfig, "ALGORITMO_REEMPLAZO"));
		my_log_info("[INFO]: Archivo de configuracion leido correctamente");

		logMsg = string_from_format("Archivo de configuracion leido correctamente\n");
		puts(logMsg);

		//config_destroy(archivoConfig);

	}

}

void iniciar(int idProc, int cantPag, int socketCPU) {
	int contador;
	t_TablaDePaginas* tablaDePag;
	t_PID * estructuraEnvio;
	estructuraEnvio = crearPID();

	for (contador = 0; contador < cantPag; contador++) {
		tablaDePag = iniciarTablaDePaginas();
		tablaDePag->idProc = idProc;
		tablaDePag->paginaDelProceso = contador;
		tablaDePag->idMarco = variableIdMarco;
		tablaDePag->bitPagModificada = 0;
		tablaDePag->bitPresencia = 0;
		pthread_mutex_lock(&mutexTablaPags);
		list_add(listaTablaDePag, tablaDePag);
		pthread_mutex_unlock(&mutexTablaPags);

		variableIdMarco++;
	}

	estructuraEnvio->PID = idProc;

	int* cero = malloc(sizeof(int));
	*cero = 0;
	list_add_in_index(listaIndices, idProc, cero);

	enviarRtaIniciarOkCPU(estructuraEnvio, socketCPU);

}

void escribir(int idProc, int nroPag, char* textoAEscribir, int socketSwap, int socketCPU) {

	t_contenido_pagina * escritura;
	escritura = iniciarContenidoPagina();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	marcoYBit = buscarSiEstaEnMemoria(idProc, nroPag);

	escritura->numeroPagina = nroPag;
	escritura->PID = idProc;
	string_append(&escritura->contenido, textoAEscribir);

	if (marcoYBit->bitPresencia == 0) { // traer de swap una pag, cargarla a memoria

		usleep(configuracion->retardoMemoria * 1000);
		traerDeSwapUnaPaginaDeUnProcesoPorEscribir(idProc, nroPag, textoAEscribir, socketSwap);
		char* textoLogger = string_new();
		string_append(&textoLogger, string_from_format("Acceso a swap (fallo de página),  PID: %i, pagina: %i", idProc, nroPag));
		my_log_info(textoLogger);

	} else {	// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(marcoYBit->idMarco, textoAEscribir);

		//LOG
		char* textoLogger = string_new();
		string_append(&textoLogger, string_from_format("Acceso a memoria realizado, PID: %i, N° de"
				" página: %i y N° de marco: %i", idProc, nroPag, marcoYBit->idMarco));
		my_log_info(textoLogger);

		enviarRtaEscribirACPU(escritura, socketCPU);
	}

}

void leer(int idProc, int pag, int socketSwap, int socketCPU) {

	char* contenido;

	t_contenido_pagina * lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;

	marcoYBit = buscarSiEstaEnMemoria(idProc, pag);

	if (marcoYBit->bitPresencia == 0) {	// no lo encontro
		usleep(configuracion->retardoMemoria * 1000);
		traerDeSwapUnaPaginaDeUnProceso(idProc, pag, socketSwap); // aca se tiene que pedir a swap la pagina a y del proceso idProc
		char* textoLogger = string_new();
		string_append(&textoLogger, string_from_format("Acceso a swap (fallo de página),  PID: %i, pagina: %i", idProc, pag));
		my_log_info(textoLogger);
	} else { // aca significa que trajo el id porque esta en memoria
		contenido = traerContenidoDeMarco(marcoYBit->idMarco);
		char* textoLogger = string_new();
		string_append(&textoLogger, string_from_format("Acceso a memoria realizado, PID: %i, N° de"
				" página: %i y N° de marco: %i", idProc, pag, marcoYBit->idMarco));
		my_log_info(textoLogger);
		lecturaMandarCpu->contenido = contenido;
		enviarACPUContenidoPaginaDeUnProcesoPorLeer(lecturaMandarCpu, socketCPU);
	}

}

void finalizar(t_PID* estructuraFinalizar, int socketSwap) {
	int a, tamanioListaId;
	t_list * listaDemarcoYBit;
	listaDemarcoYBit = buscarLosMarcoYBitDeProceso(estructuraFinalizar->PID);
	tamanioListaId = list_size(listaDemarcoYBit);
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	eliminarDeTablaDePaginasDefinitivamente(estructuraFinalizar->PID);

	for (a = 0; a < tamanioListaId; a++) {
		marcoYBit = list_get(listaDemarcoYBit, a);
		if (marcoYBit->bitPresencia == 1) {
			eliminarDeMemoria(marcoYBit->idMarco);
		}
		if (configuracion->tlbHabilitada == 1) {
			eliminarDeTLBDefinitivamente(marcoYBit->idMarco);
		}
	}

	usleep(configuracion->retardoMemoria * 1000);
	enviarFinalizarASwap(estructuraFinalizar, socketSwap);

}

void enviarIniciarAlSwap(t_iniciar_swap *estructura, int socketSwap) {
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}
void enviarEscribirAlSwap(t_contenido_pagina *estructura, int socketSwap) {
	enviarStruct(socketSwap, ESCRIBIR_SWAP, estructura);
}
void enviarRtaIniciarOkCPU(t_PID * estructura, int socketCPU) {
	enviarStruct(socketCPU, RESUL_INICIAR_PROC_OK_CPU, estructura);
}
void enviarRtaEscribirACPU(t_contenido_pagina *estructura, int socketCPU) {
	enviarStruct(socketCPU, RESUL_ESCRIBIR, estructura);
}

void enviarRtaIniciarFalloCPU(t_PID * estructura, int socketCPU) {
	enviarStruct(socketCPU, RESUL_INICIAR_PROC_NO_OK_CPU, estructura);
}

