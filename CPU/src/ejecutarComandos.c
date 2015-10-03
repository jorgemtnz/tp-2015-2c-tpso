#include "CPU.h"

t_iniciar_swap* ejecuta_IniciarProceso(char* separada_instruccion, t_cpu* cpu) {
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;

	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura = NULL;
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura->cantidadPaginas = atoi(string_split(separada_instruccion, ";")[0]);
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
t_escribirMem* ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_escribirMem* estructura = malloc(sizeof(t_escribirMem));
	estructura = NULL;
	char** buffer;
	char** line;
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
	line = string_split(separada_instruccion, ";");
	buffer = string_split(line[0], " ");

	if (buffer == NULL) {
		log_error(logger, "[ERROR] al crear buffer de instruccion en ejecuta_EscribirMemoria");
		exit(-1);
	}
	estructura->pagina = atoi(buffer[0]);
	estructura->texto = buffer[1];
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//devuelve la estructura de leerMemoria
t_leerMem* ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_leerMem* estructura = malloc(sizeof(t_leerMem));
	char** buffer;
	estructura = NULL;
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
	buffer = string_split(separada_instruccion, " ");
	estructura->pagina = atoi(buffer[0]);
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
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
