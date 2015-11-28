#include "Memoria.h"

void reemplazar_tablaDePag(int index,t_TablaDePaginas* campoTablaDePag){
	t_TablaDePaginas * campoTablaDePagReemplazar;
	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePagReemplazar=campoTablaDePag;
		printf("\n indice %i \n",index);
	list_replace(listaTablaDePag, index, campoTablaDePagReemplazar);
}


t_marco_y_bit* buscarSiEstaEnMemoria(int idProc, int nroPag) {
	//warning no usa variable, se debe cual es el uso de flagTDP
	int tamanioTLB, a, tamanioTablaPag, flagTLB = 0, flagTDP = 0;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();
	pthread_mutex_lock(&mutexListaTLB);
	if (configuracion->tlbHabilitada == 1) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idProc == idProc && campoTLB->paginaDelProceso == nroPag /*que este en un marco*/) {
				marcoYBit->idMarco = campoTLB->idMarco;
				marcoYBit->bitPresencia = campoTLB->bitPresencia;
				flagTLB = 1;
			}
		}
	}
	pthread_mutex_unlock(&mutexListaTLB);
	// sino veo si esta en la tabla de paginas
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaPag = list_size(listaTablaDePag);
	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaPag && flagTDP == 0 && flagTLB == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc && campoTablaDePag->paginaDelProceso == nroPag) {
			marcoYBit->idMarco = campoTablaDePag->idMarco;
			marcoYBit->bitPresencia = campoTablaDePag->bitPresencia;
			flagTDP = 1;
			//LOG
			char* textoLogger = string_new();
					string_append(&textoLogger,string_from_format("Acceso a Tabla de paginas, PID: %i, N° de"
								" página: %i y N° de marco: %i", idProc, nroPag, campoTablaDePag->idMarco));
						my_log_info(textoLogger);
		}
	}
	pthread_mutex_unlock(&mutexTablaPags);
	//LOG
		if (aux == 0) {
			if (flagTLB == 1) {
				char* textoLogger = string_new();
						string_append(&textoLogger,string_from_format("Solicitud de escritura recibida, PID: %i , N° de página: %i, TLB hit, N° de marco resultante: %i", idProc,
						nroPag, marcoYBit->idMarco));
				my_log_info(textoLogger);
			} else {
				char* textoLogger = string_new();
				string_append(&textoLogger , string_from_format("Solicitud de escritura recibida, PID: %i , N° de página: %i, TLB miss, N° de marco resultante: %i", idProc,
						nroPag, marcoYBit->idMarco));
				my_log_info(textoLogger);
			}
		} else {
			if (flagTLB == 1) {
				char* textoLogger = string_new();
				string_append(&textoLogger ,string_from_format("Solicitud de lectura recibida, PID: %i , N° de página: %i, TLB hit, N° de marco resultante: %i", idProc,
						nroPag, marcoYBit->idMarco));
				my_log_info(textoLogger);
			} else {
				char* textoLogger = string_new();
				string_append(&textoLogger, string_from_format("Solicitud de lectura recibida, PID: %i , N° de página: %i, TLB miss, N° de marco resultante: %i", idProc,
						nroPag, marcoYBit->idMarco));
				my_log_info(textoLogger);
			}
		}
	return marcoYBit;

}

void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido) {
	int tamanioTLB, a, tamanioTablaPag, flagTLB = 0, flagTablaDePag = 0, tamanioMemoria, flagMemoria = 0;
	t_TLB * campoTLB;

	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;

	campoTablaDePag = iniciarTablaDePaginas();
	t_marco * campoMarco;
	t_marco * campoMarcoReemplazar;
	t_TLB* campoTLBReemplazar;
	campoMarco = iniciarMarco();
	pthread_mutex_lock(&mutexListaTLB);
	if (configuracion->tlbHabilitada == 1) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idMarco == idMarco) {
				campoTLBReemplazar = iniciarTLB();
				campoTLBReemplazar = campoTLB;
				campoTLBReemplazar->bitPagModificada = 1;
				list_replace(listaTLB, a, campoTLBReemplazar);
				flagTLB = 1;
			}
		}
	}
	pthread_mutex_unlock(&mutexListaTLB);
	// veo si esta en la tabla de paginas y la modifico ( por mas que este en la TLB, tambien tengo que modificarlo
	// en la Tabla de Pag)
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaPag = list_size(listaTablaDePag);
	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaPag && flagTablaDePag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == idMarco) {
			campoTablaDePag->bitPagModificada = 1;
			reemplazar_tablaDePag(a,campoTablaDePag);
			flagTablaDePag = 1;
		}
	}
	pthread_mutex_unlock(&mutexTablaPags);
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);
	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioMemoria && flagMemoria == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->idMarco == idMarco) {
			campoMarcoReemplazar = iniciarMarco();
			campoMarcoReemplazar = campoMarco;
			campoMarcoReemplazar->contenido = contenido;
			variableEnvejecimientoMarco++;
			campoMarcoReemplazar->posicion = variableEnvejecimientoMarco;
			list_replace(listaMemoria, a, campoMarcoReemplazar);
			flagMemoria = 1;
		}
	}
	pthread_mutex_unlock(&mutexListaMemoria);
}

void cargarNuevoMarcoAMemoria(char* contenido, int PID, int pag, int flagEscritura) {
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	int tamanioTablaDePag, a, flag = 0;
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaDePag = list_size(listaTablaDePag);

	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaDePag && flag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == PID && campoTablaDePag->paginaDelProceso == pag) {
			campoAux->idMarco = campoTablaDePag->idMarco;
			campoTablaDePag->bitPresencia = 1;
			if(flagEscritura == 1){
				campoTablaDePag->bitPagModificada = 1;

			}
			reemplazar_tablaDePag(a,campoTablaDePag);
			flag = 1;
		}
	}
	pthread_mutex_unlock(&mutexTablaPags);


	campoAux->contenido = string_new();
	string_append(&campoAux->contenido, contenido);
	variableEnvejecimientoMarco++;
	campoAux->posicion = variableEnvejecimientoMarco;
	variableParaFifo ++;
	campoAux->posicionCargadoAMemoria =variableParaFifo;

	if (configuracion->tlbHabilitada == 1) {
		cargarNuevoEnTLB(PID, pag, campoAux->idMarco);
	}

	if (flagEscritura == 1) {
		campoAux->bitUso = 1;
		campoAux->bitModificada = 1;
	} else {
		campoAux->bitUso = 1;
		campoAux->bitModificada = 0;
	}

	pthread_mutex_lock(&mutexListaMemoria);
	list_add(listaMemoria, campoAux);
	pthread_mutex_unlock(&mutexListaMemoria);

}

