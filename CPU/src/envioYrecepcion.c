#include "CPU.h"
//agregar comportamiento en cada break
//ejecutar todo tipo de comandos del mCod
void ejecutar(int token, char** separada_instruccion, t_cpu* cpu) {

	log_info(logger, "se va a ejecutar la funcion ejecutar");
	switch (token) {
	case (INICIAR_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar iniciar proceso memoria ");
		ejecuta_IniciarProceso(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, INICIAR_PROCESO_MEM,
				cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (ESCRIBIR_MEM): {
		log_info(logger, "se va a ejecutar escribir memoria");
		ejecuta_EscribirMemoria(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, ESCRIBIR_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (LEER_MEM): {
		log_info(logger, "se va a ejecutar leer memoria ");
		ejecuta_LeerMemoria(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, LEER_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (FIN_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar fin proceso memoria ");
		//	t_PID* estructura = malloc(sizeof(t_PID));
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		ejecuta_FinProcesoMemoria(cpu);
		enviarStruct(socketMemoria, FIN_PROCESO_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (ENTRADA_SALIDA): { //falta modificar
		log_info(logger, "se va a ejecutar entrada salida ");

		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		ejecuta_EntradaSalida(separada_instruccion, cpu);

		enviarStruct(socketPlanificador, ENTRADA_SALIDA,
				cpu->mCodCPU->respEjec);
		free(cpu->mCodCPU->respEjec);
		cpu->estado = DISPONIBLE;
		break;
	}
	}
}

//recibe las respuestas
void recibirMensajeVarios(t_header* header, char* buffer, void* extra,
		t_cpu* cpu) {

	log_info(logger, "se va a ejecutar recibirMensajeVarios ");
	int token;
	token = header->tipoMensaje;
	switch (token) {
	case (CONTEXTO_MPROC): {
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
		//inicia toda la cadena de instruccion desde la CPU hacia memoria
		t_pcb* pcbPlanificador = (t_pcb*) buffer;
		printf("Ruta recibida del planificador: %s\n",
				pcbPlanificador->rutaArchivoMcod);
		cpu->estado = NO_DISPONIBLE;
		cpu->pcbPlanificador = pcbPlanificador;
		procesaCodigo(cpu);

		break;
	}

	case (RESUL_INICIAR_PROC_OK_CPU): {
		//se cuenta aca como terminado de ejecutar la instruccion iniciar
		cpu->cantInstEjecutadas += 1;
		cpu->estadoEjecucion = NO_USO;
		//recibe desde memoria y debe continuar con la ejecucion
		log_info(logger, "se va a ejecutar result iniciar proceso ok");
		t_PID* datosDesdeMem = (t_PID*) buffer;
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_PID)
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc %i - Iniciado ;\0"));
		string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
				string_from_format("mProc %i - Iniciado ;\0",
						datosDesdeMem->PID));

		//se ejecuta la siguiente instruccion si corresponde
		if (cpu->pcbPlanificador->tieneDesalojo == true) {
			if (cpu->pcbPlanificador->tamanioRafaga
					>= cpu->cantInstEjecutadas) {
				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resultadoAlPlanificador(cpu);
			}
		} else { // es planificacion FIFO
			ejecuta_Instruccion(
					cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
					cpu);
		}
		break;
	}
	case (RESUL_INICIAR_PROC_NO_OK_CPU): {
		//se cuenta aca como terminado de ejecutar la instruccion iniciar
		cpu->cantInstEjecutadas += 1;
		cpu->estadoEjecucion = NO_USO;
		//al dar error se debe devolver el proceso
		log_info(logger, "se va a ejecutar result iniciar proceso no ok");
		t_PID* datosDesdeMem = (t_PID*) buffer;
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_PID)
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc  - Fallo ;\0"));
		string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
				string_from_format("mProc %i - Fallo ;\0", datosDesdeMem->PID));

		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		cpu->mCodCPU->respEjec->finalizoOk = false;
		cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
		//ESTO HAY QUE CAMBIARLO EN EL PLANIFICADOR PARA QUE ANDE (OJO)
		enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
				cpu->mCodCPU->respEjec);
		free(cpu->mCodCPU->respEjec);
	 	cpu->mCodCPU->respEjec=creaRespuestaEjecucion();
		break;
	}

	case (RESUL_LEER_OK_CPU): {
		//fin de la instruccion leer
		cpu->cantInstEjecutadas += 1;
		cpu->estadoEjecucion = NO_USO;
		//se cuenta aca para tener en cuenta el retraso de pedirle a memoria
		log_info(logger, "se va a ejecutar resultLeer ");
		t_contenido_pagina* datosDesdeMem = (t_contenido_pagina*) buffer;
		//cambio sizeof(t_contenido_pagina) * 10
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_contenido_pagina) + 1
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc %i; - Pagina %i; leida: %s ;\0"));
		string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
				string_from_format("mProc %i; - Pagina %i; leida: %s ;\0",
						datosDesdeMem->PID, datosDesdeMem->numeroPagina,
						datosDesdeMem->contenido));

		//se ejecuta la siguiente instruccion si corresponde
		if (cpu->pcbPlanificador->tieneDesalojo == true) {
			if (cpu->pcbPlanificador->tamanioRafaga
					>= cpu->cantInstEjecutadas) {

				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
				printf("dsp de ejecuta inst\n");
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resultadoAlPlanificador(cpu);
			}
		} else { // es planificacion FIFO

			ejecuta_Instruccion(
					cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
					cpu);
		}
		break;
	}

	case (RESUL_ESCRIBIR): {
		//es el fin de la ejecucion de escribir
		cpu->cantInstEjecutadas += 1;
		cpu->estadoEjecucion = NO_USO;
		//++++++++++++++cpu libre
		log_info(logger, "se va a ejecutar result escribir ");
		t_contenido_pagina* datosdesdeMEmoria = (t_contenido_pagina*) buffer;
		// se asigna espacio contiguo para los datos del resultado
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_contenido_pagina) + 1
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc; - Pagina; escrita:;\0"));
		string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
				string_from_format("mProc %i; - Pagina %i; escrita: %s ;\0",
						datosdesdeMEmoria->PID, datosdesdeMEmoria->numeroPagina,
						datosdesdeMEmoria->contenido));
		//se ejecuta la siguiente instruccion si corresponde
		if (cpu->pcbPlanificador->tieneDesalojo == true) {
			if (cpu->pcbPlanificador->tamanioRafaga
					>= cpu->cantInstEjecutadas) {
				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resultadoAlPlanificador(cpu);
			}
		} else { // es planificacion FIFO
			ejecuta_Instruccion(
					cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
					cpu);
		}
		break;
	}

	case (RESUL_FIN): {
		//se cuenta el fin de la instruccion finalizar
		cpu->cantInstEjecutadas += 1;
		cpu->estadoEjecucion = NO_USO;

		//++++++++++++++++++++++++++
		log_info(logger, "se va a ejecutar result fin de proceso ");
		t_PID* datosDesdeMem = (t_PID*) buffer;
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_PID)
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc %i - finalizado ;\0"));
		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		//		++++++++++++++++++++++funcion finalizar
		cpu->mCodCPU->respEjec->finalizoOk = true;
		cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
		cpu->mCodCPU->respEjec->cant_entrada_salida = 0;
		string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
				string_from_format("mProc %i - finalizado ;\0",
						datosDesdeMem->PID));
