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
	putsConsola("=========== Inicio planificador ===========\n");
	imprimirTodo();
	putsConsola("\n");
	//TODO
	//POR AHORA ENVIAMOS DIRECTAMENTE
	if(list_all_satisfy(listaCPUs, cpuDesconectada)) {
		putsConsola("No hay ningun CPU conectado\n");
	} else {
		//putsConsola("Ejecutamos planificacion\n");
	}

	//entrada salida
	bool ejecutando;
	bool finalizado;
	t_pcb* pcbEnEntradaSalida;

	lockEstadoEntradaSalida();
	ejecutando = estadoEntradaSalida.pcb != NULL;
	pcbEnEntradaSalida = estadoEntradaSalida.pcb;
	finalizado = estadoEntradaSalida.finalizoEntradaSalida;
	unlockEstadoEntradaSalida();

	if(ejecutando) {
		printConsola("Proceso pid: %d en E/S\n", pcbEnEntradaSalida->pid);
		if(finalizado) {
			printConsola("E/S FINALIZADA\n", pcbEnEntradaSalida->pid);
			lockEstadoEntradaSalida();
			t_pcb* pcbEnEntradaSalida = estadoEntradaSalida.pcb;
			estadoEntradaSalida.cantidadCiclos = 0;
			estadoEntradaSalida.pcb = NULL;
			estadoEntradaSalida.finalizoEntradaSalida = false;
			unlockEstadoEntradaSalida();
			list_add(getColaDeListos(), pcbEnEntradaSalida);
			printConsola("El proceso pid: %d va a la cola de listos\n", pcbEnEntradaSalida->pid);
		} else {
			printConsola("E/S en ejecucion\n", pcbEnEntradaSalida->pid);
		}
	}

	lockEstadoEntradaSalida();
	ejecutando = estadoEntradaSalida.pcb != NULL;
	unlockEstadoEntradaSalida();

	if(!ejecutando) {
		//printConsola("E/S Disponible\n");
		bool hayEntradaSalidaEsperando = !list_is_empty(getColaDeEntradaSalida());
		if (hayEntradaSalidaEsperando) {
			t_pcb_entrada_salida* pcbEntradaSalida = (t_pcb_entrada_salida*)list_get(getColaDeEntradaSalida(), 0);
			if(pcbEntradaSalida != NULL) {
				list_remove(getColaDeEntradaSalida(), 0);
				lockEstadoEntradaSalida();
				estadoEntradaSalida.cantidadCiclos = pcbEntradaSalida->cantidadCiclos;
				estadoEntradaSalida.pcb = pcbEntradaSalida->pcb;
				estadoEntradaSalida.finalizoEntradaSalida = false;
				unlockEstadoEntradaSalida();

				//deja correr
				printConsola("Proceso pid: %d va a ejecutar su E/S\n", pcbEntradaSalida->pcb->pid);

				unlockHayEntradaSalidaParaEjecutar();
			}
		} else {
			//printConsola("No hay procesos esperando E/S\n");
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
			printConsola("Se envia a ejecutar el proceso %d en la cpu \"%s\" por el socket %d\n", pcb->pid, cpu->nombre, cpu->socket);
		} else {
//			printConsola("No hay CPU disponible para ejecutar el proceso %d\n", pcb->pid);
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
			printConsola("Se envia a ejecutar el proceso %d en la cpu \"%s\" por el socket %d\n", pcb->pid, cpu->nombre, cpu->socket);
		} else {
//			printConsola("No hay CPU disponible para ejecutar el proceso %d\n", pcb->pid);
		}
	}

	putsConsola("\n");
	imprimirTodo();
	putsConsola("=========== Fin planificador ===========\n");
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
		imprimirTodo();
		abort();
	}

	quitarProcesoDeCpu(cpu);
	list_add(getColaDeFinalizados(), pcb);
}

void ejecucionAColaDeListos(t_pcb* pcb) {
	t_cpu_ref* cpu = obtenerCPUEjecutandoPcb(pcb);
	if(cpu == NULL) {
		printConsola("Se enviar a cola de listos el proceso %d y no se estaba ejecutando\n", pcb->pid);
		imprimirTodo();
		abort();
	}

	quitarProcesoDeCpu(cpu);
	list_add(getColaDeListos(), pcb);
}

t_cpu_ref* obtenerCPUEjecutandoPcb(t_pcb* pcb) {
	return obtenerCPUEjecutandoPcbPorPid(pcb->pid);
}

t_cpu_ref* obtenerCPUEjecutandoPcbPorPid(uint8_t PID) {
	bool estaCPUEnUsoPorPcb(void* elemento) {
		t_cpu_ref* cpu = (t_cpu_ref*) elemento;
		return cpu->pcb != NULL && cpu->pcb->pid == PID;
	}

	t_cpu_ref* cpuEnUso = (t_cpu_ref*) list_find(listaCPUs, estaCPUEnUsoPorPcb);

	return cpuEnUso;
}