void cargarNuevoEnTLB(int PID, int pag, int id) {
	int tamanioTLB;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();

	pthread_mutex_lock(&mutexListaTLB);
	tamanioTLB = list_size(listaTLB);
	pthread_mutex_unlock(&mutexListaTLB);
	if (tamanioTLB == configuracion->entradasTlb) {
		sacarAlPrimeroDeTLB();
	}

	variableTLB++;
	campoTLB->bitPagModificada = 0;
	campoTLB->idMarco = id;
	campoTLB->idProc = PID;
	campoTLB->paginaDelProceso = pag;
	campoTLB->posicion = variableTLB;
	campoTLB->bitPresencia = 1;

	pthread_mutex_lock(&mutexListaTLB);
	list_add(listaTLB, campoTLB);
	pthread_mutex_unlock(&mutexListaTLB);
}

void sacarAlPrimeroDeTLB() {
	int tamanioTLB, a, posicionEnLista;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TLB * campoMasViejoTLB;
	campoMasViejoTLB = iniciarTLB();

	pthread_mutex_lock(&mutexListaTLB);
	tamanioTLB = list_size(listaTLB);

	for (a = 0; a < tamanioTLB; a++) {
		campoTLB = list_get(listaTLB, a);
		if (a == 0) {
			campoMasViejoTLB = campoTLB;
			posicionEnLista = a;
		} else {
			if (campoTLB->posicion < campoMasViejoTLB->posicion) {
				posicionEnLista = a;
			}
		}
	}
	list_remove(listaTLB, posicionEnLista);
	pthread_mutex_unlock(&mutexListaTLB);

}

bool llegoAlMaximoDelProcesoLaMemoria(int idProc) {
	bool respuesta;

	int a, tamanioTablaDePag, contadorMarcosEnMemoria = 0, flag = 0;
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc && campoTablaDePag->bitPresencia == 1 && flag == 0) { // o sea esta en memoria
			contadorMarcosEnMemoria++;
			if (contadorMarcosEnMemoria == configuracion->maximosMarcosPorProceso) {
				flag = 1;
			}
		}
	}

	pthread_mutex_unlock(&mutexTablaPags);
	if (flag == 0) {
		respuesta = false;
	} else {
		respuesta = true;
	}

	return respuesta;
}

void sacaProcesoDeMemoriaSegunFifo(char* contenidoACargar, int PIDACargar, int pagACargar, int flagEscritura, int socketSwap) {

	// busco todos los id de un proceso, luego el menor va a ser el mas viejo
	int a, tamanioListaMarco, primero = 0;
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_list* listaMarco;
	listaMarco = list_create();

	pthread_mutex_lock(&mutexListaMemoria);

	listaMarco = buscarLosMarcosDeProcesoEnMemoria(PIDACargar);

	tamanioListaMarco = list_size(listaMarco);
	for (a = 0; a < tamanioListaMarco; a++) {
		campoMarco = list_get(listaMarco, a);
		if (primero == 0) {
			primero++;
			campoAux = campoMarco;
		} else {
			if (campoMarco->posicionCargadoAMemoria < campoAux->posicionCargadoAMemoria) {
				campoAux = campoMarco;
			}
		}

	}
	pthread_mutex_unlock(&mutexListaMemoria);

	verificarBitDeModificada(campoAux, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);

}

void sacarDeMemoriaSegunFifo(int socketSwap, int PIDACargar, char* contenidoACargar, int pagACargar, int flagEscritura) {
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	int tamanioMemoria, a, primero = 0;

	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);
	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioMemoria; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (primero == 0) {
			primero++;
			campoAux = campoMarco;
		} else {
			if (campoMarco->posicionCargadoAMemoria < campoAux->posicionCargadoAMemoria) {
				campoAux = campoMarco;
			}
		}

	}
	pthread_mutex_unlock(&mutexListaMemoria);


	verificarBitDeModificada(campoAux, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);

}



t_marco_con_flag* buscarModificadaYUsoEnCeroDeProceso(int PID) {
	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();
	t_marco_con_indice* marcoYIndice;
	marcoYIndice = iniciarMarcoYIndice();
	int a, tamanioMarcosDelProceso, flagReemplazo = 0;
	int* indice;
	t_list* listaMarcoYIndices;
	listaMarcoYIndices = list_create();
	listaMarcoYIndices = buscarLosMarcosDeProcesoEnMemoriaConSusIndices(PID);
	tamanioMarcosDelProceso = list_size(listaMarcoYIndices);
	indice = list_get(listaIndices, PID);

	sleep(configuracion->retardoMemoria); // este sleep vale por este for y por el de abajo,
									  // si no se entiende por que, preguntarle a los matis

	for (a = *indice; a < tamanioMarcosDelProceso && flagReemplazo == 0; a++) {
		marcoYIndice = list_get(listaMarcoYIndices, a);
		if (marcoYIndice->marco->bitModificada == 0 && marcoYIndice->marco->bitUso == 0) {
			flagReemplazo = 1;
			*indice = a ;
		}
	}
	for (a = 0; a < *indice && flagReemplazo == 0; a++) {
		marcoYIndice = list_get(listaMarcoYIndices, a);
		if (marcoYIndice->marco->bitModificada == 0 && marcoYIndice->marco->bitUso == 0) {
			flagReemplazo = 1;
			*indice = a;
		}
	}
	marcoYFlag->flag = flagReemplazo;

	if (marcoYFlag->flag == 1) {
		marcoYFlag->marco = marcoYIndice->marco;
	}



	list_replace(listaIndices, PID, indice);

	return marcoYFlag;
}

