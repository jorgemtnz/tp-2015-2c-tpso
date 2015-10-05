
#include "CPU.h"
// todas las funciones de resultados de ejecucion deben devolver un string segun su tipo al planificador
int ejecutaResultError(t_cpu* cpu) {

	return EXIT_SUCCESS;
}
int ejecutaResultEscribir(t_cpu* cpu) {


	return EXIT_SUCCESS;
}

int ejecutaResulFin(t_cpu* cpu) {

	return EXIT_SUCCESS;
}

int ejecutaResulIniciarProcOK(t_cpu* cpu) {
t_resultado_instruccion* resultado = creaResultadoInstruccion();
char* temporal;
strcpy(resultado->comandoInstruccion, "inicializar") ;
resultado->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU ;
temporal = string_from_format("mProc %d",cpu->pcbPlanificador->pid, "Iniciado" );
strcpy(resultado->expresion,temporal);
list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones  , resultado);

	return EXIT_SUCCESS;
}

int ejecutaResulIniciarProc_NO_OK(t_cpu* cpu) {
t_resultado_instruccion* resultado = creaResultadoInstruccion();
char* temporal;
strcpy(resultado->comandoInstruccion, "inicializar") ;
resultado->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU ;
temporal = string_from_format("mProc %d",cpu->pcbPlanificador->pid, "fallo" );
strcpy(resultado->expresion,temporal);
list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones  , resultado);

	return EXIT_SUCCESS;
}

int ejecutaResulInstrEjec(t_cpu* cpu) {

	return EXIT_SUCCESS;
}
int ejecutaResultLeer(t_cpu* cpu) {

	return EXIT_SUCCESS;
}

int ejecutaResulOk(t_cpu* cpu) {

	return EXIT_SUCCESS;
}