void quitarProcesoDeCpu(t_cpu_ref* cpu) {
	cpu->ejecutando = false;
	cpu->pcb = NULL;
}


bool conectadoAPadre = false;
int socketPadre;
bool finalizarProcesoEntradaSalida = false;

void *ejecutarEntradaSalida(void *param) {

	while (!finalizarProcesoEntradaSalida) {
		int cantidadCiclos;
		t_pcb* pcb;
		lockHayEntradaSalidaParaEjecutar();

		if(!conectadoAPadre) {
			conectar(string_duplicate("127.0.0.1"), configuracion->puertoEscucha,
						&socketPadre);
		}

		lockEstadoEntradaSalida();
		cantidadCiclos = estadoEntradaSalida.cantidadCiclos;
		pcb = estadoEntradaSalida.pcb;
		printConsola("Proceso pid: %d empieza su E/S\n", pcb->pid);
		unlockEstadoEntradaSalida();
		while(cantidadCiclos > 0) {
			//time_t t;
			//time(&t);
			//printConsola("\n current time is : %s ",ctime(&t));
			printConsola("Cantidad ciclos restantes %d\n", cantidadCiclos);
			usleep(1000000);
			lockEstadoEntradaSalida();
			estadoEntradaSalida.cantidadCiclos--;
			cantidadCiclos = estadoEntradaSalida.cantidadCiclos;
			unlockEstadoEntradaSalida();
		}

		lockEstadoEntradaSalida();
		estadoEntradaSalida.finalizoEntradaSalida = true;
		unlockEstadoEntradaSalida();

		printConsola("Proceso pid: %d finaliza su E/S\n", pcb->pid);
		enviarStruct(socketPadre, NOTIFICACION_HILO_ENTRADA_SALIDA, NULL);
	}

	putsConsola("Fin del hilo de entrada salida");
	return NULL;
}


void imprimirEstadoCpus() {
	int cantCpus = list_size(listaCPUs);
	int i = 0;
	for (i = 0; i < cantCpus; i++) {
		t_cpu_ref* cpu = list_get(listaCPUs, i);
		char* pid = cpu != NULL && cpu->pcb != NULL ? string_from_format("%i",cpu->pcb->pid) : "VACIO";
		printConsola("[Cpu: %s, pid: %s]\n", cpu->nombre, pid);
	}
	if(cantCpus == 0) {
		printConsola("[Cpus: Lista vacia]\n");
	}
}

void imprimirColaPcbs(t_list* colaPcb, char* prefijo) {
	int cantElem = list_size(colaPcb);
	int i = 0;
	for (i = 0; i < cantElem; i++) {
		t_pcb* pcb = list_get(colaPcb, i);
		char* pid = pcb != NULL ? string_from_format("%i",pcb->pid) : "VACIO";
		printConsola("%s[%d], pid: %s\n", prefijo, i, pid);
	}
	if(cantElem == 0) {
		printConsola("[Lista vacia de %s]\n", prefijo);
	}
}

void imprimirColaDeFinalizados() {
	t_list* colaPcb = getColaDeFinalizados();
//	printConsola("Estado Cola de finalizados\n");
	imprimirColaPcbs(colaPcb, "finalizado");
}

void imprimirProcesoEnEntradaSalida() {
	lockEstadoEntradaSalida();
	t_pcb* pcb = estadoEntradaSalida.pcb;
	if(pcb != NULL) {
		printConsola("[ejec. e/s: pid: %d]\n", pcb->pid);
	} else {
		printConsola("[ejec. e/s: disponible]\n");
	}
	unlockEstadoEntradaSalida();
}

void imprimirColaDeEntradaSalida() {
	t_list* colaPcb = getColaDeEntradaSalida();
//	printConsola("Estado Cola de Entrada Salida\n");
	imprimirColaPcbs(colaPcb, "espera e/s");
}

void imprimirColaDeListos() {
	t_list* colaPcb = getColaDeListos();
//	printConsola("Estado Cola de Listos\n");
	imprimirColaPcbs(colaPcb, "listos");
}
void imprimirColaDeNuevos() {
	t_list* colaPcb = getColaDeNuevos();
//	printConsola("Estado Cola de Nuevos\n");
	imprimirColaPcbs(colaPcb, "nuevos");
}

void imprimirTodo() {
	imprimirEstadoCpus();
	imprimirColaDeNuevos();
	imprimirColaDeListos();
	imprimirColaDeEntradaSalida();
	imprimirProcesoEnEntradaSalida();
	imprimirColaDeFinalizados();
}
