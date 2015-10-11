#include "CPU.h"
// todas las funciones de resultados de ejecucion deben devolver un string segun su tipo al planificador

int ejecutaResulFin(t_cpu* cpu) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
	char* temporal;
	strcpy(resultado->comandoInstruccion, "finalizar");
	resultado->tipoMensaje = RESUL_FIN_OK;
	char* finalizado = "finalizado";
	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, finalizado);
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
	cpu->mCodCPU->respEjec->finalizoOk = true;
	cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
	enviarStruct(socketPlanificador, RESUL_EJECUCION_OK, cpu->mCodCPU->respEjec);

	return EXIT_SUCCESS;
}
int ejecutaResultEscribir(t_cpu* cpu) {

	return EXIT_SUCCESS;
}

int ejecutaResulIniciarProcOK(t_cpu* cpu) {
//	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	t_resultado_instruccion* resultado= malloc(sizeof(t_resultado_instruccion));
	char* temporal;
//	strcpy(resultado->comandoInstruccion, "inicializar");
	resultado->comandoInstruccion = "inicializar";
	resultado->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU;
//	char* iniciado = "Iniciado";
	//temporal = string_from_format("mProc %d %s", cpu->pcbPlanificador->pid, iniciado);
	temporal = "mProc %d %s";
	//strcpy(resultado->expresion, temporal);
	resultado->expresion = temporal;
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);

	return EXIT_SUCCESS;
}


int ejecutaResulIniciarProc_NO_OK(t_cpu* cpu) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	char* temporal;
	strcpy(resultado->comandoInstruccion, "inicializar");
	resultado->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU;
	char* fallo = "fallo";
	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, fallo);
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);

	return EXIT_SUCCESS;
}

int ejecutaResultLeerOK(t_cpu* cpu,char* contenido) {
	t_resultado_instruccion* resultado = creaResultadoInstruccion();
	char* temporal;
	strcpy(resultado->comandoInstruccion, "leer");
	resultado->tipoMensaje = RESUL_LEER_OK;
	temporal = string_from_format("mProc %d Pagina leida", cpu->pcbPlanificador->pid);//, "Pagina", "leida", contenido);
	strcpy(resultado->expresion, temporal);
	list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
	return EXIT_SUCCESS;
}



