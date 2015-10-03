#include "CPU.h"
//agregar comportamiento en cada break
//ejecutar todo tipo de comandos del mCod
int ejecutar(int token, char* separada_instruccion, t_cpu* cpu) {
	log_info(logger, "se va a ejecutar la funcion ejecutar");
	switch (token) {
	case (INICIAR_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar iniciar proceso memoria ");
		ejecutaIniciarProceso(separada_instruccion, cpu);
		break;
	}
	case (ESCRIBIR_MEM): {
		log_info(logger, "se va a ejecutar escribir memoria");
		ejecutaEscribirMemoria(separada_instruccion, cpu);
		break;
	}
	case (LEER_MEM): {
		log_info(logger, "se va a ejecutar leer memoria ");
		ejecutaLeerMemoria(separada_instruccion, cpu);
		break;
	}
	case (FIN_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar fin proceso memoria ");
		ejecutaFinProcesoMemoria(cpu);
		break;
	}
	case (ENTRADA_SALIDA): {
		log_info(logger, "se va a ejecutar entrada salida ");
		ejecutaEntradaSalida(separada_instruccion, cpu);
		break;
	}
	}

	return EXIT_SUCCESS;
}
//recibe las respuestas
int recibirMensajeVarios(int token, char* buffer, void* extra) {
	log_info(logger, "se va a ejecutar recibirMensajeVarios ");
	switch (token) {
	case (RESUL_ERROR): {
		log_info(logger, "se va a ejecutar result error");
		ejecutaResultError(socket);

		break;
	}
	case (RESUL_ESCRIBIR): {
		log_info(logger, "se va a ejecutar result escribir ");
		ejecutaResultEscribir();
		break;
	}
	case (RESUL_FIN): {
		log_info(logger, "se va a ejecutar result fin de proceso ");
		ejecutaResulFin();
		break;
	}
	case (RESUL_INICIAR_PROC): {
		log_info(logger, "se va a ejecutar result iniciar proceso ");
		ejecutaResulIniciarProc();
		break;
	}
	case (RESUL_INSTR_EJEC): {
		log_info(logger, "se va a ejecutar result instruccion ejecutar ");
		ejecutaResulInstrEjec();
		break;
	}
	case (RESUL_LEER): {
		log_info(logger, "se va a ejecutar resultLeer ");
		ejecutaResultLeer();
		break;
	}
	case (RESUL_OK): {
		log_info(logger, "se va a ejecutar resultOK ");
		ejecutaResulOk();

		break;
	}
	case (CONTEXTO_MPROC): {
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
		//me marca error
//		t_pcb pcbProc = malloc(sizeof(t_pcb));
//		pcbProc = (t_pcb) buffer;
//		t_pcb*	pcbPlanificador = &pcbProc;
		t_pcb* pcbPlanificador = (t_pcb*) buffer;
		printf("Ruta recibida del planificador: %s\n", pcbPlanificador->rutaArchivoMcod);
		preparaCPU(pcbPlanificador);
		break;
	}

	case (STRING): {
		log_info(logger, "se va a recibir un string ");
//			char* mensaje = malloc(header->tamanioMensaje);
//			recibirPorSocket(socket, mensaje, header->tamanioMensaje);
//			printf("Recibi el mensaje: %s\n", mensaje);
//		}
		/*		if(string_starts_with(buffer, "correr programa")) {
		 char* socketCPU = (char*)dictionary_get(conexiones, "Memoria");
		 puts("Enviando \"correr programa\" a la Memoria");
		 enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
		 // de aca para abajo serian las conexiones
		 }*/
		break;
	}

	case (TIEMPO_CPU): {
		log_info(logger, "llega mensaje del planificador pidiendo el porcentaje del tiempo al CPU ");
	}

	}
	return EXIT_SUCCESS;
}
// todas las funciones de resultados de ejecucion deben devolver un string segun su tipo al planificador
int ejecutaResultError() {
	ejecutaResult_Error();
	return EXIT_SUCCESS;
}
int ejecutaResultEscribir() {
	ejecutaResult_Escribir();

	return EXIT_SUCCESS;
}

int ejecutaResulFin() {
	ejecutaResul_Fin();
	return EXIT_SUCCESS;
}

int ejecutaResulIniciarProc() {
	ejecutaResul_IniciarProc();
	return EXIT_SUCCESS;
}

int ejecutaResulInstrEjec() {
	ejecutaResul_InstrEjec();
	return EXIT_SUCCESS;
}
int ejecutaResultLeer() {
	ejecutaResult_Leer();
	return EXIT_SUCCESS;
}

int ejecutaResulOk() {

	ejecutaResul_Ok();
	return EXIT_SUCCESS;
}

// en todas estas funciones de ejecutar se debe mandar al palnificador su estructura del PCB del proceso en cuestion
//debe mandar el inicio a memoria con serializacion correspondiente
int ejecutaIniciarProceso(char* separada_instruccion, t_cpu* cpu) {
	//TODO HACK usar un tipo especifico
	t_iniciar_swap* estructura;

	estructura = ejecuta_IniciarProceso(separada_instruccion, cpu);
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, INICIAR_PROC_SWAP, estructura);

	return EXIT_SUCCESS;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
int ejecutaEscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_escribirMem* estructura = malloc(sizeof(t_escribirMem));
	estructura->PID = cpu->pcbPlanificador->pid;
	estructura = (t_escribirMem*) ejecuta_EscribirMemoria(separada_instruccion, cpu);

	if (estructura == NULL) {
		log_error(logger, "[ERROR] no se genero la estructura para enviar EscribirMemoria");
		return EXIT_FAILURE;
	}
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, ESCRIBIR_MEM, estructura);

	return EXIT_SUCCESS;
}
//mandar comando a memoria y  el numero de pagina que se debe leer
int ejecutaLeerMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_leerMem* estructura = malloc(sizeof(t_leerMem));
	estructura->PID = cpu->pcbPlanificador->pid;

	estructura = (t_leerMem*) ejecuta_LeerMemoria(separada_instruccion, cpu);
	if (estructura == NULL) {
		log_error(logger, "[ERROR] no se genero la estructura para enviar leerMemoria");
		return EXIT_FAILURE;
	}
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, LEER_MEM, estructura);

	return EXIT_SUCCESS;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
int ejecutaFinProcesoMemoria(t_cpu* cpu) {
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	int tipoMensaje;
	tipoMensaje = ejecuta_FinProcesoMemoria(cpu);
	return EXIT_SUCCESS;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
int ejecutaEntradaSalida(char* separada_instruccion, t_cpu* cpu) {
	ejecuta_EntradaSalida(separada_instruccion, cpu);
	return EXIT_SUCCESS;
}
