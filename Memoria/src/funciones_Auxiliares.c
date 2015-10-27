#include "Memoria.h"

int buscarSiEstaEnMemoria(int idProc, int nroPag) {
	int tamanioTLB, a, tamanioTablaPag, idMarco, flagTLB = 0, flagTDP = 0;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	if (configuracion->tlbHabilitada == 0) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idProc == idProc && campoTLB->paginaDelProceso == nroPag /*que este en un marco*/) {
				idMarco = campoTLB->idMarco;
				flagTLB = 1;
			}
		}
	} else {
		// sino veo si esta en la tabla de paginas

		tamanioTablaPag = list_size(listaTablaDePag);
		//sleep(configuracion->retardoMemoria);
		for (a = 0; a < tamanioTablaPag && flagTDP == 0; a++) {
			campoTablaDePag = list_get(listaTablaDePag, a);
			if (campoTablaDePag->idProc == idProc && campoTablaDePag->paginaDelProceso == nroPag) {
				idMarco = campoTablaDePag->idMarco;
				flagTDP = 1;
			}
		}
	}

	return idMarco;

}

void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido) {
	int tamanioTLB, a, tamanioTablaPag, flagTLB = 0, flagTablaDePag = 0, tamanioMemoria, flagMemoria = 0;
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
				list_replace(listaTLB,a,campoTLB);
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
			list_replace(listaTablaDePag,a,campoTablaDePag);
			flagTablaDePag = 1;
		}
	}
	tamanioMemoria = list_size(listaMemoria);
	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioMemoria && flagMemoria == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->idMarco == idMarco) {
			campoMarco->contenido = contenido;
			variableEnvejecimientoMarco ++;
			campoMarco->posicion = variableEnvejecimientoMarco;
			list_replace(listaMemoria,a,campoMarco);
			flagMemoria = 1;
		}
	}

}

void cargarNuevoMarcoAMemoria(char* contenido,int PID, int pag) {
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	int tamanioTablaDePag,a,flag=0;
	tamanioTablaDePag = list_size(listaTablaDePag);

	variableIdMarcoPos++;

	//sleep(configuracion->retardoMemoria);
	for(a=0;a<tamanioTablaDePag && flag ==1;a++){
		campoTablaDePag= list_get(listaTablaDePag,a);
		if(campoTablaDePag->idProc == PID && campoTablaDePag->paginaDelProceso == pag){
			campoTablaDePag->idMarco = variableIdMarcoPos;
			list_add(listaTablaDePag,campoTablaDePag);
		}
	}

	variableEnvejecimientoMarco ++;
	campoAux->idMarco = variableIdMarcoPos;
	campoAux->contenido = contenido;
	campoAux->posicion = variableEnvejecimientoMarco;

	if(configuracion->tlbHabilitada ==0){
		cargarNuevoEnTLB(PID,pag,campoAux->idMarco);
	}
	list_add(listaMemoria, campoAux);


}

void cargarNuevoEnTLB(int PID,int pag,int id){
	int tamanioTLB,a;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();

	tamanioTLB=list_size(listaTLB);

	if(tamanioTLB==configuracion->entradasTlb){
		sacarAlPrimeroDeTLB();
	}


	variableTLB ++;
	campoTLB->bitPagModificada=0;
	campoTLB->idMarco=id;
	campoTLB->idProc=PID;
	campoTLB->paginaDelProceso=pag;
	campoTLB->posicion = variableTLB;

	list_add(listaTLB,campoTLB);
}

void sacarAlPrimeroDeTLB() {
	int tamanioTLB, a,posicionEnLista;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TLB * campoMasViejoTLB;
	campoMasViejoTLB = iniciarTLB();

	tamanioTLB = list_size(listaTLB);

	for(a=0;a<tamanioTLB;a++){
		campoTLB=list_get(listaTLB,a);
		if(a==0){
			campoMasViejoTLB = campoTLB;
			posicionEnLista = a;
		} else {
			if(campoTLB->posicion < campoMasViejoTLB->posicion){
				posicionEnLista = a;
			}
		}
	}
	list_remove(listaTLB,posicionEnLista);


}

