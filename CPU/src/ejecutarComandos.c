#include "CPU.h"
//nota
//no se cuenta una instruccion ejecutada hasta que no regreso de memoria la respuesta, cuando corresponda
void ejecuta_IniciarProceso(char** separada_instruccion, t_cpu* cpu) {
	cpu->estadoEjecucion = USO;
	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura->cantidadPaginas = atoi(separada_instruccion[1]);
	cpu->estructuraSolicitud = (t_iniciar_swap*) estructura;
	cpu->pcbPlanificador->proximaInstruccion++;
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger,
			string_from_format("Id del proceso %i \n", estructura->PID));
	log_info(logger,
			string_from_format(
					"instruccion ejecutada: iniciar. Cantidad de paginas, %i \n",
					estructura->cantidadPaginas));
	pthread_mutex_unlock(&mutexCPULogs);

	sleep(configuracion->retardo);
}
//mandar comando a  memoria con los datos y la pagina donde debe ser escrita
void ejecuta_EscribirMemoria(char** separada_instruccion, t_cpu* cpu) {
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
	cpu->estadoEjecucion = USO;
	//printf("BBBBBBBBBBBB %s \n",separada_instruccion)
	estructura->numeroPagina = atoi(separada_instruccion[1]);
	estructura->contenido = string_new();
	string_append(&estructura->contenido, separada_instruccion[2]);
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->pcbPlanificador->proximaInstruccion++;
	cpu->estructuraSolicitud = estructura;
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger,
			string_from_format("Id del proceso %i \n", estructura->PID));
	log_info(logger,
			string_from_format(
					"instruccion ejecutada: escribir. numero de pagina, %i \n",
					estructura->numeroPagina));
	log_info(logger,
			string_from_format("contenido %s \n", estructura->contenido));
	pthread_mutex_unlock(&mutexCPULogs);
	sleep(configuracion->retardo);
}
//devuelve la estructura de leerMemoria
void ejecuta_LeerMemoria(char** separada_instruccion, t_cpu* cpu) {
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
	cpu->estadoEjecucion = USO;
	estructura->contenido = string_new();
	estructura->numeroPagina = atoi(separada_instruccion[1]);
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->pcbPlanificador->proximaInstruccion++;
	cpu->estructuraSolicitud = estructura;
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger,
			string_from_format("Id del proceso %i \n", estructura->PID));
	log_info(logger,
			string_from_format(
					"instruccion ejecutada: leer . numero de pagina, %i \n",
					estructura->numeroPagina));
	pthread_mutex_unlock(&mutexCPULogs);
	sleep(configuracion->retardo);
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
void ejecuta_FinProcesoMemoria(t_cpu* cpu) {
	t_PID* estructura = malloc(sizeof(t_PID));
	cpu->estadoEjecucion = USO;
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->pcbPlanificador->proximaInstruccion = 0;
	cpu->estructuraSolicitud = estructura;
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger,
			string_from_format("Id del proceso %i \n", estructura->PID));
	log_info(logger, "instruccion ejecutada: fin proceso memoria  \n");
	pthread_mutex_unlock(&mutexCPULogs);
	sleep(configuracion->retardo);
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
void ejecuta_EntradaSalida(char** separada_instruccion, t_cpu* cpu) {

	cpu->estadoEjecucion = USO;
	//+++++++++++++++
	cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
			cpu->mCodCPU->respEjec->resultadosInstrucciones,
			strlen(cpu->mCodCPU->respEjec->resultadosInstrucciones) + 1
					+ strlen("mProc %d %s %d - en entrada-salida de tiempo ;\0")
					+ strlen(separada_instruccion[1]));
	//		++++++++++++++++++++++
	cpu->mCodCPU->respEjec->finalizoOk = true;
	cpu->pcbPlanificador->proximaInstruccion++;
	cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
	cpu->mCodCPU->respEjec->cant_entrada_salida = atoi(separada_instruccion[1]);
	string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
			string_from_format("mProc %d %s %d", cpu->pcbPlanificador->pid,
					"en entrada-salida de tiempo",
					atoi(separada_instruccion[1])));
	//+++++++++++++++++++++++++++++++++++
	cpu->cantInstEjecutadas += 1;
	cpu->estadoEjecucion = NO_USO;
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger,
			string_from_format("Id del proceso %i \n",
					cpu->pcbPlanificador->pid));
	log_info(logger, "instruccion ejecutada: entrada -salida  \n");
	pthread_mutex_unlock(&mutexCPULogs);
	sleep(configuracion->retardo);
}

void resultadoAlPlanificador(t_cpu* cpu) {
	//int socketPlanificador = atoi(
	//		(char*) dictionary_get(conexiones, "Planificador"));
	int socketPlanificador = cpu->socketPlanificador;
	//		++++++++++++++++++++++funcion finalizar
	cpu->mCodCPU->respEjec->finalizoOk = true;
	cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
	enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
			cpu->mCodCPU->respEjec);
///	free(cpu->mCodCPU->respEjec);
}
