/*
 * funcionesConsola.c
 *
 *  Created on: 5/11/2015
 *      Author: utnso
 */
#include "Memoria.h"

void levantarConsola() {
	char comando[20];
	uint8_t idFunc;

	mostrarComandos();
	while (1) {
		puts("Ingrese un comando >> ");
		fgets(comando, 20 ,stdin);
		//scanf("%s", comando);
		idFunc = idFuncion(comando);
		aplicarFuncion(idFunc);
	}
}

uint8_t idFuncion(char* funcion) {
	uint8_t i;
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

	uint8_t contador = 0;
	while (contador <= 2) {
		printf("*------------------------------------------*\n");
		printf("COMANDO 	= %s\n", funcionesConsola[contador]);
		printf("DESCRIPCION = %s\n", descripcionesConsola[contador]);
		printf("*------------------------------------------*\n");
		contador += 1;
	}
}

int procesarMensajesConsola(int socket, t_header* header, char* buffer) {
	pid_t pid=getpid();
	if(buffer != NULL && strstr(buffer, "\n")) {
		if(string_starts_with(buffer, "\n")){
			buffer = "";
		} else {
			char** split = string_split(buffer, "\n");
			buffer = split[0];
		}
	}
	if(string_equals(buffer, "TLB_FLUSH")) {
		my_log_info("Recibo señal SIGUSR1\n");
		puts("");
		kill(pid,SIGUSR1);
	} else if(string_equals(buffer, "MEM_FLUSH")) {
		my_log_info("Recibo señal SIGUSR2\n");
		puts("");
		kill(pid,SIGUSR2);
	} else if(string_equals(buffer, "MEM_DUMP")) {
		my_log_info("Recibo señal SIGPOLL\n");
		puts("");
		kill(pid,SIGPOLL);
	} else {
		puts("Comando no reconocido");
	}
	return 0;
}
void limpiarTLB(){
	if (configuracion->tlbHabilitada==1){
		pthread_mutex_lock(&mutexListaTLB);
		list_clean(listaTLB);
		pthread_mutex_unlock(&mutexListaTLB);
	} else {
		my_log_info("TLB inactiva\n");
		puts("");
	}
}
void limpiarMemoria(){
	uint8_t i;
	t_TablaDePaginas* campoTabla;
	int socketSwap = atoi((char*) dictionary_get(conexiones, "Swap"));
	pthread_mutex_lock(&mutexTablaPags);
	for (i=0;i<list_size(listaTablaDePag);i++){
		campoTabla = list_get(listaTablaDePag,i);
		campoTabla->bitPresencia=0;
		if (campoTabla->bitPagModificada==1){
			t_sobreescribir_swap* estructura;
			estructura = crearEstructuraReemplazar();
			estructura->PIDAResponderleAMemoria = 236; // EESTE CAMPO ES EL QUE VERIFICA SWAP
			estructura->PIDAReemplazar = campoTabla->idProc;
			estructura->contenido = traerContenidoDeMarco(campoTabla->idMarco);
			estructura->numeroPagina = campoTabla->paginaDelProceso;
			enviarEscribirAlSwap(estructura, socketSwap);
		}
		campoTabla->bitPagModificada=0;
		reemplazar_tablaDePag(i,campoTabla);

	}
	pthread_mutex_unlock(&mutexTablaPags);
	pthread_mutex_lock(&mutexListaMemoria);
	list_clean(listaMemoria);
	pthread_mutex_unlock(&mutexListaMemoria);
	pthread_mutex_lock(&mutexListaTLB);
	list_clean(listaTLB);
	pthread_mutex_unlock(&mutexListaTLB);
}
void volcarMemoria(){
	my_log_info("Volcado de memoria iniciado\n");
	puts("");
	uint8_t pid,a;
	pid = fork();
	if (pid<0) {
		puts(string_itoa(errno));
		my_log_error("Fallo la creación del proceso hijo\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) {
		t_marco* campoMarco;
		pthread_mutex_lock(&mutexListaMemoria);
		if (list_is_empty(listaMemoria)){
			my_log_info("Memoria vacía\n");
			puts("");
			exit(EXIT_SUCCESS);
		}
		for (a=0;a<list_size(listaMemoria);a++){
			campoMarco= list_get(listaMemoria,a);
			my_log_info(string_from_format("El marco %s ubicado en la posición %s contiene: %s\n"
					,string_itoa(campoMarco->idMarco) ,string_itoa(campoMarco->posicion) ,campoMarco->contenido));
			puts("");
		}
		pthread_mutex_unlock(&mutexListaMemoria);
		exit(EXIT_SUCCESS);
	}
	else {
		wait(EXIT_SUCCESS);
		my_log_info("Volcado de memoria finalizado\n");
		puts("");
	}
}

void atencionSIGUSR1(){
	pthread_t hilo1;
	my_log_info("Limpieza de TLB iniciada\n");
	puts("");
	pthread_create(&hilo1,NULL,(void*) limpiarTLB,NULL);
	pthread_join(hilo1,NULL);
	my_log_info("Limpieza de TLB finalizada\n");
	puts("");
}

void atencionSIGUSR2(){
	pthread_t hilo2;
	my_log_info("Limpieza de Memoria iniciada\n");
	puts("");
	pthread_create(&hilo2,NULL,(void*) limpiarMemoria,NULL);
	pthread_join(hilo2,NULL);
	my_log_info("Limpieza de Memoria finalizada\n");
	puts("");
}