bool llegoAlMaximoDelProcesoLaMemoria(int idProc) {
	bool respuesta;
	int a, tamanioTablaDePag, contadorMarcosEnMemoria=0, flag = 0;
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc && campoTablaDePag->idMarco >0 && flag == 0) { // o sea esta en memoria
			contadorMarcosEnMemoria++;
			if (contadorMarcosEnMemoria == configuracion->maximosMarcosPorProceso) {
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

void sacarAlMasViejoDelProcesoDeMemoria(char* contenidoACargar, int PIDACargar, int pagACargar,int socketSwap) {

	// busco todos los id de un proceso, luego el menor va a ser el mas viejo
	int a,tamanioListaMarco,primero=0;
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	t_list* listaMarco;
	listaMarco = list_create();

	listaMarco = buscarLosMarcosDeProcesoEnMemoria(PIDACargar);
	tamanioListaMarco = list_size(listaMarco);
	for(a=0;a<tamanioListaMarco;a++){
		campoMarco = list_get(listaMarco,a);
		if(primero == 0){
			primero++;
			campoAux = campoMarco;
		}else {
			if(campoMarco->posicion < campoAux->posicion){
				campoAux = campoMarco;
			}
		}


	}

	verificarBitDeModificada(campoAux,contenidoACargar, PIDACargar,pagACargar,socketSwap);


}

void sacarAlMasViejoUsadoDeMemoria(int socketSwap,int PIDACargar,char* contenidoACargar,int pagACargar) {
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_marco* campoAux;
	campoAux = iniciarMarco();
	int tamanioMemoria,idMenor,id,a,nuevoId,primero=0;

	tamanioMemoria = list_size(listaMemoria);
	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioMemoria; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->idMarco > 0) {
			if (primero == 0) {
				primero++;
				campoAux=campoMarco;
			} else {
				if (campoMarco->posicion < campoAux->posicion) {
					campoAux=campoMarco;
				}
			}
		}
	}
	verificarBitDeModificada(campoAux,contenidoACargar,PIDACargar,pagACargar,socketSwap);

}

void verificarBitDeModificada(t_marco* campoMarco, char* contenidoACargar, int PIDaCargar, int pagACargar,int socketSwap) {
	/* solo se usa en las funciones de sacar a un marco de memoria entonces
	   en la respuesta de sobreescribir a swap, se va a mandar a cpu el contenido,
	   que es lo que se manda en el caso que no haya que sacar alguno */

	int tamanioTLB, tamanioTablaDePag, a, flagTLB, flagTablaDePag, pagina, idProc;
	int bitTLB=0, bitTablaDePag=0,nuevoId;
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
					list_add(listaTLB,campoTLB);
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
					list_add(listaTablaDePag,campoTablaDePag);
					pagina = campoTablaDePag->paginaDelProceso;
					idProc = campoTablaDePag->idProc;
				}
			}
		}

	}
	eliminarDeMemoria(campoMarco->idMarco);
	nuevoId= eliminarDeTablaDePaginas(campoMarco->idMarco);
	if (configuracion->tlbHabilitada == 0) {
		eliminarDeTLBSiEstaPorNuevoId(campoMarco->idMarco,nuevoId);
	}
	cargarNuevoMarcoAMemoria(contenidoACargar,PIDaCargar,pagACargar);


	if (bitTablaDePag == 1 || bitTLB == 1) {// teoricamente los dos tienen que estar en uno

		//sleep(configuracion->retardoMemoria);
		enviarASwapContenidoPaginaDesactualizada(idProc, pagina, campoMarco->contenido,socketSwap);
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
			variableEnvejecimientoMarco ++;
			campoMemoria->posicion = variableEnvejecimientoMarco;
			flag = 1;
		}
	}
	contenido = campoMemoria->contenido;

	return contenido;
}

t_list* buscarLosIdDeProceso(int idProc) {
	int a, tamanioTablaDePag;
	t_list* listaId;
	listaId = list_create();
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	//sleep(configuracion->retardoMemoria);
	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc) {
			list_add(listaId, campoTablaDePag->idMarco);
		}
	}

	return listaId;

}