printf("%s\n",cpu->mCodCPU->respEjec->resultadosInstrucciones);
		enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
				cpu->mCodCPU->respEjec);
		cpu->estado = DISPONIBLE;
		free(cpu->mCodCPU->respEjec);
	 	cpu->mCodCPU->respEjec=creaRespuestaEjecucion();
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
		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;

		t_porcentajeCPUs* porcentajeCPUs = malloc(sizeof(t_porcentajeCPUs));

		porcentajeCPUs->respuestasPorcentaje = list_create();

		void tomaPorcentaje(t_cpu* cpu){
			t_respuesta_porcentaje* respuestaPorcentaje = malloc(sizeof(t_respuesta_porcentaje));
			respuestaPorcentaje->res_porcentaje=  cpu->porcentajeUso;
			respuestaPorcentaje->idCpu  = cpu->idCPU;
			list_add(porcentajeCPUs->respuestasPorcentaje, respuestaPorcentaje);
		}

			list_iterate(procCPU->listaCPU,(void*) tomaPorcentaje)	;

		porcentajeCPUs->cantidadDeElementos = list_size(porcentajeCPUs->respuestasPorcentaje);

		//PRUEBA PARA VER QUE SE ESTA ENVIANDO
		t_respuesta_porcentaje *contenido = malloc(sizeof(t_respuesta_porcentaje));
			int a;
			for(a=0;a<porcentajeCPUs->cantidadDeElementos;a++){
				contenido=list_get(porcentajeCPUs->respuestasPorcentaje,a);

			printf("ESTOY ENVIANDO A PLANIF CPU:  PORCENTAJE: %i\n",contenido->res_porcentaje);
			}
		enviarStruct(socketPlanificador, TIEMPO_CPU_RESUL, porcentajeCPUs);
		break;
	}
	}

}

