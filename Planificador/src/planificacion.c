/*
 * planificacion.c
 *
 *  Created on: Sep 20, 2015
 *      Author: utnso
 */
#include "Planificador.h"

t_pcb* crearPcb(char* rutaArchivoMcod) {
	t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->pid = crearPid();
	pcb->rutaArchivoMcod = rutaArchivoMcod;
	pcb->tieneDesalojo = planificacion->tieneDesalojo;
	pcb->tamanioRafaga = planificacion->tamanioRafaga;
	pcb->proximaInstruccion = 0;
	printConsola("Se creo el proceso PID: %d\n", pcb->pid);
	return pcb;
}

void agregarPcbAColaDeListos(t_pcb* pcb) {
	list_add(getColaDeListos(), pcb);
	ejecutarPlanificadorLargoPlazo();
}

uint8_t crearPid() {
	if(proximoPid == NULL) {
		proximoPid = malloc(sizeof(uint8_t));
		*proximoPid = 0;
	}
	uint8_t pid = *proximoPid;
	*proximoPid = *proximoPid + 1;
	return pid;
}

t_list* getColaDeListos() {
	return colaDeListos;
}

void ejecutarPlanificadorLargoPlazo() {
	//TODO
	//POR AHORA ENVIAMOS DIRECTAMENTE
	if(list_all_satisfy(listaCPUs, cpuDesconectada)) {
		putsConsola("No hay ningun CPU conectado\n");
	} else {
		putsConsola("Enviamos a ejecutar el programa\n");
	}

	//TODO
	//POR AHORA TOMAMOS EL PRIMERO DE LA COLA DE LISTOS Y LO MANDAMOS A EJECUTAR
	t_pcb* pcb = list_get(colaDeListos, 0);
	t_cpu_ref* cpu = obtenerCPUDisponible();
	correrProcesoEnCpu(pcb, cpu);
}

bool cpuDesconectada(void *cpu) {
	return ((t_cpu_ref *)cpu)->conectada == false;
}

t_cpu_ref* obtenerCPUDisponible() {
	return (t_cpu_ref*)list_get(listaCPUs, 0);
}

void correrProcesoEnCpu(t_pcb* pcb, t_cpu_ref* cpu) {
	enviarStruct(cpu->socket, CONTEXTO_MPROC, pcb);
}