t_list* buscarLosMarcosDeProcesoEnMemoria( int PID) {
	int a,b,tamanioMemoria,id,tamanioListaId,flag;
	t_list* listaMarcos;
	listaMarcos = list_create();
	t_list* listaId;
	listaId = list_create();
	t_marco* campoMarco;
	campoMarco = iniciarMarco();

	listaId=buscarLosIdDeProceso(PID);
	tamanioListaId = list_size(listaId);
	for (a = 0; a < tamanioListaId; a++) {
		id = list_get(listaId,a);
		flag =0;
		//sleep(configuracion->retardoMemoria);
		for(b=0;flag ==0;b++){
			campoMarco = list_get(listaMemoria,b);
			if(campoMarco->idMarco== id){
				list_add(listaMarcos,campoMarco);
				flag =1;
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

int eliminarDeTablaDePaginas(int id) {
	int a, tamanioTablaDePaginas, flag = 0;
	tamanioTablaDePaginas = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	for (a = 0; a < tamanioTablaDePaginas && flag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == id) {
			variableIdMarcoNeg--;
			campoTablaDePag->idMarco = variableIdMarcoNeg;
			flag = 1;
		}
	}
	return campoTablaDePag->idMarco;
}

void eliminarDeTLBSiEstaPorNuevoId(int idMenor,int nuevoId) {
	int a, tamanioTLB, flag = 0;
	tamanioTLB = list_size(listaTLB);
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();

	for (a = 0; a < tamanioTLB && flag == 0; a++) {
		campoTLB = list_get(listaTLB, a);
		if (campoTLB->idMarco == idMenor) {
			campoTLB->idMarco = nuevoId;
			flag = 1;
		}
	}

}

void eliminarDeTLBDefinitivamente(int id){
	int a, tamanioTLB, flag = 0;
		tamanioTLB = list_size(listaTLB);
		t_TLB* campoTLB;
		campoTLB = iniciarTLB();

		for (a = 0; a < tamanioTLB && flag == 0; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idMarco == id) {
				list_remove(listaTLB,a);
				flag = 1;
			}
		}
}

void eliminarDeTablaDePaginasDefinitivamente(int id){
	int a, tamanioTablaDePaginas, flag = 0;
		tamanioTablaDePaginas = list_size(listaTablaDePag);
		t_TablaDePaginas* campoTablaDePag;
		campoTablaDePag = iniciarTablaDePaginas();

		for (a = 0; a < tamanioTablaDePaginas && flag == 0; a++) {
			campoTablaDePag = list_get(listaTablaDePag, a);
			if (campoTablaDePag->idMarco == id) {
				list_remove(listaTablaDePag,a);
				flag = 1;
			}
		}
}

void respuestaTraerDeSwapUnaPaginaDeUnProceso(int idProc, int pag, char* contenido, int socketCPU, int socketSwap) {

	t_contenido_pagina* lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	if (configuracion->algoritmo_reemplazo == "LRU") {

		if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
			sacarAlMasViejoDelProcesoDeMemoria(contenido, idProc, pag, socketSwap);
		} else if (estaLlenaLaMemoria()) {
			sacarAlMasViejoUsadoDeMemoria(socketSwap, idProc, contenido, pag);
		}

	} else { // aca significa que es el de clock

	}

	// aca significa que no tuvo que sacar ninguno
	cargarNuevoMarcoAMemoria(contenido,idProc,pag);
	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;
	lecturaMandarCpu->contenido = contenido;

	enviarACPUContenidoPaginaDeUnProceso(lecturaMandarCpu, socketCPU); // en esta funcion se tiene que mandar a cpu el lecturaMandarCPU
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

void enviarACPUContenidoPaginaDeUnProceso(t_contenido_pagina* lecturaMandarCpu,int socketCPU) {
	enviarStruct(socketCPU, RESUL_LEER_OK_CPU, lecturaMandarCpu);
}

void traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag,int socketSwap) {
	t_leerDeProceso* estructura;
	estructura = crearEstructuraLeer();
	estructura->PID = idProc;
	estructura->numeroPaginaFin = nroDePag;
	estructura->numeroPaginaInicio = nroDePag;
	enviarStruct(socketSwap, LEER_SWAP, estructura);
}

void enviarFinalizarACPU(t_PID* estructuraFinalizar, int socketCPU){
	enviarStruct(socketCPU, RESUL_FIN, estructuraFinalizar);
}

void inicializacionDesdeCero(){
	listaMemoria = list_create();
	listaTLB = list_create();
	listaTablaDePag = list_create();
	variableIdMarcoNeg = 0;
	variableIdMarcoPos =0;
	variableTLB =0;
	variableEnvejecimientoMarco =0;

}

void iniciarConfiguracionTLBNoHabilitada(){
	configuracion->tlbHabilitada = 0;
}

void hardcodearTablaDePaginas(int pag1,int pag2,int pag3,int pag4,int pag5) {
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
    t_marco * campoMemoria;
    t_marco * campoMemoria1;
    campoMemoria1 = iniciarMarco();

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag1);
	campoTablaDePag->idMarco = 455;
	list_replace(listaTablaDePag,1,campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag2);
	campoTablaDePag->idMarco = 456 ;
	list_replace(listaTablaDePag,5,campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag3);
	campoTablaDePag->idMarco = 457 ;
	list_replace(listaTablaDePag,9,campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag4);
	campoTablaDePag->idMarco = 458 ;
	list_replace(listaTablaDePag,14,campoTablaDePag);

	campoTablaDePag = iniciarTablaDePaginas();
	campoTablaDePag = list_get(listaTablaDePag, pag5);
	campoTablaDePag->idMarco = 459;
	list_replace(listaTablaDePag,18,campoTablaDePag);


	campoMemoria = iniciarMarco();
	campoMemoria->contenido = 'PID 1';
	campoMemoria->idMarco = 455;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria = iniciarMarco();
	campoMemoria->contenido = 'PID 2';
	campoMemoria->idMarco = 456;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria->contenido = 'PID 3';
	campoMemoria->idMarco = 457;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria->contenido = 'PID 3';
	campoMemoria->idMarco = 458;
	list_add(listaMemoria, campoMemoria);

	campoMemoria = iniciarMarco();
	campoMemoria->contenido = 'PID 4';
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
			variableIdMarcoNeg --;
			tablaDePag = iniciarTablaDePaginas();
			tablaDePag->idProc = idProc;
			tablaDePag->paginaDelProceso = contador;
			tablaDePag->idMarco = variableIdMarcoNeg; // porque no esta en algun marco en mem pcpal
			tablaDePag->bitPagModificada = 0;
			list_add(listaTablaDePag, tablaDePag);
		}
    return estructura;
}

