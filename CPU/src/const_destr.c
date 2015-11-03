#include "CPU.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------FUNCIONES CONTRUCTORAS-------------------------

t_respuesta_ejecucion* creaRespuestaEjecucion() {
	t_respuesta_ejecucion* respEjec = malloc(sizeof(t_respuesta_ejecucion));
	if (respEjec == NULL) {
		perror("[ERROR] No se reservo memoria respuesta ejecucion CPU");
		log_error(logger,
				"[ERROR] No se reservo memoria respuesta ejecucion CPU");
		exit(-1);
	}
	respEjec->resultadosInstrucciones = string_new();
//	respEjec->pcb = crearPcb();
	respEjec->finalizoOk = false;
	return respEjec;
}

t_instruccion* creaInstruccion() {
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	if (instruccion == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>instruccion");
		log_error(logger,
				"[ERROR] No se reservo memoria para CPU>..>instruccion");
		exit(-1);
	}
	return instruccion;
}

t_mCod* crearmCod() {
	t_mCod* mCod = malloc(sizeof(t_mCod));
	if (mCod == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>mCod");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>mCod");
		exit(-1);
	}
	mCod->cantidadInstrucciones = 0;
	mCod->respEjec = creaRespuestaEjecucion();
	return mCod;
}

t_configuracion* crearConfiguracion() {
	t_configuracion* configuracion = malloc(sizeof(t_configuracion));
	if (configuracion == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>configuracion");
		log_error(logger,
				"[ERROR] No se reservo memoria para CPU>..>configuracion");
		exit(-1);
	}
	configuracion->cantidad_hilos = 0;
	configuracion->retardo = 0;
	configuracion->vg_ipMemoria = '\0';
	configuracion->vg_ipPlanificador = '\0';
	configuracion->vg_puertoMemoria = 0;
	configuracion->vg_puertoPlanificador = 0;

	return configuracion;
}

t_pcb * crearPcb() {
	t_pcb* pcb = malloc(sizeof(t_pcb));
	if (pcb == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>pcb");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>pcb");
		exit(-1);
	}
	pcb->pid = 0;
	pcb->proximaInstruccion = 0;
	pcb->rutaArchivoMcod = '\0';
	pcb->tamanioRafaga = 0;
	pcb->tieneDesalojo = true;
	return pcb;
}

t_cpu* crearCPU() {
	t_cpu* cPUHilo = malloc(sizeof(t_cpu));
	if (cPUHilo == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		exit(-1);
	} //si esta vacia la lista
	int token = procCPU->contadorIdCPU;
	switch (token) {
	case 0: {
		cPUHilo->idCPU = procCPU->contadorIdCPU;
		cPUHilo->nombre = strdup("CPU primera");
		procCPU->contadorIdCPU++;
		break;
	}
	case 1: {
		cPUHilo->idCPU = procCPU->contadorIdCPU;
		cPUHilo->nombre = strdup("CPU segunda");
		procCPU->contadorIdCPU++;
		break;
	}
	case 2: {
		cPUHilo->idCPU = procCPU->contadorIdCPU;
		cPUHilo->nombre = strdup("CPU tercera");
		procCPU->contadorIdCPU++;
		break;
	}
	case 3: {
		cPUHilo->idCPU = procCPU->contadorIdCPU;
		cPUHilo->nombre = strdup("CPU cuarta");
		procCPU->contadorIdCPU++;
		break;
	}
	default:
		cPUHilo->idCPU = procCPU->contadorIdCPU;
		cPUHilo->nombre = strdup("CPU mayor a 4");
		procCPU->contadorIdCPU++;

	}
	cPUHilo->porcentajeUso = 0;
	cPUHilo->estadoEjecucion = NO_USO;
	cPUHilo->cantInstEjecutadas = 0;
	cPUHilo->mCodCPU = crearmCod();
	cPUHilo->pcbPlanificador = crearPcb();
	cPUHilo->respuestaInstruccion = NULL;
	cPUHilo->estructuraSolicitud = NULL;
	return cPUHilo;
}

t_ProcCPU* crearProcCPU() {

	t_ProcCPU* procCPU = malloc(sizeof(t_ProcCPU));
	if (procCPU == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>procCPU");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>procCPU");
		exit(-1);
	}
	procCPU->contadorIdCPU = 0;
	procCPU->listaCPU = list_create();

	return procCPU;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------FUNCIONES DESTRUCTORAS------------------------------
void destmCod(t_mCod* unmCod) {
//	eliminarEspacioDeDatos(unmCod->ptrCMemoriaMap, unmCod->ptrTamPagina,
//			logger);
	destRespEjec(unmCod->respEjec); // elimina y hace free a todos los elementos de la lista
	free(unmCod);
}

void destConfig(t_configuracion* unaConfig) {
	free(unaConfig);
}

void destHiloCPU(t_cpu* unHiloCPU) {
	destmCod(unHiloCPU->mCodCPU);
	free(unHiloCPU);
}

void destProcCPU(t_ProcCPU* unCPU) {
	list_destroy_and_destroy_elements(unCPU->listaCPU, (void*) destHiloCPU);
	free(unCPU);
}

void destInstruccion(t_instruccion* unaInstruccion) {
	free(unaInstruccion);
}

void destVectorInstruccion(char** vectorInstruccion) {
	free(vectorInstruccion);
}

void destIniciarSwap(t_iniciar_swap* estructura) {
	free(estructura);
}

void destEscrMem(t_contenido_pagina* estruc) {
	free(estruc);
}

void destEntradSalid(t_entrada_salida* entradSalid) {
	free(entradSalid);
}

void destRespEjec(t_respuesta_ejecucion* respEjec) {
	free(respEjec);
}