t_marco_con_flag* buscarUsoEnCeroModificadaEnUnoDeProceso(int PID) {
	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();
	int a, tamanioMarcosDelProceso, flagReemplazo = 0;
	int* indice = malloc(sizeof(int));
	t_list* listaMarcoYIndices;
	listaMarcoYIndices = list_create();
	listaMarcoYIndices = buscarLosMarcosDeProcesoEnMemoriaConSusIndices(PID);


	tamanioMarcosDelProceso = list_size(listaMarcoYIndices);
	t_marco_con_indice* marcoYIndice;
	t_marco_con_indice* marcoYIndiceReemplazar;
	marcoYIndice = iniciarMarcoYIndice();
	indice = list_get(listaIndices, PID);

	sleep(configuracion->retardoMemoria); // este sleep vale por este for y por el de abajo,
										  // si no se entiende por que, preguntarle a los matis
	for (a = *indice; a < tamanioMarcosDelProceso && flagReemplazo == 0; a++) {
		marcoYIndice = list_get(listaMarcoYIndices, a);
		if (marcoYIndice->marco->bitModificada == 1 && marcoYIndice->marco->bitUso == 0) {
			flagReemplazo = 1;
			*indice = a ;
		} else {
			marcoYIndiceReemplazar = iniciarMarcoYIndice();
			marcoYIndiceReemplazar = marcoYIndice;
			marcoYIndiceReemplazar->marco->bitUso = 0;
			list_replace(listaMemoria, marcoYIndiceReemplazar->indice, marcoYIndiceReemplazar->marco);
		}
	}
	for (a = 0; a < *indice && flagReemplazo == 0; a++) {
		marcoYIndice = list_get(listaMarcoYIndices, a);
		if (marcoYIndice->marco->bitModificada == 1 && marcoYIndice->marco->bitUso == 0) {
			flagReemplazo = 1;
			*indice = a ;
		} else {
			marcoYIndiceReemplazar = iniciarMarcoYIndice();
			marcoYIndiceReemplazar = marcoYIndice;
			marcoYIndiceReemplazar->marco->bitUso = 0;
			list_replace(listaMemoria, marcoYIndiceReemplazar->indice, marcoYIndiceReemplazar->marco);		}
	}
	marcoYFlag->flag = flagReemplazo;

	if (marcoYFlag->flag == 1) {
		marcoYFlag->marco = marcoYIndice->marco;
	}


	list_replace(listaIndices, PID, indice);

	return marcoYFlag;
}

void sacaProcesoDeMemoriaSegunClockModificado(char* contenidoACargar, int PIDACargar, int pagACargar, int flagEscritura, int socketSwap) {

	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();

	marcoYFlag = buscarModificadaYUsoEnCeroDeProceso(PIDACargar);
	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUnoDeProceso(PIDACargar);
	}
	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarModificadaYUsoEnCeroDeProceso(PIDACargar);
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUnoDeProceso(PIDACargar);
	}

	verificarBitDeModificada(marcoYFlag->marco, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);
	//LOG
			t_TablaDePaginas* campoTablaDePag;
			campoTablaDePag = iniciarTablaDePaginas();
			int pagVieja,a;
			for(a= 0; a< list_size(listaTablaDePag); a++){
				campoTablaDePag = list_get(listaTablaDePag,a);
				if(campoTablaDePag->idMarco == marcoYFlag->marco->idMarco){
					pagVieja = campoTablaDePag->paginaDelProceso;
					a = list_size(listaTablaDePag)+1;
				}
			}
			char* textoLogger = string_new();
			string_append(&textoLogger,string_from_format("Acceso a swap (fallo de página), PID: %i, pagina a reemplazar: %i , pagina nueva: %i, marco: %i",PIDACargar,pagVieja,pagACargar,marcoYFlag->marco->idMarco));
			my_log_info(textoLogger);



}

bool estaLlenaLaMemoria() {
	bool respuesta;
	int tamanioMemoria;
	//pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);
	//pthread_mutex_unlock(&mutexListaMemoria);

	if (tamanioMemoria < configuracion->cantidadMarcos) {
		respuesta = false;
	} else {
		respuesta = true;
	}

	return respuesta;
}

void sacarAlMasViejoUsadoDelProcesoDeMemoria(char* contenidoACargar, int PIDACargar, int pagACargar, int flagEscritura, int socketSwap) {

	// busco todos los id de un proceso, luego el menor va a ser el mas viejo
	int a, tamanioListaMarco, primero = 0;
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_list* listaMarco;
	listaMarco = list_create();

	pthread_mutex_lock(&mutexListaMemoria);

	listaMarco = buscarLosMarcosDeProcesoEnMemoria(PIDACargar);

	tamanioListaMarco = list_size(listaMarco);
	for (a = 0; a < tamanioListaMarco; a++) {
		campoMarco = list_get(listaMarco, a);
		if (primero == 0) {
			primero++;
			campoAux = campoMarco;
		} else {
			if (campoMarco->posicion < campoAux->posicion) {
				campoAux = campoMarco;
			}
		}

	}
	pthread_mutex_unlock(&mutexListaMemoria);
	verificarBitDeModificada(campoAux, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);
	//LOG
		t_TablaDePaginas* campoTablaDePag;
		campoTablaDePag = iniciarTablaDePaginas();
		int pagVieja;
		for(a= 0; a< list_size(listaTablaDePag); a++){
			campoTablaDePag = list_get(listaTablaDePag,a);
			if(campoTablaDePag->idMarco == campoAux->idMarco){
				pagVieja = campoTablaDePag->paginaDelProceso;
				a = list_size(listaTablaDePag)+1;
			}
		}
		char* textoLogger = string_new();
				string_append(&textoLogger,string_from_format("Acceso a swap (fallo de página), PID: %i, pagina a reemplazar: %i , pagina nueva: %i, marco: %i",PIDACargar,pagVieja,pagACargar,campoAux->idMarco));
		my_log_info(textoLogger);


}

