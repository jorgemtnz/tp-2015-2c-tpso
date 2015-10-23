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
t_contenido_pagina* ejecuta_EscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));

//	char** buffer;
//	char** line;
	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
//	line = string_split(separada_instruccion, ";");
//	buffer = string_split(line[0], " ");

//	if (buffer[0] == NULL) {
//		log_error(logger, "[ERROR] al crear buffer de instruccion en ejecuta_EscribirMemoria");
//		exit(-1);
//	}
//	estructura->pagina = atoi(buffer[0]);
//	estructura->texto = buffer[1];
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura->numeroPagina =4; //hardcodeo
	estructura->contenido = "Hola";
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//devuelve la estructura de leerMemoria
t_contenido_pagina* ejecuta_LeerMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
//	char** buffer;

	cpu->estadoEjecucion = USO;
	cpu->cantInstEjecutadas += 1;
//	buffer = string_split(separada_instruccion, " ");
//	estructura->pagina = atoi(buffer[0]);
//	if (estructura->pagina<0)
//		return NULL;
	estructura->contenido = NULL;
        estructura->numeroPagina = 4;//hardcodeo
	estructura->PID = cpu->pcbPlanificador->pid;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
t_PID* ejecuta_FinProcesoMemoria(t_cpu* cpu) {
	t_PID* estructura=malloc(sizeof(t_PID));
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
    	temporal = string_from_format("mProc %d %s %d", cpu->pcbPlanificador->pid, "en entrada-salida de tiempo", configuracion->retardo);

    estructura->expresion = temporal;
	cpu->estadoEjecucion = NO_USO;
	return estructura;
}
