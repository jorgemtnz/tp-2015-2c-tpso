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
//	printConsola("Se creo el proceso PID: %d\n", pcb->pid);
	return pcb;
}
t_cpu_ref* crearCpuRef(){
	t_cpu_ref* cpu = malloc(sizeof(t_cpu_ref));
	cpu->conectada = false;
	cpu->ejecutando = false;
	cpu->nombre = string_new();
	cpu->socket = 0;
	char* ruta = string_new();
	cpu->pcb = crearPcb(ruta);
	return cpu;
}

void ejecutarProceso(t_pcb* pcb) {
	list_add(getColaDeNuevos(), pcb);
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

t_list* getColaDeNuevos() {
	return colaDeNuevos;
}

void ejecutarPlanificadorLargoPlazo() {
	//TODO
	//POR AHORA ENVIAMOS DIRECTAMENTE
	if(list_all_satisfy(listaCPUs, cpuDesconectada)) {
		putsConsola("No hay ningun CPU conectado\n");
	} else {
		putsConsola("Ejecutamos planificacion\n");
	}

	//TODO
	//POR AHORA TOMAMOS EL PRIMERO DE LA COLA DE LISTOS Y LO MANDAMOS A EJECUTAR
	t_pcb* pcb = list_get(getColaDeNuevos(), 0);
	t_cpu_ref* cpu = obtenerCPUDisponible();
	if(cpu != NULL) {
		correrProcesoEnCpu(pcb, cpu);
	} else {
		log_info(logger, "No hay CPU disponible para ejecutar el proceso %d\n", pcb->pid);
	}
}

bool cpuDesconectada(void *cpu) {
	return ((t_cpu_ref *)cpu)->conectada == false;
}

t_cpu_ref* obtenerCPUDisponible() {

	bool estaCPUDisponible(void* elemento) {
		return !((t_cpu_ref*) elemento)->ejecutando;
	}

	t_cpu_ref* cpuDisponible = (t_cpu_ref*) list_find(listaCPUs, estaCPUDisponible);

	return cpuDisponible;
}

void correrProcesoEnCpu(t_pcb* pcb, t_cpu_ref* cpu) {
	cpu->ejecutando = true;
	cpu->pcb = pcb;
	enviarStruct(cpu->socket, CONTEXTO_MPROC, pcb);
}
