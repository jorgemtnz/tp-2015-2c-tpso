#include "CPU.h"
// todas las funciones de resultados de ejecucion deben devolver un string segun su tipo al planificador
int ejecutaResultError(t_cpu* cpu) {

	return EXIT_SUCCESS;
}
int ejecutaResultEscribir(t_cpu* cpu) {

	return EXIT_SUCCESS;
}

int ejecutaResulFin(t_cpu* cpu) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
	char* temporal;
	strcpy(resultado->comandoInstruccion, "finalizar");
	resultado->tipoMensaje = RESUL_FIN_OK;
	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, "finalizado");
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
	cpu->mCodCPU->respEjec->finalizoOk = true;
	cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
	enviarStruct(socketPlanificador, RESUL_FIN, cpu->mCodCPU->respEjec);

	return EXIT_SUCCESS;

}

int ejecutaResulIniciarProcOK(t_cpu* cpu) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	char* temporal;
	strcpy(resultado->comandoInstruccion, "inicializar");
	resultado->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU;
	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, "Iniciado");
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);

	return EXIT_SUCCESS;
}

int ejecutaResulIniciarProc_NO_OK(t_cpu* cpu) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	char* temporal;
	strcpy(resultado->comandoInstruccion, "inicializar");
	resultado->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU;
	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, "fallo");
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);

	return EXIT_SUCCESS;
}

int ejecutaResulInstrEjec(t_cpu* cpu) {

	return EXIT_SUCCESS;
}
int ejecutaResultLeerOK(t_cpu* cpu,char* contenido) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	char* temporal;
	strcpy(resultado->comandoInstruccion, "leer");
	resultado->tipoMensaje = RESUL_LEER_OK;
	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, "Pagina", "leida", contenido);
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
	return EXIT_SUCCESS;
}

int ejecutaResulLeerError(t_cpu* cpu) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
		char* temporal;
		strcpy(resultado->comandoInstruccion, "leer");
		resultado->tipoMensaje = RESUL_LEER_ERROR;
		temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, "Pagina", "no leida");
		strcpy(resultado->expresion, temporal);
		list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);

	return EXIT_SUCCESS;
}

