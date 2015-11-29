#include "CPU.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------FUNCIONES CONTRUCTORAS-------------------------

t_respuesta_ejecucion* creaRespuestaEjecucion() {

	t_respuesta_ejecucion* respEjec = malloc(sizeof(t_respuesta_ejecucion));
	if (respEjec == NULL) {
		perror("[ERROR] No se reservo memoria respuesta ejecucion CPU");
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger,
				"[ERROR] No se reservo memoria respuesta ejecucion CPU");
		pthread_mutex_unlock(&mutexCPULogs);
		exit(-1);
	}
	respEjec->resultadosInstrucciones = string_new();

	return respEjec;
}

t_instruccion* creaInstruccion() {
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	if (instruccion == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>instruccion");
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger,
				"[ERROR] No se reservo memoria para CPU>..>instruccion");
		pthread_mutex_unlock(&mutexCPULogs);
		exit(-1);
	}
	return instruccion;
}

t_mCod* crearmCod() {
	t_mCod* mCod = malloc(sizeof(t_mCod));
	if (mCod == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>mCod");
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>mCod");
		pthread_mutex_unlock(&mutexCPULogs);
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
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger,
				"[ERROR] No se reservo memoria para CPU>..>configuracion");
		pthread_mutex_unlock(&mutexCPULogs);
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

t_cpu* crearCPU() {
	t_cpu* cPUHilo = malloc(sizeof(t_cpu));
	if (cPUHilo == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		pthread_mutex_unlock(&mutexCPULogs);
		exit(-1);
	} //si esta vacia la lista
	int token = procCPU->contadorIdCPU;
	switch (token) {
	case 0: {
		cPUHilo->idCPU = queHiloSoy();
		cPUHilo->nombre = strdup("CPU primera");
		procCPU->contadorIdCPU += 1;
		break;
	}
	case 1: {
		cPUHilo->idCPU = queHiloSoy();
		cPUHilo->nombre = strdup("CPU segunda");
		procCPU->contadorIdCPU += 1;
		break;
	}
	case 2: {
		cPUHilo->idCPU = queHiloSoy();
		cPUHilo->nombre = strdup("CPU tercera");
		procCPU->contadorIdCPU += 1;
		break;
	}
	case 3: {
		cPUHilo->idCPU = queHiloSoy();
		cPUHilo->nombre = strdup("CPU cuarta");
		procCPU->contadorIdCPU += 1;
		break;
	}

	case 4: {
		cPUHilo->idCPU = queHiloSoy();
		cPUHilo->nombre = strdup("CPU quinta");
		procCPU->contadorIdCPU += 1;
		break;
	}

	default: {

		cPUHilo->idCPU = queHiloSoy();
		procCPU->contadorIdCPU += 1;
		cPUHilo->nombre = string_from_format("CPU %d", procCPU->contadorIdCPU);
	}
	}

	cPUHilo->porcentajeUso = 0;
	cPUHilo->terminaInstruccion = SI_TERMINO;
	cPUHilo->cantInstEjecutadasPorcentaje = 0;
	cPUHilo->mCodCPU = crearmCod();
	cPUHilo->respuestaInstruccion = NULL;
	cPUHilo->estructuraSolicitud = NULL;
	cPUHilo->estado = SI_TERMINO_RAFAGA;
//	time(&cPUHilo->inicioInstruccion);
//	time(&cPUHilo->finInstruccion);
//	cPUHilo->retardoTotal =0;
//	cPUHilo->terminaInstruccion = SI_TERMINO;
//	cPUHilo->inicioInstruccion = malloc(sizeof(time_t));
//	cPUHilo->finInstruccion=malloc (sizeof(time_t));
	cPUHilo->quantumReloj = 0;
	return cPUHilo;
}

t_ProcCPU* crearProcCPU() {

	t_ProcCPU* procCPU = malloc(sizeof(t_ProcCPU));
	if (procCPU == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>procCPU");
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>procCPU");
		pthread_mutex_unlock(&mutexCPULogs);
		exit(-1);
	}
	procCPU->contadorIdCPU = 0;
	procCPU->listaCPU = list_create();

	return procCPU;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------FUNCIONES DESTRUCTORAS------------------------------
void destmCod(t_mCod* unmCod) {
	destRespEjec(unmCod->respEjec);
	destVectorInstruccion(unmCod->bufferInstrucciones);
	if (unmCod != NULL) {
		free(unmCod);
	}
}

void destConfig(t_configuracion* unaConfig) {
	free(unaConfig);
}

void destHiloCPU(t_cpu* unHiloCPU) {
	free(unHiloCPU->nombre);
	free(unHiloCPU->estructuraSolicitud);
	free(unHiloCPU->respuestaInstruccion);
//	free(unHiloCPU->pcbPlanificador);
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
	int i = 0;
	while (vectorInstruccion[i] != NULL) {
		free(vectorInstruccion[i]);
		i++;
	}
	free(vectorInstruccion);
}

void destIniciarSwap(t_iniciar_swap* estructura) {
	free(estructura);
}

void destEscrMem(t_contenido_pagina* estruc) {
	free(estruc);
}

void destRespEjec(t_respuesta_ejecucion* respEjec) {
	free(respEjec->resultadosInstrucciones);
	free(respEjec);
}

void destPCB(t_pcb* pcb){
	free(pcb->rutaArchivoMcod);
	free(pcb);

}
