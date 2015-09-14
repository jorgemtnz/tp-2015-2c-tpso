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
		configuracion = malloc(sizeof(t_configuracion));
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

		config_destroy(archivoConfig);
	}

}

void iniciar(int idProc, int cantPag, int socketCPU) {
	int rtaSwap,contador;
	t_TablaDePaginas* tablaDePag;

	/* PARTE DE ENVIAR A SWAP UN PROCESO
	 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
	 *  puts("Enviando \"puede iniciar proceso\" al Swap");
	 enviar(atoi(socketCPU), "puede iniciar proceso", strlen("puede iniciar proceso));
	 puts("Enviado al Swap");
	 */

	if(rtaSwap) { // rtaSwap == 0, indica ok
		for (contador = 0; contador < cantPag; contador++) {
			contadorPagTP++;
			tablaDePag = malloc(sizeof(tablaDePag));
			tablaDePag->idProc = idProc;
			tablaDePag->paginaEnMemoria = contadorPagTP;
			tablaDePag->paginaDelProceso = contador + 1;
			tablaDePag->marco = -1; // porque no esta en algun marco en mem pcpal
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

void escribir(int idProc, int nroPag, char* textoAEscribir, ){
	// 1 -escribir en swap
	// 2 -ver si estan en memoria y ponerle el bit de modificada

	t_escrituraProc * escritura;
	escritura = malloc(sizeof(t_escrituraProc));
	int a,flagTLB=0,flagTablaPag=0,tamanioTLB,tamanioTablaPag,tamanioMemoria;
	t_TLB * campoTLB;
	campoTLB = malloc(sizeof(t_TLB));
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = malloc(sizeof(t_TablaDePaginas));
	t_marco * campoMemoria;
	campoMemoria = malloc(sizeof(t_marco));

	// 1
	escritura->Pag = nroPag;
	escritura->idProc = idProc;
	escritura->texto = textoAEscribir;

	/* PARTE DE ENVIAR A SWAP UN PROCESO
	 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
	 *  puts("Enviando \"escribir paginas para tal proceso\" al Swap");
	 enviar(atoi(socketCPU), "escribir paginas para tal proceso", strlen("escribir paginas para tal proceso));
	 puts("Enviado al Swap");
	 */

	// 2

	//veo si esta en un marco de memoria
	tamanioTLB=list_size(listaTLB);
	for(a=0;a<tamanioTLB && flagTLB ==0;a++){
		campoTLB=list_get(listaTLB,a);
		if(campoTLB->idProc == idProc && campoTLB->marco != -1 /*que este en un marco*/) {
			flagTLB =1;
		}
	}

	tamanioTablaPag = list_size(listaTablaDePag);
	for(a=0;a<tamanioTablaPag && flagTLB == 0 && flagTablaPag == 0;a++){
		campoTablaDePag = list_get(listaTablaDePag,a);
		if(campoTablaDePag->idProc == idProc && campoTablaDePag->marco != -1){
			flagTablaPag = 1;
		}
	}

	// si esta en un marco de memoria, le pongo el bit de modificada

	if(flagTLB == 1){ /* esta en TLB */
		tamanioMemoria = list_size(listaMemoria);
		for(a=0;a<tamanioMemoria;a++){
			campoMemoria = list_get(listaMemoria,a);
			if(campoMemoria->idProc == idProc){
				campoMemoria->bitPagModificada = 1;
			}
		}
	} else if (flagTablaPag == 1) {
		tamanioMemoria = list_size(listaMemoria);
		for (a = 0; a < tamanioMemoria; a++) {
			campoMemoria = list_get(listaMemoria, a);
			if (campoMemoria->idProc == idProc) {
				campoMemoria->bitPagModificada = 1;
			}
		}

		free(campoMemoria);
		free(campoTLB);
		free(campoTablaDePag);


}

void leer(int idProc, int pagIn, int PagFin) {

}

void finalizar(int idProc) {

}
