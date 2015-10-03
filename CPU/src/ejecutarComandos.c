
#include "CPU.h"

int ejecuta_IniciarProceso(char* separada_instruccion, t_cpu* cpu) {

	return EXIT_SUCCESS;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
void* ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
void* retorno = NULL;

	return retorno;
}
//devuelve
int ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu) {

	return EXIT_SUCCESS;
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