void sacarAlMasViejoUsadoDeMemoria(int socketSwap, int PIDACargar, char* contenidoACargar, int pagACargar, int flagEscritura) {
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	int tamanioMemoria, a, primero = 0;
//warning no se usan variables, entonces lo comento
//	int nuevoId,idMenor, id;
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);
	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioMemoria; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (primero == 0) {
			primero++;
			campoAux = campoMarco;
		} else {
			if (campoMarco->posicion < campoAux->posicion) {
				campoAux = campoMarco;
			}
		}

	}
	pthread_mutex_unlock(&mutexListaMemoria);


	verificarBitDeModificada(campoAux, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);
	//LOG
			t_TablaDePaginas* campoTablaDePag;
			campoTablaDePag = iniciarTablaDePaginas();
			int pagVieja;
			for(a= 0; a< list_size(listaTablaDePag); a++){
				campoTablaDePag = list_get(listaTablaDePag,a);
				if(campoTablaDePag->idMarco == campoAux->idMarco){
					pagVieja = campoTablaDePag->paginaDelProceso;
					a = list_size(listaTablaDePag)+1;
				}
			}
			char* textoLogger = string_new();
					string_append(&textoLogger,string_from_format("Acceso a swap (fallo de página), PID: %i, pagina a reemplazar: %i , pagina nueva: %i, marco: %i",PIDACargar,pagVieja,pagACargar,campoAux->idMarco));
			my_log_info(textoLogger);

}

t_marco_con_flag* buscarModificadaYUsoEnCero() {
	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	int a, tamanioMemoria, flagReemplazo = 0;
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);

	usleep(configuracion->retardoMemoria * 1000); // este usleep vale por este for y por el de abajo,
	// si no se entiende por que, preguntarle a los matis
	for (a = indiceClockM; a < tamanioMemoria && flagReemplazo == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->bitModificada == 0 && campoMarco->bitUso == 0) {
			flagReemplazo = 1;
			indiceClockM = a ;
		}
	}

	for (a = 0; a < indiceClockM && flagReemplazo == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->bitModificada == 0 && campoMarco->bitUso == 0) {
			flagReemplazo = 1;
			indiceClockM = a ;
		}
	}
	pthread_mutex_unlock(&mutexListaMemoria);

	marcoYFlag->flag = flagReemplazo;

	if (marcoYFlag->flag == 1) {
		marcoYFlag->marco = campoMarco;
	}

	return marcoYFlag;
}

t_marco_con_flag* buscarUsoEnCeroModificadaEnUno() {
	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();
	t_marco* campoMarco;
	t_marco* campoMarcoReemplazar;
	campoMarco = iniciarMarco();
	int a, tamanioMemoria, flagReemplazo = 0;
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);

	usleep(configuracion->retardoMemoria * 1000); // este usleep vale por este for y por el de abajo,
	// si no se entiende por que, preguntarle a los matis
	for (a = indiceClockM; a < tamanioMemoria && flagReemplazo == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->bitModificada == 1 && campoMarco->bitUso == 0) {
			flagReemplazo = 1;
			indiceClockM = a ;
		} else {
			campoMarcoReemplazar = iniciarMarco();
			campoMarcoReemplazar = campoMarco;
			campoMarcoReemplazar->bitUso = 0;
			list_replace(listaMemoria, a, campoMarcoReemplazar);
		}
	}

	for (a = 0; a < indiceClockM && flagReemplazo == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->bitModificada == 1 && campoMarco->bitUso == 0) {
			flagReemplazo = 1;
			indiceClockM = a ;
		} else {
			campoMarcoReemplazar = iniciarMarco();
			campoMarcoReemplazar = campoMarco;
			campoMarcoReemplazar->bitUso = 0;
			list_replace(listaMemoria, a, campoMarcoReemplazar);
		}
	}
	pthread_mutex_unlock(&mutexListaMemoria);
	marcoYFlag->flag = flagReemplazo;

	if (marcoYFlag->flag == 1) {
		marcoYFlag->marco = campoMarco;
	}

	return marcoYFlag;
}

void sacarDeMemoriaSegunClockModificado(int socketSwap, int PIDACargar, char* contenidoACargar, int pagACargar, int flagEscritura) {

	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();

	marcoYFlag = buscarModificadaYUsoEnCero();

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUno();
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarModificadaYUsoEnCero();
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUno();
	}

	verificarBitDeModificada(marcoYFlag->marco, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);
	//LOG
			t_TablaDePaginas* campoTablaDePag;
			campoTablaDePag = iniciarTablaDePaginas();
			int pagVieja,a;
			for(a= 0; a< list_size(listaTablaDePag); a++){
				campoTablaDePag = list_get(listaTablaDePag,a);
				if(campoTablaDePag->idMarco == marcoYFlag->marco->idMarco){
					pagVieja = campoTablaDePag->paginaDelProceso;
					a = list_size(listaTablaDePag)+1;
				}
			}
			char* textoLogger = string_new();
				string_append(&textoLogger,	string_from_format("Acceso a swap (fallo de página), PID: %i, pagina a reemplazar: %i , pagina nueva: %i, marco: %i",PIDACargar,pagVieja,pagACargar,marcoYFlag->marco->idMarco));
			my_log_info(textoLogger);
}

