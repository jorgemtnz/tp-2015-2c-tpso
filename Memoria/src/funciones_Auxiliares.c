#include "Memoria.h"

t_marco_y_bit* buscarSiEstaEnMemoria(int idProc, int nroPag) {
	//warning no usa variable, se debe cual es el uso de flagTDP
	int tamanioTLB, a, tamanioTablaPag, flagTLB = 0, flagTDP = 0;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	if (configuracion->tlbHabilitada == 0) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idProc == idProc
					&& campoTLB->paginaDelProceso
							== nroPag /*que este en un marco*/) {
				marcoYBit->idMarco = campoTLB->idMarco;
				marcoYBit->bitPresencia = campoTLB->bitPresencia;
				flagTLB = 1;
			}
		}
	} else {
		// sino veo si esta en la tabla de paginas

		tamanioTablaPag = list_size(listaTablaDePag);
		printf("%i\n", tamanioTablaPag);
		//sleep(configuracion->retardoMemoria);
		for (a = 0; a < tamanioTablaPag && flagTDP == 0; a++) {
			campoTablaDePag = list_get(listaTablaDePag, a);
			if (campoTablaDePag->idProc == idProc
					&& campoTablaDePag->paginaDelProceso == nroPag) {
				marcoYBit->idMarco = campoTLB->idMarco;
				marcoYBit->bitPresencia = campoTLB->bitPresencia;
				flagTDP = 1;
			}
		}
	}

	printf("%i\n", flagTDP);
	return marcoYBit;

}

void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido) {
	int tamanioTLB, a, tamanioTablaPag, flagTLB = 0, flagTablaDePag = 0,
			tamanioMemoria, flagMemoria = 0;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco * campoMarco;
	campoMarco = iniciarMarco();

	if (configuracion->tlbHabilitada == 0) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idMarco == idMarco) {
				campoTLB->bitPagModificada = 1;
				list_replace(listaTLB, a, campoTLB);
				flagTLB = 1;
			}
		}
	}
	// veo si esta en la tabla de paginas y la modifico ( por mas que este en la TLB, tambien tengo que modificarlo
	// en la Tabla de Pag)

	tamanioTablaPag = list_size(listaTablaDePag);
	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioTablaPag && flagTablaDePag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == idMarco) {
			campoTablaDePag->bitPagModificada = 1;
			list_replace(listaTablaDePag, a, campoTablaDePag);
			flagTablaDePag = 1;
		}
	}
	tamanioMemoria = list_size(listaMemoria);
	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioMemoria && flagMemoria == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->idMarco == idMarco) {
			campoMarco->contenido = contenido;
			variableEnvejecimientoMarco++;
			campoMarco->posicion = variableEnvejecimientoMarco;
			list_replace(listaMemoria, a, campoMarco);
			flagMemoria = 1;
		}
	}

}

void cargarNuevoMarcoAMemoria(char* contenido, int PID, int pag) {
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	int tamanioTablaDePag, a, flag = 0;
	tamanioTablaDePag = list_size(listaTablaDePag);

	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioTablaDePag && flag == 1; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == PID
				&& campoTablaDePag->paginaDelProceso == pag) {
			campoAux->idMarco = campoTablaDePag->idMarco;
			list_add(listaTablaDePag, campoTablaDePag);
		}
	}

	variableEnvejecimientoMarco++;
	campoAux->contenido = contenido;
	campoAux->posicion = variableEnvejecimientoMarco;

	if (configuracion->tlbHabilitada == 0) {
		cargarNuevoEnTLB(PID, pag, campoAux->idMarco);
	}
	list_add(listaMemoria, campoAux);

}

void cargarNuevoEnTLB(int PID, int pag, int id) {
	//warning no se usa la variable a, entonces la comento
//	int a;
	int tamanioTLB;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();

	tamanioTLB = list_size(listaTLB);

	if (tamanioTLB == configuracion->entradasTlb) {
		sacarAlPrimeroDeTLB();
	}

	variableTLB++;
	campoTLB->bitPagModificada = 0;
	campoTLB->idMarco = id;
	campoTLB->idProc = PID;
	campoTLB->paginaDelProceso = pag;
	campoTLB->posicion = variableTLB;

	list_add(listaTLB, campoTLB);
}

