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
		logMsg = string_from_format("Archivo de configuracion no encontrado. Parametro especificado: %s\n", nombreArchivoConfig);
		puts(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	} else {
		t_config* archivoConfig;
		archivoConfig = config_create(nombreArchivoConfig);
		configuracion = iniciarArchivoConfig();
		configuracion->puertoEscucha = config_get_int_value(archivoConfig, "PUERTO_ESCUCHA");
		configuracion->puertoSwap = config_get_int_value(archivoConfig, "PUERTO_SWAP");
		configuracion->ipSwap = strdup(config_get_string_value(archivoConfig, "IP_SWAP"));
		configuracion->nombreMemoria = strdup(config_get_string_value(archivoConfig, "NOMBRE_MEMORIA"));
		configuracion->maximosMarcosPorProceso = config_get_int_value(archivoConfig, "MAXIMO_MARCOS_POR_PROCESO");
		configuracion->cantidadMarcos = config_get_int_value(archivoConfig, "CANTIDAD_MARCOS");
		configuracion->tamanioMarcos = config_get_int_value(archivoConfig, "TAMANIO_MARCO");
		configuracion->entradasTlb = config_get_int_value(archivoConfig, "ENTRADAS_TLB");
		configuracion->retardoMemoria = config_get_int_value(archivoConfig, "RETARDO_MEMORIA");
		configuracion->tlbHabilitada = config_get_int_value(archivoConfig, "TLB_HABILITADA");
		log_info(logger, "[INFO]: Archivo de configuracion leido correctamente");

		logMsg = string_from_format("Archivo de configuracion leido correctamente\n");
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

	for (contador = 0; contador < cantPag; contador++) {
		contadorPagTP++;
		tablaDePag = iniciarTablaDePaginas();
		tablaDePag->idProc = idProc;
		tablaDePag->paginaDelProceso = contador;
		tablaDePag->idMarco = -1; // porque no esta en algun marco en mem pcpal
		tablaDePag->bitPagModificada = 0;
		list_add(listaTablaDePag, tablaDePag);
	}
    enviarRtaIniciarOkCPU (estructura, socketCPU);

	free(estructura);
}

void escribir(int idProc, int nroPag, char* textoAEscribir, int socketSwap) {
	// 1 -ver si estan en memoria y ponerle el bit de modificada
	// 2- si no esta mandar a escribir a swap

	t_contenido_pagina * escritura;
	escritura = iniciarEscrituraProc();
	int idMarco;

	//veo si esta en un marco de memoria

	idMarco = buscarSiEstaEnMemoria(idProc,nroPag);

	if( idMarco == -1){
		// 2
			escritura->numeroPagina = nroPag;
			escritura->PID = idProc;
			escritura->contenido = textoAEscribir;

			enviarEscribirAlSwap(escritura,socketSwap);

	}else {// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(idMarco,textoAEscribir);
	}

}


void leer(int idProc, int pag, int socketSwap, int socketCPU) {

	int a, respuesta;
	char* contenido;
	t_contenido_pagina * lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	t_lectura * lecturaSwap;
	lecturaSwap = iniciarLectura();

		lecturaMandarCpu->PID = idProc;
		lecturaMandarCpu->numeroPagina = a;

		respuesta = buscarSiEstaEnMemoria(idProc, a);

		if (respuesta == -1) {
			traerDeSwapUnaPaginaDeUnProceso(idProc, a, socketSwap); // aca se tiene que pedir a swap la pagina a y del proceso idProc
		} else { // aca significa que trajo el id porque esta en memoria

			contenido = traerContenidoDeMarco(respuesta);
			lecturaMandarCpu->contenido = contenido;

		}

		enviarACPUContenidoPaginaDeUnProceso(lecturaMandarCpu, socketCPU); // en esta funcion se tiene que mandar a cpu el lecturaMandarCPU



}



void finalizar(int idProc) {
	int a,tamanioListaId,id;
	t_list * listaDeId;
	listaDeId = buscarLosIdDeProceso(idProc);
	tamanioListaId = list_size(listaDeId);

	for(a=0;a<tamanioListaId;a++){
			id= list_get(listaDeId,a);
			eliminarDeMemoria(id);
			eliminarDeTablaDePaginas(id);
			if(configuracion->tlbHabilitada == 0){
				eliminarDeTLBSiEsta(id);
			}
		}

	enviarASwapEliminarProceso(idProc);

}


void enviarIniciarAlSwap(t_iniciar_swap *estructura, int socketSwap) {
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}
void enviarEscribirAlSwap(t_contenido_pagina *estructura, int socketSwap){
	enviarStruct(socketSwap, ESCRIBIR_SWAP, estructura);
}
void enviarRtaIniciarOkCPU (t_PID * estructura, int socketCPU){
	enviarStruct(socketCPU, RESUL_INICIAR_PROC_OK_CPU, estructura);
}
void enviarRtaEscribirACPU(t_contenido_pagina *estructura, int socketCPU){
	enviarStruct(socketCPU, RESUL_ESCRIBIR_OK, estructura);
}

void enviarRtaIniciarFalloCPU (t_PID * estructura, int socketCPU){
	enviarStruct(socketCPU, RESUL_INICIAR_PROC_NO_OK_CPU, estructura);
}

void enviarASwapContenidoDePaginaDesactualizada(int idProc, int pagina, char* contenido) {

}

void enviarACPUContenidoDePaginaDeUnProceso(t_contenido_pagina* lecturaMandarCpu) {

}

