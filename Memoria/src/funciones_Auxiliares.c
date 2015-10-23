/*
 * funciones_Auxiliares.c
 *
 *  Created on: 27/9/2015
 *      Author: utnso
 */
#include "Memoria.h"

int buscarSiEstaEnMemoria(int idProc, int nroPag) {
	int tamanioTLB, a, tamanioTablaPag, idMarco ,flagTLB =0,flagTDP = 0;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	if (configuracion->tlbHabilitada == 1) {
		tamanioTLB = list_size(listaTLB);
		for (a = 0; a < tamanioTLB; a++) {
			campoTLB = list_get(listaTLB, a);
			if (campoTLB->idProc == idProc && campoTLB->paginaDelProceso == nroPag /*que este en un marco*/) {
				idMarco = campoTLB->idMarco;
				flagTLB = 1;
			}
		}
	}
	// sino veo si esta en la tabla de paginas

	tamanioTablaPag = list_size(listaTablaDePag);
	for (a = 0; a < tamanioTablaPag && flagTLB == 0 && flagTDP ==0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc && campoTablaDePag->paginaDelProceso == nroPag ) {
			idMarco = campoTablaDePag->idMarco;
			flagTDP=1;
		}
	}

	return idMarco; // si no encontro retorna -1 que es lo primero que se seteo

}

void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido) {
	int tamanioTLB, a, tamanioTablaPag, flagTLB = 0, flagTablaDePag = 0, tamanioMemoria, flagMemoria = 0;
	t_TLB * campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco * campoMarco;
	campoMarco = iniciarMarco();

	if (configuracion->tlbHabilitada == 1) {
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
	for (a = 0; a < tamanioTablaPag && flagTablaDePag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == idMarco) {
			campoTablaDePag->bitPagModificada = 1;
			list_replace(listaTablaDePag,a,campoTablaDePag);
			flagTablaDePag = 1;
		}
	}
	tamanioMemoria = list_size(listaMemoria);
	for (a = 0; a < tamanioMemoria && flagMemoria == 0; a++) {
		campoMarco = list_get(listaMemoria, a);
		if (campoMarco->idMarco == idMarco) {
			campoMarco->contenido = contenido;
			list_replace(listaMemoria,a,campoMarco);
			flagMemoria = 1;
		}
	}

}

void cargarNuevoMarcoAMemoria(char* contenido) {
	t_marco* campoAux;
	campoAux = iniciarMarco();

	variableIdMarcoPos++;

	campoAux->idMarco = variableIdMarcoPos;
	campoAux->contenido = contenido;

	list_add(listaMemoria, campoAux);


}

bool llegoAlMaximoDelProcesoLaMemoria(int idProc) {
	bool respuesta;
	int a, tamanioTablaDePag, contadorMarcosEnMemoria=0, flag = 0;
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

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

void sacarAlPrimeroDeMemoriaDelProceso(int idProc) {

	// busco todos los id de un proceso, luego el menor va a ser el mas viejo
	int a, b, id, tamanioMemoria, tamanioListaId, flag = 0,idMenor;
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	t_list* listaIdMarco;
	listaIdMarco = list_create();

	listaIdMarco = buscarLosIdDeProceso(idProc);
	tamanioListaId = list_size(listaIdMarco);
	for(a=0;a<tamanioListaId;a++){
		id = list_get(listaIdMarco,a);
		if(a == 0){
			idMenor = id;
		}else {
			if(id < idMenor){
				idMenor = id;
				campoMarco = list_get(listaMemoria,a);
			}
		}

	}

	eliminarDeMemoria(idMenor);
	eliminarDeTablaDePaginas(idMenor);
	if (configuracion->tlbHabilitada == 0) {
		eliminarDeTLBSiEsta(idMenor);
	}

	verificarBitDeModificada(campoMarco->idMarco, campoMarco->contenido);

	free(campoMarco);

}

void sacarAlPrimeroDeMemoria() {
	t_marco* campoMarco;
	campoMarco = iniciarMarco();
	int tamanioMemoria,idMenor,id,a;
	tamanioMemoria = list_size(listaMemoria);
	for(a=0;a<tamanioMemoria;a++){
		if(a == 0){
			idMenor = id;
		} else {
			if(id < idMenor){
				idMenor = id;
				campoMarco = list_get(listaMemoria,a);
			}
		}
	}
	eliminarDeMemoria(idMenor);
	eliminarDeTablaDePaginas(idMenor);
	if (configuracion->tlbHabilitada == 0) {
			eliminarDeTLBSiEsta(idMenor);
	}

	verificarBitDeModificada(campoMarco->idMarco, campoMarco->contenido);

	free(campoMarco);

}

void verificarBitDeModificada(int idMarco, char* contenido) {
	int tamanioTLB, tamanioTablaDePag, a, flagTLB, flagTablaDePag, pagina, idProc;/* teoricamente si esta en TLB el bit de modif de un proceso tamb esta en TAbla de pag,
	 lo hago para verificar nada mas*/
	tamanioTLB = list_size(listaTLB);
	tamanioTablaDePag = list_size(listaTablaDePag);
	t_TLB* campoTLB;
	campoTLB = iniciarTLB();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();

	for (a = 0; a < tamanioTLB && flagTLB == 0; a++) {
		campoTLB = list_get(listaTLB, a);
		if (campoTLB->idMarco == idMarco) {
			flagTLB = 1;
			list_remove(listaTLB, a);
		}
	}

	for (a = 0; a < tamanioTablaDePag && flagTablaDePag == 0; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == idMarco) {
			flagTablaDePag = 1;
			list_remove(listaTablaDePag, a);
		}
	}

	if (campoTablaDePag->bitPagModificada == 1 && campoTLB->bitPagModificada == 1) { // teoricamente los dos tienen que estar en uno
		pagina = campoTablaDePag->paginaDelProceso;
		idProc = campoTablaDePag->idProc;

		enviarASwapContenidoPaginaDesactualizada(idProc, pagina, contenido);
	}

	free(campoTLB);
	free(campoTablaDePag);

}