void sacarAlPrimeroDeTLB() {
	int tamanioTLB, a, posicionEnLista;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TLB * campoMasViejoTLB;
	campoMasViejoTLB = iniciarTLB();

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

}

bool llegoAlMaximoDelProcesoLaMemoria(int idProc) {
	bool respuesta;
	int a, tamanioTablaDePag, contadorMarcosEnMemoria = 0, flag = 0;
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc
				&& campoTablaDePag->bitPresencia == 1 && flag == 0) { // o sea esta en memoria
			contadorMarcosEnMemoria++;
			if (contadorMarcosEnMemoria
					== configuracion->maximosMarcosPorProceso) {
				flag = 1;
			}
		}
	}

	if (flag == 0) {
		respuesta = false;
	} else {
		respuesta = true;
	}
	return respuesta;
}

bool estaLlenaLaMemoria() {
	bool respuesta;
	int tamanioMemoria;
	tamanioMemoria = list_size(listaMemoria);

	if (tamanioMemoria < configuracion->cantidadMarcos) {
		respuesta = false;
	} else {
		respuesta = true;
	}

	return respuesta;
}

void sacarAlMasViejoUsadoDelProcesoDeMemoria(char* contenidoACargar,
		int PIDACargar, int pagACargar, int flagEscritura, int socketSwap) {

	// busco todos los id de un proceso, luego el menor va a ser el mas viejo
	int a, tamanioListaMarco, primero = 0;
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_list* listaMarco;
	listaMarco = list_create();

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

	verificarBitDeModificada(campoAux, contenidoACargar, PIDACargar, pagACargar,
			flagEscritura, socketSwap);

}

void sacarAlMasViejoUsadoDeMemoria(int socketSwap, int PIDACargar,
		char* contenidoACargar, int pagACargar, int flagEscritura) {
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	int tamanioMemoria, a, primero = 0;
//warning no se usan variables, entonces lo comento
//	int nuevoId,idMenor, id;
	tamanioMemoria = list_size(listaMemoria);
	//sleep(configuracion->retardoMemoria);
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
	verificarBitDeModificada(campoAux, contenidoACargar, PIDACargar, pagACargar,
			flagEscritura, socketSwap);

}

void verificarBitDeModificada(t_marco* campoMarco, char* contenidoACargar,
		int PIDaCargar, int pagACargar, int flagEscritura, int socketSwap) {
	/* solo se usa en las funciones de sacar a un marco de memoria entonces
	 en la respuesta de sobreescribir a swap, se va a mandar a cpu el contenido,
	 que es lo que se manda en el caso que no haya que sacar alguno */

	int tamanioTLB, tamanioTablaDePag, a, flagTLB, flagTablaDePag, pagina,
			idProc;
	int bitTLB = 0, bitTablaDePag = 0;
	//warning no usada variable, entonces comento
//		int	nuevoId;
	tamanioTLB = list_size(listaTLB);
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	if (configuracion->tlbHabilitada == 0) {
		for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idMarco == campoMarco->idMarco) {
				flagTLB = 1;
				bitTLB = campoTLB->bitPagModificada;
				if (bitTLB == 1) {
					campoTLB->bitPagModificada = 0;
					list_add(listaTLB, campoTLB);
					pagina = campoTablaDePag->paginaDelProceso;
					idProc = campoTablaDePag->idProc;
				}
			}
		}
	} else {

		for (a = 0; a < tamanioTablaDePag && flagTablaDePag == 0; a++) {
			campoTablaDePag = list_get(listaTablaDePag, a);
			if (campoTablaDePag->idMarco == campoMarco->idMarco) {
				flagTablaDePag = 1;
				bitTablaDePag = campoTablaDePag->bitPagModificada;
				if (bitTablaDePag == 1) {
					campoTablaDePag->bitPagModificada = 0;
					list_add(listaTablaDePag, campoTablaDePag);
					pagina = campoTablaDePag->paginaDelProceso;
					idProc = campoTablaDePag->idProc;
				}
			}
		}

	}
	eliminarDeMemoria(campoMarco->idMarco);
	eliminarDeTablaDePaginas(campoMarco->idMarco);
	if (configuracion->tlbHabilitada == 0) {
		eliminarDeTLB(campoMarco->idMarco);
	}
	cargarNuevoMarcoAMemoria(contenidoACargar, PIDaCargar, pagACargar);

	if (bitTablaDePag == 1 || bitTLB == 1) { // teoricamente los dos tienen que estar en uno

		if (flagEscritura == 0) { // por leer
			//sleep(configuracion->retardoMemoria);
			enviarASwapContenidoPaginaDesactualizada(idProc, pagina,
					campoMarco->contenido, socketSwap);
		} else { // por escribir
			t_contenido_pagina * escrituraSwap;
			escrituraSwap = iniciarContenidoPagina();
			escrituraSwap->PID = idProc;
			escrituraSwap->contenido = campoMarco->contenido;
			escrituraSwap->numeroPagina = pagina;
			enviarEscribirAlSwap(escrituraSwap, socketSwap);
		}

	}
}
char* traerContenidoDeMarco(int idMarco) {
	char* contenido;
	int tamanioMemoria, a, flag = 0;
	tamanioMemoria = list_size(listaMemoria);
	t_marco* campoMemoria;
	campoMemoria = iniciarMarco();

	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioMemoria && flag == 0; a++) {
		campoMemoria = list_get(listaMemoria, a);
		if (campoMemoria->idMarco == idMarco) {
			variableEnvejecimientoMarco++;
			campoMemoria->posicion = variableEnvejecimientoMarco;
			flag = 1;
		}
	}
	contenido = campoMemoria->contenido;

	return contenido;
}

