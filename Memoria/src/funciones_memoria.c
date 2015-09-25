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

		config_destroy(archivoConfig);
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
	int a, flagTLB = 0, flagTablaPag = 0, tamanioTLB, tamanioTablaPag, tamanioMemoria, respuesta;
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

	// aca tengo que empezar a crear los marcos y todo eso




	/*Por cada pagina
	 * 1- ver a traves de la tlb y la tabla de pag si esta en memoria
	 *
	 * Si no esta
	 *
	 * 2- enviar al swap aplicandole el sleep
	 * 3- recibir respuesta del swap
	 * 4- guardarla en el marco. Ver si sobrepasa la cantidad maxima por procesos, si es asi
	 *    va a haber que eliminar el primero ingresado de ese proceso y registrar ese cambio en la
	 *    tabla de paginas y si esta en tlb tambien en tlb. Ver la cantidad
	 *   maxima de marcos, si lo sobrepasa va a tener que eliminar el primero ingresado.
	 *   Siempre que se elimine uno, se va a tener que verificar el bitDeModificada, si esta en
	 *   uno se va a tener que mandar una escritura a swap de dicha pagina de ese proceso.
	 *   Al sacarlo tengo que registrar que saque este proceso de memoria en la tabla de paginas y si lo tengo en tlb tambien de la tlb.
	 *   Cuando se guarda el proceso, se va atener que actualizar la TLB y la Tabla De Paginas
	 *5- ver si tengo tlb, ver si llena la tlb y sacar el primero ingresado si es asi
	 *6- guardarla en la tabla de paginas ( aca no hay un maximo, ya que el maximo es la cant max de procesos en marcos)
	 */
	int a,b,c,d, tamanioMemoria, flagMemoria,cantMarcosLibres,flagProcesos,tamanioTLB,tamanioTablaDePag;
	int contMaxProc =0,flagGuardarPrimero,flagTablaDePag = 0;
	t_rtaLecturaCpu * lecturaMandarCpu;
	lecturaMandarCpu = iniciarRtaLecturaCpu();
	t_rtaLecturaCpu * lecturaRecibirSwap;
	lecturaRecibirSwap = iniciarRtaLecturaCpu();
	t_lecturaSwap * lecturaMandarSwap;
	lecturaMandarSwap = iniciarLectura();
	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();
	t_marco * campoMemoriaAux;
	campoMemoriaAux = iniciarMarco();
	t_marco * campoMemoriaAAgregar;
	campoMemoriaAAgregar = iniciarMarco();
	t_marco * campoMemoriaPrimerProceso;
	campoMemoriaPrimerProceso = iniciarMarco();
	t_escrituraProc * mandarASwapPorTenerBitModificada;
	mandarASwapPorTenerBitModificada = iniciarEscrituraProc();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * primerCampoTablaDePagDelProceso;
	primerCampoTablaDePagDelProceso = iniciarTablaDePaginas();

	for (a = pagIn; a <= pagFin; a++) {

	}

