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
	int rtaSwap, contador;
	t_TablaDePaginas* tablaDePag;

	/* PARTE DE ENVIAR A SWAP UN PROCESO
	 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
	 *  puts("Enviando \"puede iniciar proceso\" al Swap");
	 enviar(atoi(socketCPU), "puede iniciar proceso", strlen("puede iniciar proceso));
	 puts("Enviado al Swap");
	 */

	if (rtaSwap) { // rtaSwap == 0, indica ok
		for (contador = 0; contador < cantPag; contador++) {
			contadorPagTP++;
			tablaDePag = iniciarTablaDePaginas();
			tablaDePag->idProc = idProc;
			tablaDePag->paginaDelProceso = contador + 1;
			tablaDePag->idMarco = -1; // porque no esta en algun marco en mem pcpal
			list_add(listaTablaDePag, tablaDePag);
		}
	} else { // la respuesta no fue ok
		/* PARTE DE ENVIAR A CPU QUE NO SE ENVIO BIEN
		 *char* socketCPU = (char*) dictionary_get(conexiones, "Cpu");
		 *  puts("Enviando \"No puede iniciar el proceso\" al Cpu");
		 enviar(atoi(socketCPU), "No puede iniciar el proceso", strlen("No puede iniciar el proceso));
		 puts("Enviado al Cpu");
		 */
	}

	free(tablaDePag);
}

void escribir(int idProc, int nroPag, char* textoAEscribir) {
	// 1 -ver si estan en memoria y ponerle el bit de modificada
	// 2- si no esta mandar a escribir a swap

	t_escrituraProc * escritura;
	escritura = iniciarEscrituraProc();
	int idMarco;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();

	//veo si esta en un marco de memoria

	idMarco = buscarSiEstaEnMemoria(idProc,nroPag);

	if( idMarco == -1){
		// 2
			escritura->Pag = nroPag;
			escritura->idProc = idProc;
			escritura->texto = textoAEscribir;

			/* PARTE DE ENVIAR A SWAP UN PROCESO
			 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
			 *  puts("Enviando \"escribir paginas para tal proceso\" al Swap");
			 enviar(atoi(socketCPU), "escribir paginas para tal proceso", strlen("escribir paginas para tal proceso));
			 puts("Enviado al Swap");
			 */

	}else {// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(idMarco,textoAEscribir);
	}

}


void leer(int idProc, int pagIn, int pagFin) {

	int a, respuesta;
	char* contenido;
	t_rtaLecturaCpu * lecturaMandarCpu;
	lecturaMandarCpu = iniciarRtaLecturaCpu();
	t_lecturaSwap * lecturaSwap;
	lecturaSwap = iniciarLectura();

	for (a = pagIn; a <= pagFin; a++) {

		lecturaMandarCpu->idProc = idProc;
		lecturaMandarCpu->pagFin = a;
		lecturaMandarCpu->pagIn = a;

		respuesta = buscarSiEstaEnMemoria(idProc, a);
		if (respuesta == -1) {
			lecturaSwap = traerDeSwapUnaPaginaDeUnProceso(idProc, a); // aca se tiene que pedir a swap la pagina a y del proceso idProc

			if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
				sacarAlPrimeroDeMemoriaDelProceso(idProc);
				cargarNuevoMarcoAMemoria(lecturaSwap->contenido);

			} else if (estaLlenaLaMemoria()) {
				sacarAlPrimeroDeMemoria();
				cargarNuevoMarcoAMemoria(lecturaSwap->contenido);

			} else { // si no llego ni al maximo de procesos ni al maximo de marcos
				cargarNuevoMarcoAMemoria(lecturaSwap->contenido);

			}
			lecturaMandarCpu->contenido = lecturaSwap->contenido;

		} else { // aca significa que trajo el id porque esta en memoria

			contenido = traerContenidoDeMarco(respuesta);
			lecturaMandarCpu->contenido = contenido;

		}

		enviarACPUContenidoPaginaDeUnProceso(lecturaMandarCpu); // en esta funcion se tiene que mandar a cpu el lecturaMandarCPU

	}

}





void finalizar(int idProc) {
	int a,tamanioListaId,id;
	t_list * listaDeId;
	listaDeId = buscarLosIdDeProceso(idProc);
	tamanioListaId = list_size(listaDeId);

	for(a=0;a<tamanioListaId;a++){
		id= list_get(listaDeId,a);
		eliminarDeMemoria(id);
	}

	enviarASwapEliminarProceso(idProc);

}


void enviarIniciarAlSwap(t_iniciar_swap *estructura, int socketSwap) {
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}


void enviarASwapContenidoDePaginaDesactualizada(int idProc, int pagina, char* contenido) {

}

void enviarACPUContenidoDePaginaDeUnProceso(t_rtaLecturaCpu* lecturaMandarCpu) {

}

t_lecturaSwap* traerDeSwapUnaPaginaDesdeUnProceso(int idProc, int nroDePag) {

}