void verificarBitDeModificada(t_marco* campoMarco, char* contenidoACargar, int PIDaCargar, int pagACargar, int flagEscritura, int socketSwap) {
	/* solo se usa en las funciones de sacar a un marco de memoria entonces
	 en la respuesta de sobreescribir a swap, se va a mandar a cpu el contenido,
	 que es lo que se manda en el caso que no haya que sacar alguno */


	int tamanioTLB, tamanioTablaDePag, a, flagTLB = 0, flagTablaDePag=0, pagina, idProc;
	int bitTLB = 0, bitTablaDePag = 0,id;
	pthread_mutex_lock(&mutexListaTLB);
	tamanioTLB = list_size(listaTLB);
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	char* contenido = malloc(sizeof(char));

	if (configuracion->tlbHabilitada == 1) {
		for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idMarco == campoMarco->idMarco) {
				flagTLB = 1;
				bitTLB = campoTLB->bitPagModificada;
				if (bitTLB == 1) {
					pagina = campoTLB->paginaDelProceso;
					idProc = campoTLB->idProc;
					list_remove(listaTLB,a);
				}
			}
		}

	}

		for (a = 0; a < tamanioTablaDePag && flagTablaDePag == 0 && flagTLB == 0; a++) {
			campoTablaDePag = list_get(listaTablaDePag, a);
			if (campoTablaDePag->idMarco == campoMarco->idMarco) {

				flagTablaDePag = 1;
				bitTablaDePag = campoTablaDePag->bitPagModificada;
				campoTablaDePag->bitPresencia = 0;
				if (bitTablaDePag == 1) {
					campoTablaDePag->bitPagModificada = 0;
					reemplazar_tablaDePag(a,campoTablaDePag);
					pagina = campoTablaDePag->paginaDelProceso;
					idProc = campoTablaDePag->idProc;
				}
			}
		}
	id = campoMarco->idMarco;
	contenido = campoMarco->contenido ;


	pthread_mutex_unlock(&mutexTablaPags);
	pthread_mutex_unlock(&mutexListaTLB);
	eliminarDeMemoria(id);
	cargarNuevoMarcoAMemoria(contenidoACargar, PIDaCargar, pagACargar, flagEscritura);

	if (bitTablaDePag == 1 || bitTLB == 1) {
		if (flagEscritura == 0) { // por leer
			usleep(configuracion->retardoMemoria * 1000);
			enviarASwapContenidoPaginaDesactualizada(idProc, pagina, contenido, socketSwap);
		} else { // por escribir
			t_contenido_pagina * escrituraSwap;
			escrituraSwap = iniciarContenidoPagina();
			escrituraSwap->PID = idProc;
			escrituraSwap->contenido = contenido;
			escrituraSwap->numeroPagina = pagina;
			enviarEscribirAlSwap(escrituraSwap, socketSwap);
		}

	}

}
char* traerContenidoDeMarco(int idMarco) {
	char* contenido;
	int tamanioMemoria, a, flag = 0;
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);
	t_marco* campoMemoria;
	campoMemoria = iniciarMarco();

	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioMemoria && flag == 0; a++) {
		campoMemoria = list_get(listaMemoria, a);
		if (campoMemoria->idMarco == idMarco) {
			variableEnvejecimientoMarco++;
			campoMemoria->posicion = variableEnvejecimientoMarco;
			flag = 1;
		}
	}
	pthread_mutex_unlock(&mutexListaMemoria);
	contenido = campoMemoria->contenido;

	return contenido;
}

t_list* buscarLosMarcoYBitDeProceso(int idProc) {
	int a, tamanioTablaDePag;
	t_list* listamarcoYBit;
	listamarcoYBit = list_create();
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco_y_bit* marcoYBit;

	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);

		if (campoTablaDePag->idProc == idProc) {
			marcoYBit = iniciarMarcoYBit();
			marcoYBit->idMarco = campoTablaDePag->idMarco;
			marcoYBit->bitPresencia = campoTablaDePag->bitPresencia;
			list_add(listamarcoYBit, marcoYBit);
		}
	}
	pthread_mutex_unlock(&mutexTablaPags);
	return listamarcoYBit;

}

t_list* buscarLosMarcosDeProcesoEnMemoria(int PID) {
	int a, b, tamanioListaMarcoYBit, tamanioListaMarcos, flag;
	t_list* listaMarcos;
	listaMarcos = list_create();
	t_list* listaMarcoYBit;
	listaMarcoYBit = list_create();
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	listaMarcoYBit = buscarLosMarcoYBitDeProceso(PID);

	tamanioListaMarcoYBit = list_size(listaMarcoYBit);

	tamanioListaMarcos = list_size(listaMemoria);
	for (a = 0; a < tamanioListaMarcoYBit; a++) {
		marcoYBit = list_get(listaMarcoYBit, a);
		flag = 0;
		usleep(configuracion->retardoMemoria * 1000);
		for (b = 0; b < tamanioListaMarcos && flag == 0; b++) {
			campoMarco = list_get(listaMemoria, b);
			if (campoMarco->idMarco == marcoYBit->idMarco && marcoYBit->bitPresencia == 1) {
				list_add(listaMarcos, campoMarco);
				flag = 1;
			}
		}

	}
	return listaMarcos;
}

t_list* buscarLosMarcosDeProcesoEnMemoriaConSusIndices(int PID) {
	int a, b, tamanioListaMarcoYBit, tamanioListaMarcos, flag;
	t_list* listaMarcosYIndices;
	listaMarcosYIndices = list_create();
	t_list* listaMarcoYBit;
	listaMarcoYBit = list_create();
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();
	t_marco_con_indice* marcoYIndice;
	marcoYIndice = iniciarMarcoYIndice();

	listaMarcoYBit = buscarLosMarcoYBitDeProceso(PID);
	tamanioListaMarcoYBit = list_size(listaMarcoYBit);
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioListaMarcos = list_size(listaMemoria);
	for (a = 0; a < tamanioListaMarcoYBit; a++) {
		marcoYBit = list_get(listaMarcoYBit, a);
		flag = 0;
		sleep(configuracion->retardoMemoria);
		for (b = 0; b < tamanioListaMarcos && flag == 0; b++) {
			campoMarco = list_get(listaMemoria, b);
			if (campoMarco->idMarco == marcoYBit->idMarco && marcoYBit->bitPresencia == 1) {
				marcoYIndice = iniciarMarcoYIndice();
				marcoYIndice->marco = campoMarco;
				marcoYIndice->indice = b;
				list_add(listaMarcosYIndices, marcoYIndice);
				flag = 1;
			}
		}

	}
	pthread_mutex_unlock(&mutexListaMemoria);

	return listaMarcosYIndices;
}

void eliminarDeMemoria(int id) {
	int a, tamanioMemoria, flag = 0;
	pthread_mutex_lock(&mutexListaMemoria);
	tamanioMemoria = list_size(listaMemoria);
	t_marco* campoMarco;
	campoMarco = iniciarMarco();

	for (a = 0; a < tamanioMemoria && flag == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->idMarco == id) {
			list_remove(listaMemoria, a);
			flag = 1;
		}
	}
	pthread_mutex_unlock(&mutexListaMemoria);
}

