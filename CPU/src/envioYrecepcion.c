#include "CPU.h"
//agregar comportamiento en cada break
//ejecutar todo tipo de comandos del mCod
void* ejecutar(int token, char* separada_instruccion, t_cpu* cpu) {
	void* estructura;

	log_info(logger, "se va a ejecutar la funcion ejecutar");
	switch (token) {
	case (INICIAR_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar iniciar proceso memoria ");
		t_iniciar_swap* estructura;
		estructura = ejecuta_IniciarProceso(separada_instruccion, cpu);
		int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		enviarStruct(socketMemoria, INICIAR_PROCESO_MEM, estructura);
		return estructura;
		break;
	}
	case (ESCRIBIR_MEM): {
		log_info(logger, "se va a ejecutar escribir memoria");
		t_contenido_pagina* estructura;
		estructura = ejecuta_EscribirMemoria(separada_instruccion, cpu);
		int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		enviarStruct(socketMemoria, ESCRIBIR_MEM, estructura);
		return estructura;
		break;
	}
	case (LEER_MEM): {
		log_info(logger, "se va a ejecutar leer memoria ");
		t_contenido_pagina* estructura;

		estructura = ejecuta_LeerMemoria(separada_instruccion, cpu);
		if (estructura == NULL) {
			log_error(logger,
					"[ERROR] no se genero la estructura para enviar leerMemoria");

		}
		int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		enviarStruct(socketMemoria, LEER_MEM, estructura);
		return estructura;
		break;
	}
	case (FIN_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar fin proceso memoria ");
		t_PID* estructura = malloc(sizeof(t_PID));
		int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		estructura = ejecuta_FinProcesoMemoria(cpu);
		enviarStruct(socketMemoria, FIN_PROCESO_MEM, estructura);
		return estructura;
		break;
	}
	case (ENTRADA_SALIDA): {
		log_info(logger, "se va a ejecutar entrada salida ");
		t_entrada_salida* estructura;
		int socketPlanificador = atoi(
				(char*) dictionary_get(conexiones, "Planificador"));
		estructura = ejecuta_EntradaSalida(separada_instruccion, cpu);
		enviarStruct(socketPlanificador, ENTRADA_SALIDA, estructura);
		return estructura;
		break;
	}
	}
	return estructura;
}
//recibe las respuestas
void* recibirMensajeVarios(t_header* header, char* buffer, void* extra,
		t_cpu* cpu) {
	void* estructura;
	log_info(logger, "se va a ejecutar recibirMensajeVarios ");

	int token;
	token = header->tipoMensaje;
	switch (token) {
	case (RESUL_ESCRIBIR): {
		log_info(logger, "se va a ejecutar result escribir ");
		t_contenido_pagina* datosdesdeMEmoria = (t_contenido_pagina*) buffer;

		break;
	}
	case (RESUL_FIN): {
		log_info(logger, "se va a ejecutar result fin de proceso ");
		t_PID* datosDesdeMem = (t_PID*) buffer;

		int socketPlanificador = atoi(
				(char*) dictionary_get(conexiones, "Planificador"));
		cpu = ejecutarResul_Fin(cpu);
		enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
				cpu->mCodCPU->respEjec);
		break;
	}
	case (RESUL_INICIAR_PROC_OK_CPU): {
		//recibe desde memoria y debe continuar con la ejecucion
		log_info(logger, "se va a ejecutar result iniciar proceso ok");
		t_PID* datosDesdeMem = (t_PID*) buffer;
		//	t_resultado_instruccion* resultado = creaResultadoInstruccion();
		t_resultado_instruccion* resultado = malloc(
				sizeof(t_resultado_instruccion));
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

		break;
	}
	case (RESUL_INICIAR_PROC_NO_OK_CPU): {
		//al dar error se debe devolver el proceso
		log_info(logger, "se va a ejecutar result iniciar proceso no ok");
		t_PID* datosDesdeMem = (t_PID*) buffer;

		t_resultado_instruccion* resultado = creaResultadoInstruccion();
		char* temporal;

		resultado->comandoInstruccion =  "inicializar";

		resultado->tipoMensaje = RESUL_INICIAR_PROC_NO_OK_CPU;
		char* fallo = "fallo";

		temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid,
				fallo);

		resultado->expresion =  temporal;
		list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);

		break;
	}
	case (RESUL_LEER_OK_CPU): {
		log_info(logger, "se va a ejecutar resultLeer ");
		t_contenido_pagina* datosDesdeMem = (t_contenido_pagina*) buffer;
		char* contenido = datosDesdeMem->contenido; // este es el contenido de la pagina

		t_resultado_instruccion* resultado = creaResultadoInstruccion();
		char* temporal;
		resultado->comandoInstruccion = "leer";
		resultado->tipoMensaje = RESUL_LEER_OK_CPU;
		temporal = string_from_format("mProc %d Pagina leida",
				cpu->pcbPlanificador->pid); //, "Pagina", "leida", contenido);
//		string_append(resultado->expresion,contenido);
		resultado->expresion = temporal;
		list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
		break;
	}
	case (CONTEXTO_MPROC): {
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
		//inicia toda la cadena de instruccion desde la CPU hacia memoria
		t_pcb* pcbPlanificador = (t_pcb*) buffer;
		printf("Ruta recibida del planificador: %s\n",
				pcbPlanificador->rutaArchivoMcod);
		preparaCPU(pcbPlanificador);
		//llama a procesa codigo

		estructura = procesaCodigo(cpu);
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
		log_info(logger,
				"llega mensaje del planificador pidiendo el porcentaje del tiempo al CPU ");
	}

	}
	return estructura;
}

