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
int recibirMensajeVarios(  t_header* header,   char*   buffer, void* extra, t_cpu* cpu) {
	log_info(logger, "se va a ejecutar recibirMensajeVarios ");

	int token;
	token = header->tipoMensaje;
	switch (token) {
	case (RESUL_ERROR): {
		log_info(logger, "se va a ejecutar result error");
		ejecutaResultError(cpu);
		break;
	}
	case (RESUL_ESCRIBIR): {
		log_info(logger, "se va a ejecutar result escribir ");
		ejecutaResultEscribir(cpu);
		break;
	}
	case (RESUL_FIN): {
		log_info(logger, "se va a ejecutar result fin de proceso ");
		t_PID* datosDesdeMem = (t_PID*) buffer;
		ejecutaResulFin(cpu);
		break;
	}
	case (RESUL_INICIAR_PROC_OK_CPU): {
		log_info(logger, "se va a ejecutar result iniciar proceso ok");
		ejecutaResulIniciarProcOK(cpu);
		break;
	}
	case (RESUL_INICIAR_PROC_NO_OK_CPU): {
			log_info(logger, "se va a ejecutar result iniciar proceso no ok");
			ejecutaResulIniciarProc_NO_OK(cpu);
			break;
		}
	case (RESUL_INSTR_EJEC): {
		log_info(logger, "se va a ejecutar result instruccion ejecutar ");
		ejecutaResulInstrEjec(cpu);
		break;
	}
	case (RESUL_LEER_OK_CPU): {
		log_info(logger, "se va a ejecutar resultLeer ");
		t_respuesta_leer_CPU* datosDesdeMem = (t_respuesta_leer_CPU*) buffer;
		char* contenido= datosDesdeMem->contenido; // este es el contenido de la pagina
		ejecutaResultLeerOK(cpu, contenido);
		break;
	}
	case (RESUL_LEER_ERROR): {
		log_info(logger, "se va a ejecutar resultOK ");
		ejecutaResulLeerError(cpu);

		break;
	}
	case (CONTEXTO_MPROC): {
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
        //inicia toda la cadena de instruccion desde la CPU hacia memoria
		t_pcb* pcbPlanificador = (t_pcb*) buffer;
		printf("Ruta recibida del planificador: %s\n", pcbPlanificador->rutaArchivoMcod);
		preparaCPU(pcbPlanificador);
		//llama a procesa codigo
		procesaCodigo(cpu);
		break;
	}

	case (STRING): {
		log_info(logger, "se va a recibir un string ");
//esto esta extraido desde procesarMensaje() por lo que si se descomenta, daria error
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

// en todas estas funciones de ejecutar se debe mandar al palnificador su estructura del PCB del proceso en cuestion
//debe mandar el inicio a memoria con serializacion correspondiente
int ejecutaIniciarProceso(char* separada_instruccion, t_cpu* cpu) {
	//TODO HACK usar un tipo especifico
	t_iniciar_swap* estructura;

	estructura = ejecuta_IniciarProceso(separada_instruccion, cpu);
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, INICIAR_PROCESO_MEM, estructura);
	free(estructura);
	return EXIT_SUCCESS;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
int ejecutaEscribirMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_escribirMem* estructura;

	estructura = ejecuta_EscribirMemoria(separada_instruccion, cpu);

	if (estructura == NULL) {
		log_error(logger, "[ERROR] no se genero la estructura para enviar EscribirMemoria");
		return EXIT_FAILURE;
	}
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, ESCRIBIR_MEM, estructura);
	free(estructura);
	return EXIT_SUCCESS;
}
//mandar comando a memoria y  el numero de pagina que se debe leer
int ejecutaLeerMemoria(char* separada_instruccion, t_cpu* cpu) {
	t_leerMem* estructura ;

	estructura = ejecuta_LeerMemoria(separada_instruccion, cpu);
	if (estructura == NULL) {
		log_error(logger, "[ERROR] no se genero la estructura para enviar leerMemoria");
		return EXIT_FAILURE;
	}
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, LEER_MEM, estructura);
	free(estructura);
	return EXIT_SUCCESS;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
int ejecutaFinProcesoMemoria(t_cpu* cpu) {
	t_finalizarProc_Mem* estructura;
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));

	estructura = ejecuta_FinProcesoMemoria(cpu);
	enviarStruct(socketMemoria, FIN_PROCESO_MEM, estructura);
  free(estructura);
  destHiloCPU(cpu);
	return EXIT_SUCCESS;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
int ejecutaEntradaSalida(char* separada_instruccion, t_cpu* cpu) {
	t_entrada_salida* estructura ;
	int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
	estructura =ejecuta_EntradaSalida(separada_instruccion, cpu);
	enviarStruct(socketPlanificador, ENTRADA_SALIDA, estructura);
	free(estructura);
	return EXIT_SUCCESS;
}
