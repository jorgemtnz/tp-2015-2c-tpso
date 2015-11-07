#include "CPU.h"

void ejecuta_IniciarProceso(char** separada_instruccion, t_cpu* cpu) {
	cpu->estadoEjecucion = USO;
	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura->cantidadPaginas = atoi(separada_instruccion[1]);
	cpu->estructuraSolicitud = (t_iniciar_swap*) estructura;
	cpu->pcbPlanificador->proximaInstruccion++;
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
	sleep(configuracion->retardo);
	cpu->estructuraSolicitud = estructura;
}
//devuelve la estructura de leerMemoria
void ejecuta_LeerMemoria(char** separada_instruccion, t_cpu* cpu) {
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
	cpu->estadoEjecucion = USO;
	estructura->contenido = string_new();
	estructura->numeroPagina = atoi(separada_instruccion[1]);
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->pcbPlanificador->proximaInstruccion++;
	sleep(configuracion->retardo);
	cpu->estructuraSolicitud = estructura;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
void ejecuta_FinProcesoMemoria(t_cpu* cpu) {
	t_PID* estructura = malloc(sizeof(t_PID));
	cpu->estadoEjecucion = USO;
	log_info(logger, "se ejecuta fin de proceso desde CPU ");
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->pcbPlanificador->proximaInstruccion = 0;
	sleep(configuracion->retardo);
	cpu->estructuraSolicitud = estructura;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
t_entrada_salida* ejecuta_EntradaSalida(char** separada_instruccion, t_cpu* cpu) {
	t_entrada_salida* estructura = malloc(sizeof(t_entrada_salida));
	char* temporal;
	cpu->estadoEjecucion = USO;
	estructura->PID = cpu->pcbPlanificador->pid;
	temporal = string_from_format("mProc %d %s %d", cpu->pcbPlanificador->pid,
			"en entrada-salida de tiempo", configuracion->retardo);
	estructura->expresion = temporal;
	cpu->pcbPlanificador->proximaInstruccion++;
	cpu->cantInstEjecutadas += 1;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}

void resultadoAlPlanificador(t_cpu* cpu) {
	int socketPlanificador = atoi(
			(char*) dictionary_get(conexiones, "Planificador"));
	//		++++++++++++++++++++++funcion finalizar
	cpu->mCodCPU->respEjec->finalizoOk = true;
	cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
	enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
			cpu->mCodCPU->respEjec);
///	free(cpu->mCodCPU->respEjec);
}