char* traerContenidoDeMarco(int idMarco) {
	char* contenido;
	int tamanioMemoria, a, flag = 0;
	tamanioMemoria = list_size(listaMemoria);
	t_marco* campoMemoria;
	campoMemoria = iniciarMarco();

	for (a = 0; a < tamanioMemoria && flag == 0; a++) {
		campoMemoria = list_get(listaMemoria, a);
		if (campoMemoria->idMarco == idMarco) {
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

	for (a = 0; a < tamanioTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idProc == idProc) {
			list_add(listaId, campoTablaDePag->idMarco);
		}
	}



	return listaId;

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
			list_remove(listaTablaDePag, a);
			flag = 1;
		}
	}
}

void eliminarDeTLBSiEsta(int id) {
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

	free(campoTLB);
}

void respuestaTraerDeSwapUnaPaginaDeUnProceso(int idProc, int pag, char* contenido, int socketCPU) {

	t_contenido_pagina* lecturaMandarCpu;
	lecturaMandarCpu = iniciarContenidoPagina();
	lecturaMandarCpu->PID = idProc;
	lecturaMandarCpu->numeroPagina = pag;
	if (llegoAlMaximoDelProcesoLaMemoria(idProc)) { // si llega al max de procesos no importa si esta llena la memoria porque si o si va a sacar a uno
		sacarAlPrimeroDeMemoriaDelProceso(idProc);
		cargarNuevoMarcoAMemoria(contenido);

	} else if (estaLlenaLaMemoria()) {
		sacarAlPrimeroDeMemoria();
		cargarNuevoMarcoAMemoria(contenido);

	} else { // si no llego ni al maximo de procesos ni al maximo de marcos
		cargarNuevoMarcoAMemoria(contenido);

	}
	lecturaMandarCpu->contenido = contenido;

	enviarACPUContenidoPaginaDeUnProceso(lecturaMandarCpu, socketCPU); // en esta funcion se tiene que mandar a cpu el lecturaMandarCPU
}


void enviarASwapEliminarProceso(int idProc) {

}

void enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap) {
	enviarStruct(socketSwap, INICIAR_PROC_SWAP, estructura);
}

void enviarFinalizarASwap(t_PID *estructura, int socketSwap) {
	enviarStruct(socketSwap, FIN_PROCESO_SWAP, estructura);
}


void enviarASwapContenidoPaginaDesactualizada(int idProc, int pagina, char* contenido) {

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
	contadorPagTP = 0;
	variableIdMarcoNeg = 0;
	variableIdMarcoPos =0;

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
			contadorPagTP++;
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

int finalizar_falso(int idProc) {
	int a, tamanioListaId, id;
	t_list * listaDeId;
	listaDeId = buscarLosIdDeProceso(idProc);
	tamanioListaId = list_size(listaDeId);

	for (a = 0; a < tamanioListaId; a++) {
		id = list_get(listaDeId, a);
		eliminarDeTablaDePaginas(id);
		if (id > 0) {
			eliminarDeMemoria(id);
		}
		if (configuracion->tlbHabilitada == 0) {
			eliminarDeTLBSiEsta(id);
		}
	}
	return idProc;
}


