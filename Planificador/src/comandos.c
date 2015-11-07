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

	char** split = string_split(buffer, " ");

	if(!string_equals(split[0], "correr")) {
		printConsola("Comando no valido: %s\n", split[0]);
		return -1;
	}

	if(!existeArchivo(split[1])) {
		printConsola("El archivo especificado no existe: %s\n", split[1]);
		return -1;
	}

	int fdArchivo = abrirArchivoSoloLectura(split[1], logger);
	if(fdArchivo < 0) {
		printConsola("El archivo especificado no se pudo abrir: %s\n", split[1]);
		return -1;
	} else {
		cerrarArchivoEspacioDeDatos(fdArchivo, logger);
	}

	t_pcb* pcb = crearPcb(split[1]);

	ejecutarProceso(pcb);

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