t_contenido_pagina* escribir_falso(int idProc, int nroPag, char* textoAEscribir, int socketSwap) {
	// 1 -ver si estan en memoria y ponerle el bit de modificada
	// 2- si no esta mandar a escribir a swap

	t_contenido_pagina * escritura;
	escritura = iniciarEscrituraProc();
	int idMarco;

	//veo si esta en un marco de memoria

	idMarco = buscarSiEstaEnMemoria(idProc,nroPag);

	if( idMarco < 0){
		// 2
			escritura->numeroPagina = nroPag;
			escritura->PID = idProc;
			escritura->contenido = textoAEscribir;

			return escritura;

	}else {// entonces tengo el id del marco
		escribirEnMarcoYponerBitDeModificada(idMarco,textoAEscribir);
	}

}

t_PID* finalizar_falso(t_PID* estructuraFinalizar,int socketSwap)  {
	int a,tamanioListaId,id;
		t_list * listaDeId;
		listaDeId = buscarLosIdDeProceso(estructuraFinalizar->PID);
		tamanioListaId = list_size(listaDeId);

		for (a = 0; a < tamanioListaId; a++) {
			id = list_get(listaDeId, a);
			eliminarDeTablaDePaginasDefinitivamente(id);
			if (id > 0) {
				eliminarDeMemoria(id);
			}
			if (configuracion->tlbHabilitada ==0) {
				eliminarDeTLBDefinitivamente(id);
			}
		}
	return estructuraFinalizar;
}