t_list* buscarLosMarcoYBitDeProceso(int idProc) {
	int a, tamanioTablaDePag;
	t_list* listamarcoYBit;
	listamarcoYBit = list_create();
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc) {
			marcoYBit->idMarco = campoTablaDePag->idMarco;
			marcoYBit->bitPresencia = campoTablaDePag->bitPresencia;
			//warning porque necesita un puntero y se le esta mandando un int,verificar que elemento se quiere agregar
			// int list_add(t_list *, void *element);
			list_add(listamarcoYBit, campoTablaDePag->idMarco);
		}
	}

	return listamarcoYBit;

}

t_list* buscarLosMarcosDeProcesoEnMemoria(int PID) {
	int a, b, tamanioListaMarcoYBit, flag;
	//warning no se usa variable, entonces lo comento
//		int id, tamanioMemoria;
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
	for (a = 0; a < tamanioListaMarcoYBit; a++) {
		marcoYBit = list_get(listaMarcoYBit, a);
		flag = 0;
		//sleep(configuracion->retardoMemoria);
		for (b = 0; flag == 0; b++) {
			campoMarco = list_get(listaMemoria, b);
			if (campoMarco->idMarco == marcoYBit->idMarco) {
				list_add(listaMarcos, campoMarco);
				flag = 1;
			}
		}

	}

	return listaMarcos;
}

void eliminarDeMemoria(int id) {
	int a, tamanioMemoria, flag = 0;
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

}

void eliminarDeTablaDePaginas(int id) {
	int a, tamanioTablaDePaginas, flag = 0;
	tamanioTablaDePaginas = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	for (a = 0; a < tamanioTablaDePaginas && flag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == id) {
			campoTablaDePag->bitPresencia = 0;
			flag = 1;
		}
	}
}

void eliminarDeTLB(int idMenor) {
	int a, tamanioTLB, flag = 0;
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

}

void eliminarDeTLBDefinitivamente(int id) {
	int a, tamanioTLB, flag = 0;
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
}

void eliminarDeTablaDePaginasDefinitivamente(int id) {
	int a, tamanioTablaDePaginas, flag = 0;
	tamanioTablaDePaginas = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	for (a = 0; a < tamanioTablaDePaginas && flag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == id) {
			list_remove(listaTablaDePag, a);
			flag = 1;
		}
	}
}

