#include "CPU.h"

t_iniciar_swap* ejecuta_IniciarProceso(char* separada_instruccion, t_cpu* cpu) {
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += cpu->cantInstEjecutadas + 1;

	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura = NULL;
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura->cantidadPaginas = atoi(string_split(separada_instruccion, ";")[0]);
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
void* ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
	void* retorno = NULL;

	return retorno;
}
//devuelve
void* ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu) {
	void* retorno = NULL;

	return retorno;

}
//mandar el comando de finalizar y el respectivo PID IP del proceso
int ejecuta_FinProcesoMemoria(t_cpu* cpu) {
	log_info(logger, "se ejecuta fin de proceso desde CPU ");
	return FIN_PROCESO_MEM;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
int ejecuta_EntradaSalida(char* separada_instruccion, t_cpu* cpu) {

	return EXIT_SUCCESS;
}
