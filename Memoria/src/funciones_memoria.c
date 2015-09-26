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

void* enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap){
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}



void cargarNuevoMarcoAMemoria(char* contenido){
	t_marco* campoAux;
	campoAux = iniciarMarco();

	variableIdMarco ++;

	campoAux->idMarco = variableIdMarco;
	campoAux->contenido = contenido;

	list_add(listaMemoria,campoAux);

	free(campoAux);
}

bool llegoAlMaximoDelProcesoLaMemoria(int idProc){
	bool respuesta;
	int tamanioMemoria,a,tamanioTablaDePag,contadorMarcosEnMemoria,flag = 0;
	tamanioMemoria = list_size(listaMemoria);
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	for(a=0;a<tamanioTablaDePag;a++){
		campoTablaDePag = list_get(listaTablaDePag,a);
		if(campoTablaDePag->idProc == idProc && campoTablaDePag->idMarco != -1 && flag == 0 ){ // o sea esta en memoria
			contadorMarcosEnMemoria ++;
			if(contadorMarcosEnMemoria == configuracion->cantidadMarcos ){
				flag =1;
			}
		}
	}

	if(flag == 0){
		respuesta = false;
	} else{
		respuesta = true;
	}


	return respuesta;
}

bool estaLlenaLaMemoria(){
	bool respuesta;
	int tamanioMemoria;
	tamanioMemoria = list_size(listaMemoria);

	if(tamanioMemoria <configuracion->cantidadMarcos){
		respuesta = false;
	} else {
		respuesta = true;
	}

	return respuesta;
}

void sacarAlPrimeroDeMemoriaDelProceso(int idProc){

}

void sacarAlPrimeroDeMemoria(){

}

char* traerContenidoDeMarco(int idMarco){
	char* contenido;

	return contenido;
}

void enviarACPUContenidoPaginaDeUnProceso(t_rtaLecturaCpu* lecturaMandarCpu){

}

t_lecturaSwap* traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag){



}
