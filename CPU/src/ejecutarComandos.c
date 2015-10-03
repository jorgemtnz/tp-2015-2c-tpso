
#include "CPU.h"

int ejecuta_IniciarProceso(char* separada_instruccion, t_cpu* cpu, int socket) {

	return EXIT_SUCCESS;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
int ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu, int socket) {

	return EXIT_SUCCESS;
}
//mandar comando a memoria y  el numero de pagina que se debe leer
int ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu, int socket) {

	return EXIT_SUCCESS;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
int ejecuta_FinProcesoMemoria(char* separada_instruccion, t_cpu* cpu, int socket) {

	return EXIT_SUCCESS;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
int ejecuta_EntradaSalida(char* separada_instruccion, t_cpu* cpu, int socket) {

	return EXIT_SUCCESS;
}
