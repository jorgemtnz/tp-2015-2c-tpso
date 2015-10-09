#include "CPU.h"

t_iniciar_swap* ejecuta_IniciarProceso(char* separada_instruccion, t_cpu* cpu) {
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;

	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));

	estructura->PID = cpu->pcbPlanificador->pid;
//	estructura->cantidadPaginas = atoi(string_split(separada_instruccion, ";")[0]);
//	if (estructura->cantidadPaginas< 0)
//		return NULL;
  estructura->cantidadPaginas = 10; //hardcodeo
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
t_escribirMem* ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_escribirMem* estructura = malloc(sizeof(t_escribirMem));

	char** buffer;
	char** line;
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
//	line = string_split(separada_instruccion, ";");
//	buffer = string_split(line[0], " ");

//	if (buffer[0] == NULL) {
//		log_error(logger, "[ERROR] al crear buffer de instruccion en ejecuta_EscribirMemoria");
//		exit(-1);
	}
//	estructura->pagina = atoi(buffer[0]);
//	estructura->texto = buffer[1];
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura->pagina =4; //hardcodeo
	estructura->texto = "Hola";
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//devuelve la estructura de leerMemoria
t_leerMem* ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_leerMem* estructura = malloc(sizeof(t_leerMem));
	char** buffer;

	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
//	buffer = string_split(separada_instruccion, " ");
//	estructura->pagina = atoi(buffer[0]);
////	if (estructura->pagina<0)
		return NULL;
        estructura->pagina = 4;//hardcode
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
t_finalizarProc_Mem* ejecuta_FinProcesoMemoria(t_cpu* cpu) {
	t_finalizarProc_Mem* estructura=malloc(sizeof(t_finalizarProc_Mem));
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
	log_info(logger, "se ejecuta fin de proceso desde CPU ");
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
t_entrada_salida* ejecuta_EntradaSalida(char* separada_instruccion, t_cpu* cpu) {
	t_entrada_salida* estructura = malloc(sizeof(t_entrada_salida));
	char* temporal;
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;

    estructura->PID = cpu->pcbPlanificador->pid;
    	temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid, "en entrada-salida de tiempo %d", configuracion->retardo);
    	strcpy(estructura->expresion, temporal);

	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
