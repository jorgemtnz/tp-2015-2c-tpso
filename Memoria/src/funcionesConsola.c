/*
 * funcionesConsola.c
 *
 *  Created on: 5/11/2015
 *      Author: utnso
 */
#include "Memoria.h"

void levantarConsola() {
	char comando[20];
	int idFunc;

	mostrarComandos();
	while (1) {
		puts("Ingrese un comando >> ");
		fgets(comando, 20 ,stdin);
		//scanf("%s", comando);
		idFunc = idFuncion(comando);
		aplicarFuncion(idFunc);
	}
}

int idFuncion(char* funcion) {
	int i;
	char* funcionesConsola[] = {  "TLB_FLUSH", "MEM_FLUSH", "MEM_DUMP"};
	for (i=0;(i < 3) && (strcmp(funcion, funcionesConsola[i]) != 0); i++);
	return (i <= 3 - 1) ? (i + 1) : -1;
}

void aplicarFuncion(int idFuncion) {
	enum nomFun {
		TLB_FLUSH = 1, MEM_FLUSH, MEM_DUMP, MOSTRAR_COMANDOS
	};
	switch (idFuncion) {
	case TLB_FLUSH: {
		puts("Limpiar la TLB\n");
		break;
	}
	case MEM_FLUSH:
		puts("Limpiar la Memoria Principal\n");
		break;

	case MEM_DUMP:
		puts("Volcado de la Memoria a un Log\n");
		break;

	case MOSTRAR_COMANDOS: {
		mostrarComandos();
	}
	break;
	case -1:
		puts("--Ojo ese comando no existe!! proba con mostrarComandos \n");
	}
}

void mostrarComandos() {
	char* funcionesConsola[] = { "TLB_FLUSH", "MEM_FLUSH", "MEM_DUMP"};

	char* descripcionesConsola[] = { "Limpiar la TLB",
			"Limpiar la Memoria Principal", "Volcado de la Memoria a un Log"};

	int contador = 0;
	while (contador <= 2) {
		printf("*------------------------------------------*\n");
		printf("COMANDO 	= %s\n", funcionesConsola[contador]);
		printf("DESCRIPCION = %s\n", descripcionesConsola[contador]);
		printf("*------------------------------------------*\n");
		contador += 1;
	}
}

int procesarMensajesConsola(int socket, t_header* header, char* buffer) {
	pthread_t hilo1, hilo2;
	if(buffer != NULL && strstr(buffer, "\n")) {
		if(string_starts_with(buffer, "\n")){
			buffer = "";
		} else {
			char** split = string_split(buffer, "\n");
			buffer = split[0];
		}
	}
	if(string_equals(buffer, "TLB_FLUSH")) {
		puts("Limpiar TLB");
		pthread_create(&hilo1,NULL,(void*) limpiarTLB,NULL);
		pthread_join(hilo1,NULL);
		//limpiarTLB();
	} else if(string_equals(buffer, "MEM_FLUSH")) {
		puts("Limpiar Memoria");
		pthread_create(&hilo2,NULL,(void*) limpiarMemoria,NULL);
		//limpiarMemoria();
	} else if(string_equals(buffer, "MEM_DUMP")) {
		puts("Volcar Memoria");
		//volcarMemoria();
	} else {
		puts("Comando no reconocido");
	}
	return 0;
}

void limpiarTLB(){
	if (configuracion->tlbHabilitada==1){
		list_clean(listaTLB);
	} else {
		puts("TLB inactiva");
	}
}
void limpiarMemoria(){
	int i;
	t_TablaDePaginas* campoTabla;
	int socketSwap = atoi((char*) dictionary_get(conexiones, "Swap"));
	for (i=0;i<list_size(listaTablaDePag);i++){
		campoTabla = list_get(listaTablaDePag,i);
		campoTabla->bitPresencia=0;
		if (campoTabla->bitPagModificada==1){
			t_contenido_pagina * escrituraSwap;
			escrituraSwap = iniciarContenidoPagina();
			escrituraSwap->PID = campoTabla->idProc;
			escrituraSwap->contenido = traerContenidoDeMarco(campoTabla->idMarco);
			escrituraSwap->numeroPagina = campoTabla->paginaDelProceso;
			enviarEscribirAlSwap(escrituraSwap, socketSwap);
		}
	}
	list_clean(listaMemoria);
}
void volcarMemoria(){
	int pid,a;
	pid = fork();
	if (pid<0) {
		log_error(logger,"Fallo la creación del proceso hijo");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		t_marco* campoMarco;
		char* texto = string_new();
		for (a=0;a<list_size(listaMemoria);a++){
			campoMarco= list_get(listaMemoria,a);
			string_append_with_format(&texto,"El marco %s ubicado en la posición %s contiene: %s /n",string_itoa(campoMarco->idMarco) ,string_itoa(campoMarco->posicion) ,campoMarco->contenido);
			log_info(logger,texto);
		}
		kill(pid,SIGTERM);
	}
	else {
		wait(NULL);
		log_info(logger,"El proceso hijo termino correctamente");
		exit(0);
	}

}