/*
	for (a = pagIn; a <= pagFin; a++) {
		//1
		flagMemoria = 0;
		tamanioMemoria = list_size(listaMemoria);

		//busca si esta
		for (c = 0; c < tamanioMemoria && flagMemoria == 0; c++) {
			campoMemoria = list_get(listaMemoria, a);
			if (idProc == campoMemoria->idProc && campoMemoria->paginaDelProceso == a) {
				flagMemoria = 1;
			}
		}
		if (flagMemoria == 1) { // 1 = si esta
				lecturaMandarCpu->idProc = idProc;
				lecturaMandarCpu->contenido = campoMemoria->contenido;
				lecturaMandarCpu->pagIn = a;
				lecturaMandarCpu->pagFin = a;

			/* PARTE DE ENVIAR A CPU UN RTA DE LEER UNA PAG
			 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
			 *  puts("Enviando \"mandar a cpu un rta de leer una pag\" al Swap");
			 enviar(atoi(socketCPU), "mandar a cpu un rta de leer una pag", strlen("mandar a cpu un rta de leer una pag"));
			 puts("Enviado al Swap");

			}
		else { // aca hay que empezar con el //2
			lecturaMandarSwap->idProc = campoMemoria->idProc;
			lecturaMandarSwap->pagIn = a;
			lecturaMandarSwap->pagFin = a ;

			sleep(configuracion->retardoMemoria);

			/* PARTE DE ENVIAR A Swap UN LEER UNA PAG
			 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
			 *  puts("Enviando \"mandar a swap un leer una pag\" al Swap");
			 enviar(atoi(socketCPU), "mandar a swap un leer una pag", strlen("mandar a swap un leer una pag"));
			 puts("Enviado al Swap");


			//3
			//aca tendria que recibir de swap la rta de leer una pagina lo hardocdeo y hago de cuenta como que recibi ok
			char* contenido;
			lecturaRecibirSwap->idProc = idProc;
			lecturaRecibirSwap->pagIn = a;
			lecturaRecibirSwap->pagFin = a;
			lecturaRecibirSwap->contenido = contenido;

			//4
			cantMarcosLibres = configuracion->cantidadMarcos - tamanioMemoria ;

			if(cantMarcosLibres > 0){ // hay al menos un espacio disponible en memoria
				flagProcesos = 0;
				for(b=0;b< tamanioMemoria && flagProcesos == 0;b++){// con este for voy a ver si se llego al maximo de marcos por proceso
					campoMemoriaAux = list_get(listaMemoria, b);
					if(campoMemoriaAux->idProc == idProc){
						contMaxProc ++;
						if(flagGuardarPrimero == 0){ // si esta en cero es porque no tiene guardado el primero encontrado de este proceso
							campoMemoriaPrimerProceso = campoMemoriaAux;
							flagGuardarPrimero = 1;
						}

						if(contMaxProc >= configuracion->maximosMarcosPorProceso){ //
							flagProcesos=1;
						}
					}
				}
				// si el flagProcesos quedo en 0 es porque no se llego al maximo de marcos por proceso
				if(flagProcesos == 0){
					campoMemoriaAAgregar->bitPagModificada = 0;
					campoMemoriaAAgregar->contenido = contenido;
					campoMemoriaAAgregar->idMarco = idMarco ++;
					campoMemoriaAAgregar->idProc = idProc;
					campoMemoriaAAgregar->paginaDelProceso = a;
					list_add(listaMemoria, campoMemoriaAAgregar);

					campoTablaDePag->idProc = campoMemoriaAAgregar->idProc;
					campoTablaDePag->idMarco = campoMemoriaAAgregar->idMarco;
					campoTablaDePag->paginaDelProceso =  a;
					list_add(listaTablaDePag,campoTablaDePag);

					// aca tengo que sacar el primero de la TLB y agregar el de ahora si es que la tlb llego a su maximo,
					// y sino simplemente agregarlo

					tamanioTLB = list_size(listaTLB);
					if (configuracion->tlbHabilitada) {
						if (tamanioTLB < configuracion->entradasTlb) {
							campoTLB->idMarco = campoMemoriaAAgregar->idMarco;
							campoTLB->idProc = campoMemoriaAAgregar->idProc;
							campoTLB->paginaDelProc = campoMemoriaAAgregar->paginaDelProceso;
							list_add(listaTLB, campoTLB);
						} else {
							list_remove(listaTLB, 0);
							campoTLB->idMarco = campoMemoriaAAgregar->idMarco;
							campoTLB->idProc = campoMemoriaAAgregar->idProc;
							campoTLB->paginaDelProc = campoMemoriaAAgregar->paginaDelProceso;
							list_add(listaTLB, campoTLB);
						}
					}


				} else { // aca si llego al maximo de marcos por proceso y tengo que sacar el primero ingresado de la tabla de pag de ese proceso (y fijarme
					     // si tambien de la tlb) y agregar el nuevo en la lista de memoria y su informacion en la lista de tabla de paginas y fijarme si tambien
					     // en la tlb
					if (campoMemoriaPrimerProceso->bitPagModificada == 1) { // si lo tengo modificada lo tengo que mandar a escribir al swap
						mandarASwapPorTenerBitModificada->Pag = a;
						mandarASwapPorTenerBitModificada->texto = campoMemoriaPrimerProceso->contenido;
						mandarASwapPorTenerBitModificada->idProc = campoMemoriaPrimerProceso->idProc;

						 PARTE DE ENVIAR A Swap UN Escribir UNA PAG
						 *char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
						 *  puts("Enviando \"mandar a swap un escribir una pag\" al Swap");
						 enviar(atoi(socketCPU), "mandar a swap un escribir una pag", strlen("mandar a swap un escribir una pag"));
						 puts("Enviado al Swap");


						 ahora tengo que sacar el primer proceso de la tabla de paginas, y si tengo la TLB habilitada,
						 *  fijarme si la info de ese marco esta tambien guardada en la tlb y sacarlo de ahi
						*

						tamanioTablaDePag = list_size(listaTablaDePag);
						for(d=0;d < tamanioTablaDePag && flagTablaDePag == 0;d++){
							primerCampoTablaDePagDelProceso = list_get(listaTablaDePag,d);
							if(primerCampoTablaDePagDelProceso->idMarco == campoMemoriaPrimerProceso->idMarco ){
								list_remove(listaTablaDePag,d);
								flagTablaDePag = 1;
							}
						}
						if(configuracion->tlbHabilitada){

						}

						list_remove(listaTablaDePag,0);



					}
				}

			}

		}

*/
	}