void eliminarDeTablaDePaginas(int id) {
	int a, tamanioTablaDePaginas, flag = 0;
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaDePaginas = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaDePaginas && flag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == id) {
			campoTablaDePag->bitPresencia = 0;
			reemplazar_tablaDePag(a,campoTablaDePag);
			flag = 1;
		}
	}
	pthread_mutex_unlock(&mutexTablaPags);
}

void eliminarDeTLB(int idMenor) {
	int a, tamanioTLB, flag = 0;
	pthread_mutex_lock(&mutexListaTLB);
	tamanioTLB = list_size(listaTLB);
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();

	for (a = 0; a < tamanioTLB && flag == 0; a++) {
		campoTLB = list_get(listaTLB, a);
		if (campoTLB->idMarco == idMenor) {
			campoTLB->bitPresencia = 0;
			flag = 1;
		}
	}
	pthread_mutex_unlock(&mutexListaTLB);

}

void eliminarDeTLBDefinitivamente(int id) {
	int a, tamanioTLB, flag = 0;
	pthread_mutex_lock(&mutexListaTLB);
	tamanioTLB = list_size(listaTLB);
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();

	for (a = 0; a < tamanioTLB && flag == 0; a++) {
		campoTLB = list_get(listaTLB, a);
		if (campoTLB->idMarco == id) {
			list_remove(listaTLB, a);
			flag = 1;
		}
	}
	pthread_mutex_unlock(&mutexListaTLB);
}

void eliminarDeTablaDePaginasDefinitivamente(int PID) {
	int a, tamanioTablaDePaginas;
	pthread_mutex_lock(&mutexTablaPags);
	tamanioTablaDePaginas = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	usleep(configuracion->retardoMemoria * 1000);
	for (a = 0; a < tamanioTablaDePaginas; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == PID) {
			list_remove(listaTablaDePag, a);
			a--;
			tamanioTablaDePaginas--;
		}
	}
	pthread_mutex_unlock(&mutexTablaPags);

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void respuestaTraerDeSwapUnaPaginaDeUnProceso(int idProc, int pag, char* contenido, int flagEscritura, int socketCPU, int socketSwap) {

	char* algoritmoLRU = string_new();
	string_append(&algoritmoLRU, "LRU");
	char* algoritmoCLOCK = string_new();
	string_append(&algoritmoCLOCK, "CLOCK");
	char* algoritmoFIFO = string_new();
	string_append(&algoritmoFIFO, "FIFO");
	t_contenido_pagina* lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	int flagSaco=0;

	if (strcmp(configuracion->algoritmo_reemplazo, algoritmoLRU) == 0) {

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacarAlMasViejoUsadoDelProcesoDeMemoria(contenido, idProc, pag, flagEscritura, socketSwap);
			flagSaco = 1;
		} else if (estaLlenaLaMemoria()) {
			sacarAlMasViejoUsadoDeMemoria(socketSwap, idProc, contenido, pag, flagEscritura);
			flagSaco = 1;
		}

	} else if(strcmp(configuracion->algoritmo_reemplazo, algoritmoCLOCK) == 0) {

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacaProcesoDeMemoriaSegunClockModificado(contenido, idProc, pag, flagEscritura, socketSwap);
			flagSaco = 1;
		} else if (estaLlenaLaMemoria()) {
			sacarDeMemoriaSegunClockModificado(socketSwap, idProc, contenido, pag, flagEscritura);
			flagSaco = 1;
		}
	} else if(strcmp(configuracion->algoritmo_reemplazo, algoritmoFIFO) == 0) {

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) {
			sacaProcesoDeMemoriaSegunFifo(contenido, idProc, pag, flagEscritura, socketSwap);
			flagSaco = 1;
		} else if (estaLlenaLaMemoria()) {
			sacarDeMemoriaSegunFifo(socketSwap, idProc, contenido, pag, flagEscritura);
			flagSaco = 1;
		}
	}

	// aca significa que no tuvo que sacar ninguno
	if(flagSaco == 0){
	cargarNuevoMarcoAMemoria(contenido, idProc, pag, flagEscritura);
	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;
	string_append(&lecturaMandarCpu->contenido, contenido);


	if (flagEscritura == 0) {
		enviarACPUContenidoPaginaDeUnProcesoPorLeer(lecturaMandarCpu, socketCPU);
	} else { // por escribir
		t_contenido_pagina * escrituraSwap;
		escrituraSwap = iniciarContenidoPagina();
		escrituraSwap->PID = lecturaMandarCpu->PID;
		string_append(&escrituraSwap->contenido, lecturaMandarCpu->contenido);
		escrituraSwap->numeroPagina = lecturaMandarCpu->numeroPagina;
		enviarRtaEscribirACPU(escrituraSwap, socketCPU);
	}
	}

}

void enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap) {
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}

void enviarFinalizarASwap(t_PID *estructura, int socketSwap) {
	enviarStruct(socketSwap, FIN_PROCESO_SWAP, estructura);
}

void enviarASwapContenidoPaginaDesactualizada(int idProc, int pagina, char* contenido, int socketSwap) {
	t_contenido_pagina* estructura;
	estructura = iniciarContenidoPagina();
	estructura->PID = idProc;
	estructura->numeroPagina = pagina;
	estructura->contenido = contenido;
	enviarStruct(socketSwap, SOBREESCRIBIR_SWAP, estructura);
}

void enviarACPUContenidoPaginaDeUnProcesoPorLeer(t_contenido_pagina* lecturaMandarCpu, int socketCPU) {
	enviarStruct(socketCPU, RESUL_LEER_OK_CPU, lecturaMandarCpu);
}

void traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag, int socketSwap) {
	t_leerDeProceso* estructura;
	estructura = crearEstructuraLeer();
	estructura->PID = idProc;
	estructura->numeroPaginaFin = nroDePag;
	estructura->numeroPaginaInicio = nroDePag;
	enviarStruct(socketSwap, LEER_SWAP, estructura);
}
void traerDeSwapUnaPaginaDeUnProcesoPorEscribir(int idProc, int nroPag, char* textoAEscribir, int socketSwap) {
	t_leerDeProcesoPorEscribir* estructura;
	estructura = crearEstructuraLeerProcesoPorEscribir();
	estructura->PID = idProc;
	estructura->numeroPaginaFin = nroPag;
	estructura->numeroPaginaInicio = nroPag;
	string_append(&estructura->textoAEscribir, textoAEscribir);
	enviarStruct(socketSwap, LEER_SWAP_POR_ESCRIBIR, estructura);
}

void enviarFinalizarACPU(t_PID* estructuraFinalizar, int socketCPU) {
	enviarStruct(socketCPU, RESUL_FIN, estructuraFinalizar);
}

void inicializacionDesdeCero() {

	listaMemoria = list_create();
	listaTLB = list_create();
	listaTablaDePag = list_create();
	listaIndices = list_create();
	variableIdMarco = 0;
	variableTLB = 0;
	variableEnvejecimientoMarco = 0;
	indiceClockM = 0;
	variableParaFifo = 0;

}

void iniciarConfiguracionTLBNoHabilitada() {
	configuracion->tlbHabilitada = 0;
}

void iniciarConfiguracionTLBHabilitada() {
	configuracion->tlbHabilitada = 1;
}


void hardcodearValoresEnTLB(int PID, int id, int pag) {
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();

	campoTLB->idProc = PID;
	campoTLB->idMarco = id;
	campoTLB->paginaDelProceso = pag;
	campoTLB->bitPagModificada = 0;
	campoTLB->bitPresencia = 0;
	campoTLB->posicion = 0;
	pthread_mutex_lock(&mutexListaTLB);
	list_add(listaTLB, campoTLB);
	pthread_mutex_unlock(&mutexListaTLB);
}

t_PID* iniciar_falso(int idProc, int cantPag, int socketCPU) {
	int contador;
	t_TablaDePaginas* tablaDePag;
	t_PID * estructuraEnvio;
	estructuraEnvio = crearPID();

	for (contador = 0; contador < cantPag; contador++) {

		tablaDePag = iniciarTablaDePaginas();
		tablaDePag->idProc = idProc;
		tablaDePag->paginaDelProceso = contador;
		tablaDePag->idMarco = variableIdMarco; // porque no esta en algun marco en mem pcpal
		tablaDePag->bitPagModificada = 0;
		tablaDePag->bitPresencia = 0;
		pthread_mutex_lock(&mutexTablaPags);
		list_add(listaTablaDePag, tablaDePag);
		pthread_mutex_unlock(&mutexTablaPags);

		variableIdMarco++;
	}

	estructuraEnvio->PID = idProc;

	return estructuraEnvio;
}

t_escribir_falso* escribir_falso(int idProc, int nroPag, char* textoAEscribir, int socketSwap, int socketCPU) {

	t_contenido_pagina * escritura;
	aux = 0;
	escritura = iniciarContenidoPagina();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();
	t_escribir_falso* estructuraDevolucionEscribirFalso;
	estructuraDevolucionEscribirFalso = crearEscribirFalso();

	marcoYBit = buscarSiEstaEnMemoria(idProc, nroPag);

	escritura->numeroPagina = nroPag;
	escritura->PID = idProc;
	escritura->contenido = textoAEscribir;

	if (marcoYBit->bitPresencia == 0) { // traer de swap una pag, cargarla a memoria

		usleep(configuracion->retardoMemoria * 1000);
		estructuraDevolucionEscribirFalso->PID = idProc;
		estructuraDevolucionEscribirFalso->pagina = nroPag;
		estructuraDevolucionEscribirFalso->socketSwap = socketSwap;
		estructuraDevolucionEscribirFalso->idMarco = marcoYBit->idMarco;

		return estructuraDevolucionEscribirFalso;
	} else {	// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(marcoYBit->idMarco, textoAEscribir);

		estructuraDevolucionEscribirFalso->contenido = textoAEscribir;
		return estructuraDevolucionEscribirFalso;
	}

}

t_contenido_pagina* respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(int idProc, int pag, char* contenido, int flagEscritura, int socketCPU, int socketSwap) {

	char* algoritmoLRU = string_new();
	string_append(&algoritmoLRU, "LRU");
	char* algoritmoCLOCK = string_new();
	string_append(&algoritmoCLOCK, "CLOCK");
	char* algoritmoFIFO = string_new();
	string_append(&algoritmoFIFO, "FIFO");

	t_contenido_pagina* lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	//warning comparacion provoca resultado inesperado, entonces se corrige

	if (strcmp(configuracion->algoritmo_reemplazo, algoritmoLRU) == 0) {

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacarAlMasViejoUsadoDelProcesoDeMemoria(contenido, idProc, pag, flagEscritura, socketSwap);
		} else if (estaLlenaLaMemoria()) {
			sacarAlMasViejoUsadoDeMemoria(socketSwap, idProc, contenido, pag, flagEscritura);
		}

	} else if(strcmp(configuracion->algoritmo_reemplazo, algoritmoCLOCK) == 0){ // aca significa que es el de clock

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacaProcesoDeMemoriaSegunClockModificado(contenido, idProc, pag, flagEscritura, socketSwap);
		} else if (estaLlenaLaMemoria()) {
			sacarDeMemoriaSegunClockModificado(socketSwap, idProc, contenido, pag, flagEscritura);
		}

	}else if(strcmp(configuracion->algoritmo_reemplazo, algoritmoFIFO) == 0){

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
					sacaProcesoDeMemoriaSegunFifo(contenido, idProc, pag, flagEscritura, socketSwap);
		} else if (estaLlenaLaMemoria()) {
					sacarDeMemoriaSegunFifo(socketSwap, idProc, contenido, pag, flagEscritura);
		}

	}

	// aca significa que no tuvo que sacar ninguno
	cargarNuevoMarcoAMemoria(contenido, idProc, pag, flagEscritura);
	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;
	string_append(&lecturaMandarCpu->contenido, contenido);

	if (flagEscritura == 0) {
		return lecturaMandarCpu;
	} else { // por escribir
		t_contenido_pagina * escrituraSwap;
		escrituraSwap = iniciarContenidoPagina();
		escrituraSwap->PID = lecturaMandarCpu->PID;
		string_append(&escrituraSwap->contenido, contenido);
		escrituraSwap->numeroPagina = lecturaMandarCpu->numeroPagina;
		return escrituraSwap;
	}

}