void respuestaTraerDeSwapUnaPaginaDeUnProceso(int idProc, int pag,
		char* contenido, int flagEscritura, int socketCPU, int socketSwap) {

	t_contenido_pagina* lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	//warning comparacion provoca resultado inesperado, entonces se corrige
	if (string_equals(configuracion->algoritmo_reemplazo, "LRU")) {
		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacarAlMasViejoUsadoDelProcesoDeMemoria(contenido, idProc, pag,
					flagEscritura, socketSwap);
		} else if (estaLlenaLaMemoria()) {
			sacarAlMasViejoUsadoDeMemoria(socketSwap, idProc, contenido, pag,
					flagEscritura);
		}

	} else { // aca significa que es el de clock

	}

	// aca significa que no tuvo que sacar ninguno
	cargarNuevoMarcoAMemoria(contenido, idProc, pag);
	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;
	lecturaMandarCpu->contenido = contenido;

	if (flagEscritura == 0) {
		enviarACPUContenidoPaginaDeUnProcesoPorLeer(lecturaMandarCpu,
				socketCPU);
	} else { // por escribir
		t_contenido_pagina * escrituraSwap;
		escrituraSwap = iniciarContenidoPagina();
		escrituraSwap->PID = lecturaMandarCpu->PID;
		escrituraSwap->contenido = lecturaMandarCpu->contenido;
		escrituraSwap->numeroPagina = lecturaMandarCpu->numeroPagina;
		enviarRtaEscribirACPU(escrituraSwap, socketCPU);
	}

}

void enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap) {
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}

void enviarFinalizarASwap(t_PID *estructura, int socketSwap) {
	enviarStruct(socketSwap, FIN_PROCESO_SWAP, estructura);
}

void enviarASwapContenidoPaginaDesactualizada(int idProc, int pagina,
		char* contenido, int socketSwap) {
	t_contenido_pagina* estructura;
	estructura = iniciarContenidoPagina();
	estructura->PID = idProc;
	estructura->numeroPagina = pagina;
	estructura->contenido = contenido;
	enviarStruct(socketSwap, SOBREESCRIBIR_SWAP, estructura);
}

void enviarACPUContenidoPaginaDeUnProcesoPorLeer(
		t_contenido_pagina* lecturaMandarCpu, int socketCPU) {
	enviarStruct(socketCPU, RESUL_LEER_OK_CPU, lecturaMandarCpu);
}

void traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag, int socketSwap) {
	t_leerDeProceso* estructura;
	estructura = crearEstructuraLeer();
	estructura->PID = idProc;
	estructura->numeroPaginaFin = nroDePag;
	estructura->numeroPaginaInicio = nroDePag;
	printf("a\n");
	enviarStruct(socketSwap, LEER_SWAP, estructura);
}
void traerDeSwapUnaPaginaDeUnProcesoPorEscribir(int idProc, int nroPag,
		int socketSwap) {
	t_leerDeProceso* estructura;
	estructura = crearEstructuraLeer();
	estructura->PID = idProc;
	estructura->numeroPaginaFin = nroPag;
	estructura->numeroPaginaInicio = nroPag;
	enviarStruct(socketSwap, LEER_SWAP_POR_ESCRIBIR, estructura);
}

void enviarFinalizarACPU(t_PID* estructuraFinalizar, int socketCPU) {
	enviarStruct(socketCPU, RESUL_FIN, estructuraFinalizar);
}

void inicializacionDesdeCero() {
	listaMemoria = list_create();
	listaTLB = list_create();
	listaTablaDePag = list_create();
	variableIdMarco = 0;
	variableTLB = 0;
	variableEnvejecimientoMarco = 0;

}

void iniciarConfiguracionTLBNoHabilitada() {
	configuracion->tlbHabilitada = 0;
}

