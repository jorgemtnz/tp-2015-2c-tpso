#include "Memoria.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion(int argc, char *argv[]) {

	char* logMsg = NULL;

	if (argc < 2) {
		logMsg =
				string_from_format(
						"Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./Memoria /home/utnso/tp-2015-2c-tpso/Memoria/config_memoria.cfg\n");
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	}

	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
	int result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		logMsg =
				string_from_format(
						"Archivo de configuracion no encontrado. Parametro especificado: %s\n",
						nombreArchivoConfig);
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	} else {
		t_config* archivoConfig;
		archivoConfig = config_create(nombreArchivoConfig);
		//warning asignacion diferentes tipos
		configuracion = iniciarArchivoConfig();
		configuracion->puertoEscucha = config_get_int_value(archivoConfig,
				"PUERTO_ESCUCHA");
		configuracion->puertoSwap = config_get_int_value(archivoConfig,
				"PUERTO_SWAP");
		configuracion->ipSwap = strdup(
				config_get_string_value(archivoConfig, "IP_SWAP"));

		configuracion->nombreMemoria = strdup(
				config_get_string_value(archivoConfig, "NOMBRE_MEMORIA"));

		configuracion->maximosMarcosPorProceso = config_get_int_value(
				archivoConfig, "MAXIMO_MARCOS_POR_PROCESO");
		configuracion->cantidadMarcos = config_get_int_value(archivoConfig,
				"CANTIDAD_MARCOS");
		configuracion->tamanioMarcos = config_get_int_value(archivoConfig,
				"TAMANIO_MARCO");
		configuracion->entradasTlb = config_get_int_value(archivoConfig,
				"ENTRADAS_TLB");
		configuracion->tlbHabilitada = config_get_int_value(archivoConfig,
				"TLB_HABILITADA");
		configuracion->retardoMemoria = config_get_int_value(archivoConfig,
						"RETARDO_MEMORIA");
		configuracion->algoritmo_reemplazo =strdup( config_get_string_value(
				archivoConfig, "ALGORITMO_REEMPLAZO"));

		log_info(logger,
				"[INFO]: Archivo de configuracion leido correctamente");

		logMsg = string_from_format(
				"Archivo de configuracion leido correctamente\n");
		puts(logMsg);
		log_error(logger, logMsg);

		//config_destroy(archivoConfig);

	}

}

void iniciar(int idProc, int cantPag, int socketCPU) {
	int contador;
	t_TablaDePaginas* tablaDePag;
	t_iniciar_swap * estructura;
	estructura = crearEstructuraIniciar();
	estructura->PID = idProc;
	estructura->cantidadPaginas = cantPag;
	printf("cant pag  %i \n", cantPag);

	for (contador = 0; contador < cantPag; contador++) {
		variableIdMarco++;
		tablaDePag = iniciarTablaDePaginas();
		tablaDePag->idProc = idProc;
		tablaDePag->paginaDelProceso = contador;
		tablaDePag->idMarco = variableIdMarco; // porque no esta en algun marco en mem pcpal
		tablaDePag->bitPagModificada = 0;
		tablaDePag->bitPresencia = 0;
		list_add(listaTablaDePag, tablaDePag);
	}
	printf("tamanio tabla %i\n", list_size(listaTablaDePag));
	//warning paso incompatible para el primer parametro diferente tipo para la estructura que se pasa por lo que llega mal a CPU
	t_PID * estructuraEnvio = malloc(sizeof(t_PID));
	estructuraEnvio->PID = estructura->PID;
	enviarRtaIniciarOkCPU(estructuraEnvio, socketCPU);

}

void escribir(int idProc, int nroPag, char* textoAEscribir, int socketSwap,
		int socketCPU) {

	t_contenido_pagina * escritura;
	escritura = iniciarContenidoPagina();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	marcoYBit = buscarSiEstaEnMemoria(idProc, nroPag);

	escritura->numeroPagina = nroPag;
	escritura->PID = idProc;
	escritura->contenido = textoAEscribir;

	if (marcoYBit->bitPresencia == 0) { // traer de swap una pag, cargarla a memoria

		//sleep(configuracion->retardoMemoria);
		traerDeSwapUnaPaginaDeUnProcesoPorEscribir(idProc, nroPag, socketSwap);

	} else {	// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(marcoYBit->idMarco,
				textoAEscribir);
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
		//sleep(configuracion->retardoMemoria);
		traerDeSwapUnaPaginaDeUnProceso(idProc, pag, socketSwap); // aca se tiene que pedir a swap la pagina a y del proceso idProc

	} else { // aca significa que trajo el id porque esta en memoria
		contenido = traerContenidoDeMarco(marcoYBit->idMarco);
		lecturaMandarCpu->contenido = contenido;
		enviarACPUContenidoPaginaDeUnProcesoPorLeer(lecturaMandarCpu,
				socketCPU);
	}
}

void finalizar(t_PID* estructuraFinalizar, int socketSwap) {
	//warning no uso de variable, entonces se comenta
	int a, tamanioListaId;
//	int id;
	t_list * listaDemarcoYBit;
	listaDemarcoYBit = buscarLosMarcoYBitDeProceso(estructuraFinalizar->PID);
	tamanioListaId = list_size(listaDemarcoYBit);
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	for (a = 0; a < tamanioListaId; a++) {
		marcoYBit = list_get(listaDemarcoYBit, a);
		eliminarDeTablaDePaginasDefinitivamente(marcoYBit->idMarco);
		if (marcoYBit->bitPresencia == 0) {
			eliminarDeMemoria(marcoYBit->idMarco);
		}
		if (configuracion->tlbHabilitada == 0) {
			eliminarDeTLBDefinitivamente(marcoYBit->idMarco);
		}
	}

	//sleep(configuracion->retardoMemoria);
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

