/*
 * comandos.c
 *
 *  Created on: Sep 20, 2015
 *      Author: utnso
 */

#include "Planificador.h"

int correrPrograma(int socket, t_header* header, char* buffer) {
	//char* socketCPU = (char*) dictionary_get(conexiones, "CPU");
	//putsConsola("Enviando \"correr programa\" a la CPU");
	//enviarStruct(atoi(socketCPU), STRING, "correr programa");

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

	uint8_t pid = atoi(split[1]);

	if (!existePID(pid)) {
		printConsola("El PID especificado es invalido: %i\n", pid);
		return -1;
	}

	t_pcb* pcbEnEntradaSalida;
	pthread_mutex_lock(&mutexEstadoEntradaSalida);

	pcbEnEntradaSalida = estadoEntradaSalida.pcb;
	pthread_mutex_unlock(&mutexEstadoEntradaSalida);

	int a;
	t_cpu_ref * cpu = crearCpuRef();
	char* ruta = string_new();
	t_pcb* pcb = crearPcb(ruta);
	for (a = 0; a < list_size(listaCPUs); a++) {
		cpu = list_get(listaCPUs, a);
		if (cpu->pcb != NULL) {
			if (cpu->pcb->pid == pid) {
				list_remove(listaCPUs, a);
				cpu->pcb->finalizar = true;
				list_add_in_index(listaCPUs, a, cpu);
				a = list_size(listaCPUs) + 1;
			}
		}
	}

	for (a = 0; a < list_size(colaDeListos); a++) {
		pcb = list_get(colaDeListos, a);

		if (pcb->pid == pid) {
			list_remove(colaDeListos, a);
			pcb->proximaInstruccion = pcb->instruccionFinal;
			list_add_in_index(colaDeListos, a, pcb);

			a = list_size(colaDeListos) + 1;

		}
	}
	for (a = 0; a < list_size(colaDeEntradaSalida); a++) {
		pcb = list_get(colaDeEntradaSalida, a);

		if (pcb->pid == pid) {
			list_remove(colaDeEntradaSalida, a);
			pcb->proximaInstruccion = pcb->instruccionFinal;
			list_add_in_index(colaDeEntradaSalida, a, pcb);
			list_add(colaDeListos, pcb);
			a = list_size(colaDeEntradaSalida) + 1;

		}
	}

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
		if (cpu->ejecutando) {
			char** splitRuta = string_split(cpu->pcb->rutaArchivoMcod, "/");
			while (splitRuta[b] != NULL) {
				cont++;
				b++;
			}

			printf("mProc %i: %s -> Ejecutando\n", cpu->pcb->pid, splitRuta[cont - 1]);
		}
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
	t_pcb_entrada_salida* pcbES = malloc(sizeof(t_pcb_entrada_salida));
	for (a = 0; a < list_size(colaDeEntradaSalida); a++) {
		pcbES = list_get(colaDeEntradaSalida, a);

		char** splitRuta = string_split(pcbES->pcb->rutaArchivoMcod, "/");
		while (splitRuta[b] != NULL) {
			cont++;
			b++;
		}
		printf("mProc %i: %s -> Bloqueado\n", pcbES->pcb->pid, splitRuta[cont - 1]);
	}

	return 0;
}

int cpu(int socket, t_header* header, char* buffer) {
	putsConsola("comando: cpu");
	t_PID* pid = malloc(sizeof(t_PID));
	pid->PID = 3;
	t_cpu_ref * cpu = crearCpuRef();
	cpu = list_get(listaCPUs, 0);
	enviarStruct(cpu->socket, TIEMPO_CPU, pid);

	return 0;
}
