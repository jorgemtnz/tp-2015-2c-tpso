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
	if(list_size(listaCPUs) == 0) {
		putsConsola("No hay ningun CPU conectado\n");
	} else {
		putsConsola("Enviamos a ejecutar el programa\n");
	}
}

