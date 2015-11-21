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
	pcb->finalizar = false;
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

t_list* getColaDeEntradaSalida() {
	return colaDeEntradaSalida;
}

t_list* getColaDeFinalizados() {
	return colaDeFinalizados;
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

	//entrada salida
	bool ejecutando;
	bool finalizado;
	t_pcb* pcbEnEntradaSalida;

	pthread_mutex_lock(&mutexEstadoEntradaSalida);
	ejecutando = estadoEntradaSalida.pcb != NULL;
	pcbEnEntradaSalida = estadoEntradaSalida.pcb;
	finalizado = estadoEntradaSalida.finalizoEntradaSalida;
	pthread_mutex_unlock(&mutexEstadoEntradaSalida);

	if(ejecutando) {
		printConsola("Proceso pid: %d en E/S\n", pcbEnEntradaSalida->pid);
		if(finalizado) {
			printConsola("E/S FINALIZADA\n", pcbEnEntradaSalida->pid);
			pthread_mutex_lock(&mutexEstadoEntradaSalida);
			t_pcb* pcbEnEntradaSalida = estadoEntradaSalida.pcb;
			estadoEntradaSalida.cantidadCiclos = 0;
			estadoEntradaSalida.pcb = NULL;
			estadoEntradaSalida.finalizoEntradaSalida = false;
			pthread_mutex_unlock(&mutexEstadoEntradaSalida);
			list_add(getColaDeListos(), pcbEnEntradaSalida);
			printConsola("El proceso pid: %d va a la cola de listos\n", pcbEnEntradaSalida->pid);
		} else {
			printConsola("E/S en ejecucion\n", pcbEnEntradaSalida->pid);
		}
	}

	pthread_mutex_lock(&mutexEstadoEntradaSalida);
	ejecutando = estadoEntradaSalida.pcb != NULL;
	pthread_mutex_unlock(&mutexEstadoEntradaSalida);

	if(!ejecutando) {
		printConsola("E/S Disponible\n");
		bool hayEntradaSalidaEsperando = !list_is_empty(getColaDeEntradaSalida());
		if (hayEntradaSalidaEsperando) {
			t_pcb_entrada_salida* pcbEntradaSalida = (t_pcb_entrada_salida*)list_get(getColaDeEntradaSalida(), 0);
			if(pcbEntradaSalida != NULL) {
				list_remove(getColaDeEntradaSalida(), 0);
				pthread_mutex_lock(&mutexEstadoEntradaSalida);
				estadoEntradaSalida.cantidadCiclos = pcbEntradaSalida->cantidadCiclos;
				estadoEntradaSalida.pcb = pcbEntradaSalida->pcb;
				estadoEntradaSalida.finalizoEntradaSalida = false;
				pthread_mutex_unlock(&mutexEstadoEntradaSalida);

				//deja correr
				printConsola("Proceso pid: %d va a ejecutar su E/S\n", pcbEntradaSalida->pcb->pid);

				pthread_mutex_unlock(&mutexHayEntradaSalidaParaEjecutar);
			}
		} else {
			printConsola("No hay procesos esperando E/S\n");
		}
	}



	//TODO
	//POR AHORA TOMAMOS EL PRIMERO DE LA COLA DE NUEVOS Y LO MANDAMOS A EJECUTAR
	t_pcb* pcb = list_get(getColaDeNuevos(), 0);
	if (pcb != NULL) {
		t_cpu_ref* cpu = obtenerCPUDisponible();
		if(cpu != NULL) {
			list_remove(getColaDeNuevos(), 0);
			correrProcesoEnCpu(pcb, cpu);
			printConsola("Se envia a ejecutar el proceso %d\n", pcb->pid);
		} else {
			printConsola("No hay CPU disponible para ejecutar el proceso %d\n", pcb->pid);
		}
	}

	//TODO
	//POR AHORA TOMAMOS EL PRIMERO DE LA COLA DE LISTOS Y LO MANDAMOS A EJECUTAR
	pcb = list_get(getColaDeListos(), 0);
	if (pcb != NULL) {
		t_cpu_ref* cpu = obtenerCPUDisponible();
		if(cpu != NULL) {
			list_remove(getColaDeListos(), 0);
			correrProcesoEnCpu(pcb, cpu);
			printConsola("Se envia a ejecutar el proceso %d\n", pcb->pid);
		} else {
			printConsola("No hay CPU disponible para ejecutar el proceso %d\n", pcb->pid);
		}
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


void ejecucionAFinalizado(t_pcb* pcb) {
	t_cpu_ref* cpu = obtenerCPUEjecutandoPcb(pcb);
	if(cpu == NULL) {
		printConsola("Se intento finalizar el proceso %d y no se estaba ejecutando\n", pcb->pid);
		abort();
	}

	quitarProcesoDeCpu(cpu);
	list_add(getColaDeFinalizados(), pcb);
}

void ejecucionAColaDeListos(t_pcb* pcb) {
	t_cpu_ref* cpu = obtenerCPUEjecutandoPcb(pcb);
	if(cpu == NULL) {
		printConsola("Se intento finalizar el proceso %d y no se estaba ejecutando\n", pcb->pid);
		abort();
	}

	quitarProcesoDeCpu(cpu);
	list_add(getColaDeListos(), pcb);
}

t_cpu_ref* obtenerCPUEjecutandoPcb(t_pcb* pcb) {
	bool estaCPUEnUsoPorPcb(void* elemento) {
		t_cpu_ref* cpu = (t_cpu_ref*) elemento;
		return cpu->pcb != NULL && cpu->pcb->pid == pcb->pid;
	}

	t_cpu_ref* cpuEnUso = (t_cpu_ref*) list_find(listaCPUs, estaCPUEnUsoPorPcb);

	return cpuEnUso;
}

void quitarProcesoDeCpu(t_cpu_ref* cpu) {
	cpu->ejecutando = false;
	cpu->pcb = NULL;
}
