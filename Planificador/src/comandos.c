/*
 * comandos.c
 *
 *  Created on: Sep 20, 2015
 *      Author: utnso
 */

#include "Planificador.h"


int correrPrograma(int socket, t_header* header, char* buffer) {
	char* socketCPU = (char*)dictionary_get(conexiones, "CPU");
	putsConsola("Enviando \"correr programa\" a la CPU");
	//enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
	enviarStruct(atoi(socketCPU), STRING, "correr programa");

	return 0;
}

int correrPath(int socket, t_header* header, char* buffer) {
	putsConsola("comando: correr PATH");
	return 0;
}

int finalizarPid(int socket, t_header* header, char* buffer) {
	putsConsola("comando: finalizar PID");
	return 0;
}

int ps(int socket, t_header* header, char* buffer) {
	putsConsola("comando: ps");
	return 0;
}

int cpu(int socket, t_header* header, char* buffer) {
	putsConsola("comando: cpu");
	return 0;
}