void finalizar(int idProc) {

}


int buscarSiEstaEnMemoria(int idProc, int nroPag) {
	int tamanioTLB, a, tamanioTablaPag;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	if (configuracion->tlbHabilitada == 1) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idProc == idProc && campoTLB->paginaDelProceso == nroPag /*que este en un marco*/) {
				return campoTLB->idMarco;
			}
		}
	}
	// sino veo si esta en la tabla de paginas

	tamanioTablaPag = list_size(listaTablaDePag);
	for (a = 0; a < tamanioTablaPag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc && campoTablaDePag->paginaDelProceso == nroPag) {
			return campoTablaDePag->idMarco;
		}
	}

	// si llego aca es porque no esta entonces devuelve - 1

	return -1;
}

void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido){
	int tamanioTLB, a, tamanioTablaPag,flagTLB = 0,flagTablaDePag = 0,tamanioMemoria,flagMemoria = 0;
		t_TLB * campoTLB;
		campoTLB = iniciarTLB();
		t_TablaDePaginas * campoTablaDePag;
		campoTablaDePag = iniciarTablaDePaginas();
		t_marco * campoMarco;
		campoMarco= iniciarMarco();


		if (configuracion->tlbHabilitada == 1) {
			tamanioTLB = list_size(listaTLB);
			for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
				campoTLB = list_get(listaTLB, a);
				if (campoTLB->idMarco == idMarco) {
					 campoTLB->bitPagModificada = 1;
					 flagTLB =1;
				}
			}
		}
		// sino veo si esta en la tabla de paginas

		tamanioTablaPag = list_size(listaTablaDePag);
		for (a = 0; a < tamanioTablaPag && flagTablaDePag == 0; a++) {
			campoTablaDePag = list_get(listaTablaDePag, a);
			if (campoTablaDePag->idMarco == idMarco) {
				campoTablaDePag->bitPagModificada = 1;
				flagTablaDePag = 1;
			}
		}
		tamanioMemoria= list_size(listaMemoria);
		for(a=0;a< tamanioMemoria && flagMemoria ==0;a++){
			campoMarco = list_get(listaMemoria,a);
			if(campoMarco->idMarco == idMarco){
				campoMarco->contenido = contenido;
				flagMemoria = 1;
			}
		}
			free(campoMarco);
			free(campoTLB);
			free(campoTablaDePag);
}