void hardcodearTablaDePaginas(int pag1, int pag2, int pag3, int pag4, int pag5) {
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco * campoMemoria;
	//warning no se usa, ojo se declara y se pide memoria en la misma linea, entonces se comenta
//	t_marco * campoMemoria1 = iniciarMarco();

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag1);
	campoTablaDePag->idMarco = 455;
	list_replace(listaTablaDePag, 1, campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag2);
	campoTablaDePag->idMarco = 456;
	list_replace(listaTablaDePag, 5, campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag3);
	campoTablaDePag->idMarco = 457;
	list_replace(listaTablaDePag, 9, campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag4);
	campoTablaDePag->idMarco = 458;
	list_replace(listaTablaDePag, 14, campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag5);
	campoTablaDePag->idMarco = 459;
	list_replace(listaTablaDePag, 18, campoTablaDePag);

	campoMemoria = iniciarMarco();
	//warning asignacion incorrecta, se debe corregir, uso strdup para alojar con malloc, luego se debe hacer el free.
	campoMemoria->contenido = strdup("PID 1");
	campoMemoria->idMarco = 455;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria = iniciarMarco();
	campoMemoria->contenido =strdup("PID 2");
	campoMemoria->idMarco = 456;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria->contenido =strdup("PID 3");
	campoMemoria->idMarco = 457;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria->contenido =strdup("PID 3");
	campoMemoria->idMarco = 458;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria->contenido = strdup("PID 4");
	campoMemoria->idMarco = 459;
	list_add(listaMemoria, campoMemoria);

}

t_iniciar_swap* iniciar_falso(int idProc, int cantPag, int socketCPU) {
	int contador;
	t_TablaDePaginas* tablaDePag;
	t_iniciar_swap * estructura;
	estructura = crearEstructuraIniciar();
	estructura->PID = idProc;
	estructura->cantidadPaginas = cantPag;

	for (contador = 0; contador < cantPag; contador++) {
		variableIdMarco++;
		tablaDePag = iniciarTablaDePaginas();
		tablaDePag->idProc = idProc;
		tablaDePag->paginaDelProceso = contador;
		tablaDePag->idMarco = variableIdMarco; // porque no esta en algun marco en mem pcpal
		tablaDePag->bitPagModificada = 0;
		list_add(listaTablaDePag, tablaDePag);
	}
	return estructura;
}

t_contenido_pagina* escribir_falso(int idProc, int nroPag, char* textoAEscribir,
		int socketSwap) {
	// 1 -ver si estan en memoria y ponerle el bit de modificada
	// 2- si no esta mandar a escribir a swap

	t_contenido_pagina * escritura;
	//warning incompatible la asignacion
	//t_escrituraProc* iniciarEscrituraProc()  y escritura es otro puntero
	//se debe corregir, se corrige tomando de protocolo.h
	escritura = iniciarEscrituraProc();
	t_marco_y_bit* ptr_marco_bit = malloc(sizeof(t_marco_y_bit));

	//warning incompatible la asignacion
	// t_marco_y_bit* buscarSiEstaEnMemoria(int idProc, int nroPag)   y idMarco es un int
	//se debe corregir
	//veo si esta en un marco de memoria
	ptr_marco_bit = buscarSiEstaEnMemoria(idProc, nroPag);

	if (ptr_marco_bit->idMarco < 0) {
		// 2
		escritura->numeroPagina = nroPag;
		escritura->PID = idProc;
		escritura->contenido = textoAEscribir;

		return escritura;

	} else {	// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(ptr_marco_bit->idMarco, textoAEscribir);
		escritura->numeroPagina = nroPag;
		escritura->PID = idProc;
		escritura->contenido = textoAEscribir;

		return escritura;
	}

}

t_PID* finalizar_falso(t_PID* estructuraFinalizar, int socketSwap) {
	int a, tamanioListaMarcoYBit;
	t_list * listaDeMarcoYBit;
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();
	listaDeMarcoYBit = buscarLosMarcoYBitDeProceso(estructuraFinalizar->PID);
	tamanioListaMarcoYBit = list_size(listaDeMarcoYBit);

	for (a = 0; a < tamanioListaMarcoYBit; a++) {
		marcoYBit = list_get(listaDeMarcoYBit, a);
		eliminarDeTablaDePaginasDefinitivamente(marcoYBit->idMarco);
		if (marcoYBit->bitPresencia == 0) {
			eliminarDeMemoria(marcoYBit->idMarco);
		}
		if (configuracion->tlbHabilitada == 0) {
			eliminarDeTLBDefinitivamente(marcoYBit->idMarco);
		}
	}
	return estructuraFinalizar;
}
