/*
 * comandos.c
 *
 *  Created on: Sep 20, 2015
 *      Author: utnso
 */

#include "Planificador.h"

int correrPrograma(int socket, t_header* header, char* buffer) {
	char* socketCPU = (char*) dictionary_get(conexiones, "CPU");
	putsConsola("Enviando \"correr programa\" a la CPU");
	//enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
	enviarStruct(atoi(socketCPU), STRING, "correr programa");

	return 0;
}

int correrPath(int socket, t_header* header, char* buffer) {
	putsConsola("comando: correr PATH");

	char** split = string_split(buffer, " ");

	if (!string_equals(split[0], "correr")) {
		printConsola("Comando no valido: %s\n", split[0]);
		return -1;
	}

	if (!existeArchivo(split[1])) {
		printConsola("El archivo especificado no existe: %s\n", split[1]);
		return -1;
	}

	int fdArchivo = abrirArchivoSoloLectura(split[1], logger);
	if (fdArchivo < 0) {
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
	char** split = string_split(buffer, " ");

	if (!string_equals_ignore_case(split[0], "finalizar")) {
		printConsola("Comando no valido: %s\n", split[0]);
		return -1;
	}

	int pid = atoi(split[1]);

	if (!existePID(pid)) {
		printConsola("El PID especificado es invalido: %i\n", pid);
		return -1;
	}

	int a;
	t_cpu_ref * cpu = crearCpuRef();
	char* ruta = string_new();
	t_pcb* pcb = crearPcb(ruta);
	for (a = 0; a < list_size(listaCPUs); a++) {
		cpu = list_get(listaCPUs, a);
		if (cpu->pcb->pid == pid) {
			a = list_size(listaCPUs) + 1;
		}
	}
	cpu->pcb->proximaInstruccion = cpu->pcb->instruccionFinal;

	for (a = 0; a < list_size(colaDeEntradaSalida); a++) {
		pcb = list_get(colaDeEntradaSalida, a);

		if (pcb->pid == pid) {
			a = list_size(colaDeEntradaSalida) + 1;

		}
	}
	pcb->proximaInstruccion = pcb->instruccionFinal;

	return 0;
}

int ps(int socket, t_header* header, char* buffer) {
	putsConsola("comando: ps");
	int a, b, cont;
	cont = 0;
	b = 0;
	t_cpu_ref * cpu = crearCpuRef();
	//EJECUTANDO
	for (a = 0; a < list_size(listaCPUs); a++) {
		cpu = list_get(listaCPUs, a);
		char** splitRuta = string_split(cpu->pcb->rutaArchivoMcod, "/");
		while (splitRuta[b] != NULL) {
			cont++;
			b++;
		}
		printf("mProc %i: %s -> Ejecutando\n", cpu->pcb->pid, splitRuta[cont - 1]);
	}
	//LISTO
	char* ruta = string_new();
	t_pcb* pcb = crearPcb(ruta);
	b = 0;
	cont = 0;
	for (a = 0; a < list_size(colaDeListos); a++) {
		pcb = list_get(colaDeListos, a);
		char** splitRuta = string_split(pcb->rutaArchivoMcod, "/");
		while (splitRuta[b] != NULL) {
			cont++;
			b++;
		}
		printf("mProc %i: %s -> Listo\n", pcb->pid, splitRuta[cont - 1]);
	}
	//BLOQUEADOS

	b = 0;
	cont = 0;
	for (a = 0; a < list_size(colaDeEntradaSalida); a++) {
		pcb = list_get(colaDeEntradaSalida, a);
		char** splitRuta = string_split(pcb->rutaArchivoMcod, "/");
		while (splitRuta[b] != NULL) {
			cont++;
			b++;
		}
		printf("mProc %i: %s -> Bloqueado\n", pcb->pid, splitRuta[cont - 1]);
	}

	return 0;
}

int cpu(int socket, t_header* header, char* buffer) {
	putsConsola("comando: cpu");
	return 0;
}