t_contenido_pagina* leer_falso(int idProc, int pag, int socketSwap, int socketCPU) {

	char* contenido;
	aux = 1;
	t_contenido_pagina * lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	t_contenido_pagina * traerDeSwapUnaPaginaDeUnProceso;
	traerDeSwapUnaPaginaDeUnProceso = iniciarContenidoPagina();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;

	marcoYBit = buscarSiEstaEnMemoria(idProc, pag);

	if (marcoYBit->bitPresencia == 0) {	// no lo encontro
		usleep(configuracion->retardoMemoria * 1000);

		// estos valores me hacen ver que entro aca en el test
		traerDeSwapUnaPaginaDeUnProceso->PID = 11;
		traerDeSwapUnaPaginaDeUnProceso->numeroPagina = 11;

		return traerDeSwapUnaPaginaDeUnProceso;
	} else { // aca significa que trajo el id porque esta en memoria
		contenido = traerContenidoDeMarco(marcoYBit->idMarco);
		lecturaMandarCpu->contenido = contenido;
		return lecturaMandarCpu;
	}
}

t_PID* finalizar_falso(t_PID* estructuraFinalizar, int socketSwap) {

	int a, tamanioListaMarcoYBit;
	t_list * listaDeMarcoYBit;
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();
	listaDeMarcoYBit = buscarLosMarcoYBitDeProceso(estructuraFinalizar->PID);
	tamanioListaMarcoYBit = list_size(listaDeMarcoYBit);

	eliminarDeTablaDePaginasDefinitivamente(estructuraFinalizar->PID);

	for (a = 0; a < tamanioListaMarcoYBit; a++) {
		marcoYBit = list_get(listaDeMarcoYBit, a);
		if (marcoYBit->bitPresencia == 1) {
			eliminarDeMemoria(marcoYBit->idMarco);
		}
		if (configuracion->tlbHabilitada == 1) {
			eliminarDeTLBDefinitivamente(marcoYBit->idMarco);
		}
	}
	return estructuraFinalizar;
}

t_contenido_pagina* respuestaTraerDeSwapUnaPaginaDeUnProcesoFalsoFalso(int idProc, int pag, char* contenido, int flagEscritura, int socketCPU, int socketSwap) {

	char* algoritmo = string_new();
	string_append(&algoritmo, "LRU");

	t_contenido_pagina* lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();

	//warning comparacion provoca resultado inesperado, entonces se corrige

	if (strcmp(configuracion->algoritmo_reemplazo, algoritmo) == 0) {
		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacarAlMasViejoUsadoDelProcesoDeMemoria(contenido, idProc, pag, flagEscritura, socketSwap);
		} else if (estaLlenaLaMemoria()) {
			sacarAlMasViejoUsadoDeMemoria(socketSwap, idProc, contenido, pag, flagEscritura);

		}

	} else { // aca significa que es el de clock
		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			marcoYFlag = sacaProcesoDeMemoriaSegunClockModificadoFalso(contenido, idProc, pag, flagEscritura, socketSwap);
			lecturaMandarCpu->contenido = marcoYFlag->marco->contenido;
			lecturaMandarCpu->PID = marcoYFlag->flag;
			return lecturaMandarCpu;
		} else if (estaLlenaLaMemoria()) {
			marcoYFlag = sacarDeMemoriaSegunClockModificadoFalso(socketSwap, idProc, contenido, pag, flagEscritura);
			lecturaMandarCpu->contenido = marcoYFlag->marco->contenido;
			lecturaMandarCpu->PID = marcoYFlag->flag;
			return lecturaMandarCpu;
		}
	}

	// aca significa que no tuvo que sacar ninguno
	cargarNuevoMarcoAMemoria(contenido, idProc, pag, flagEscritura);
	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;
	string_append(&lecturaMandarCpu->contenido, contenido);

	if (flagEscritura == 0) {
		return lecturaMandarCpu;
	} else { // por escribir
		t_contenido_pagina * escrituraSwap;
		escrituraSwap = iniciarContenidoPagina();
		escrituraSwap->PID = lecturaMandarCpu->PID;
		string_append(&escrituraSwap->contenido, contenido);
		escrituraSwap->numeroPagina = lecturaMandarCpu->numeroPagina;
		return escrituraSwap;
	}

}

t_marco_con_flag* sacaProcesoDeMemoriaSegunClockModificadoFalso(char* contenidoACargar, int PIDACargar, int pagACargar, int flagEscritura, int socketSwap) {

	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();

	marcoYFlag = buscarModificadaYUsoEnCeroDeProceso(PIDACargar);

	if (marcoYFlag->flag == 0) {

		marcoYFlag = buscarUsoEnCeroModificadaEnUnoDeProceso(PIDACargar);
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarModificadaYUsoEnCeroDeProceso(PIDACargar);
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUnoDeProceso(PIDACargar);
	}

	return marcoYFlag;
	verificarBitDeModificada(marcoYFlag->marco, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);

}

t_marco_con_flag* sacarDeMemoriaSegunClockModificadoFalso(int socketSwap, int PIDACargar, char* contenidoACargar, int pagACargar, int flagEscritura) {

	t_marco_con_flag* marcoYFlag;
	marcoYFlag = iniciarMarcoYFlag();

	marcoYFlag = buscarModificadaYUsoEnCero();

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUno();
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarModificadaYUsoEnCero();
	}

	if (marcoYFlag->flag == 0) {
		marcoYFlag = buscarUsoEnCeroModificadaEnUno();
	}

	return marcoYFlag;
	verificarBitDeModificada(marcoYFlag->marco, contenidoACargar, PIDACargar, pagACargar, flagEscritura, socketSwap);
}

