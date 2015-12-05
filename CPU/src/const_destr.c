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
		abort();
	}
	respEjec->resultadosInstrucciones = string_new();
	respEjec->pcb = NULL;
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
		abort();
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
		abort();
	}
	mCod->cantidadInstrucciones = 0;
	mCod->respEjec = creaRespuestaEjecucion();
//	printf("se crea un mCod\n");
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
		abort();
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
		abort();
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
	cPUHilo->inicioInstruccion=NULL;
	cPUHilo->acumuladoSegundos=0;
//	time(&cPUHilo->finInstruccion);
//	cPUHilo->retardoTotal =0;
//	cPUHilo->terminaInstruccion = SI_TERMINO;
//	cPUHilo->inicioInstruccion = malloc(sizeof(time_t));
//	cPUHilo->finInstruccion=malloc (sizeof(time_t));
	cPUHilo->quantumReloj = 0;
	cPUHilo->actualPID=-1;
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
		abort();
	}
	procCPU->contadorIdCPU = 0;
	procCPU->listaCPU = list_create();

	return procCPU;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------FUNCIONES DESTRUCTORAS------------------------------
void destmCod(t_mCod* unmCod) {
	if (unmCod != NULL) {
		destRespEjec(unmCod->respEjec);
		unmCod->respEjec = NULL;
		destVectorInstruccion(unmCod->bufferInstrucciones);
		unmCod->bufferInstrucciones = NULL;
		free(unmCod);
	}
}

void destConfig(t_configuracion* unaConfig) {
	if (unaConfig != NULL)
		free(unaConfig);
}

void destHiloCPU(t_cpu* unHiloCPU) {
	if (unHiloCPU != NULL) {
		free(unHiloCPU->nombre);
		free(unHiloCPU->estructuraSolicitud);
		free(unHiloCPU->respuestaInstruccion);
		destPCB(unHiloCPU->pcbPlanificador);
		destmCod(unHiloCPU->mCodCPU);
		unHiloCPU->mCodCPU = NULL;
		free(unHiloCPU);
	}
}

void destProcCPU(t_ProcCPU* unCPU) {
	if (unCPU != NULL) {
		list_destroy_and_destroy_elements(unCPU->listaCPU, (void*) destHiloCPU);
		free(unCPU);
	}
}

void destInstruccion(t_instruccion* unaInstruccion) {
	if (unaInstruccion != NULL)
		free(unaInstruccion);
}

void destVectorInstruccion(char** vectorInstruccion) {
	if (vectorInstruccion != NULL) {
		int i = 0;
		while (vectorInstruccion[i] != NULL) {
			free(vectorInstruccion[i]);
			i++;
		}
		free(vectorInstruccion);
	}
}

void destIniciarSwap(t_iniciar_swap* estructura) {
	if (estructura != NULL)
		free(estructura);
}

void destEscrMem(t_contenido_pagina* estruc) {
	if (estruc != NULL)
		free(estruc);
}

void destRespEjec(t_respuesta_ejecucion* respEjec) {
	if (respEjec != NULL) {
		free(respEjec->resultadosInstrucciones);
		respEjec->resultadosInstrucciones = NULL;
		free(respEjec);
	}
}

void destPCB(t_pcb* pcb) {
	if (pcb != NULL) {
		free(pcb->rutaArchivoMcod);
		pcb->rutaArchivoMcod = NULL;
		free(pcb);

	}
}
