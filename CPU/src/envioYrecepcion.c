#include "CPU.h"
//agregar comportamiento en cada break
//ejecutar todo tipo de comandos del mCod
void ejecutar(int token, char** separada_instruccion, t_cpu* cpu) {
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar la funcion ejecutar");
	pthread_mutex_unlock(&mutexCPULogs);

	switch (token) {
	case (INICIAR_PROCESO_MEM): {
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
		ejecuta_EscribirMemoria(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, ESCRIBIR_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (LEER_MEM): {
		ejecuta_LeerMemoria(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, LEER_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (FIN_PROCESO_MEM): {
		//	t_PID* estructura = malloc(sizeof(t_PID));
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		ejecuta_FinProcesoMemoria(cpu);
		enviarStruct(socketMemoria, FIN_PROCESO_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		break;
	}
	case (ENTRADA_SALIDA): {
		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		ejecuta_EntradaSalida(separada_instruccion, cpu);

		enviarStruct(socketPlanificador, ENTRADA_SALIDA,
				cpu->mCodCPU->respEjec);
		free(cpu->mCodCPU->respEjec);
		cpu->estado = DISPONIBLE;
		cpu->cantInstEjecutadas = 0;
		break;
	}
	}
}

//recibe las respuestas
void recibirMensajeVarios(t_header* header, char* buffer, void* extra,
		t_cpu* cpu) {
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar recibirMensajeVarios ");
	pthread_mutex_unlock(&mutexCPULogs);
	int token;
	token = header->tipoMensaje;
	switch (token) {
	case (CONTEXTO_MPROC): {
		t_pcb* pcbPlanificador = (t_pcb*) buffer;
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
		log_info(logger,
				string_from_format("ruta archivo %s",
						pcbPlanificador->rutaArchivoMcod));
		log_info(logger,
				string_from_format("tamanio rafaga %i",
						pcbPlanificador->tamanioRafaga));
		log_info(logger,
				string_from_format("proxima instruccion %i",
						pcbPlanificador->proximaInstruccion));
		log_info(logger,
				string_from_format("instruccion final %i",
						pcbPlanificador->instruccionFinal));
		log_info(logger,
				string_from_format("Id del proceso %i \n",
						pcbPlanificador->pid));
		pthread_mutex_unlock(&mutexCPULogs);
		//inicia toda la cadena de instruccion desde la CPU hacia memoria

//		printf("Ruta recibida del planificador: %s\n",
//				pcbPlanificador->rutaArchivoMcod);
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
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se ejecuta resultado iniciar proceso ok");
		log_info(logger,
				string_from_format("Id del proceso %i \n", datosDesdeMem->PID));
		pthread_mutex_unlock(&mutexCPULogs);
		//se ejecuta la siguiente instruccion si corresponde
		if (cpu->pcbPlanificador->tieneDesalojo == true) {
			if (cpu->pcbPlanificador->tamanioRafaga
					>= cpu->cantInstEjecutadas) {
				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resultadoAlPlanificador(cpu);
				cpu->estado = DISPONIBLE;
				cpu->cantInstEjecutadas = 0;
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

		t_PID* datosDesdeMem = (t_PID*) buffer;
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_PID)
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc  - Fallo ;\0"));
		string_append(&cpu->mCodCPU->respEjec->resultadosInstrucciones,
				string_from_format("mProc %i - Fallo ;\0", datosDesdeMem->PID));

		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se ejecuta resultado iniciar proceso no ok");
		log_info(logger,
				string_from_format("Id del proceso %i \n", datosDesdeMem->PID));
		pthread_mutex_unlock(&mutexCPULogs);
		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		cpu->mCodCPU->respEjec->finalizoOk = false;
		cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
		//ESTO HAY QUE CAMBIARLO EN EL PLANIFICADOR PARA QUE ANDE (OJO)
		enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
				cpu->mCodCPU->respEjec);
		free(cpu->mCodCPU->respEjec);
		cpu->mCodCPU->respEjec = creaRespuestaEjecucion();
		cpu->estado = DISPONIBLE;
		cpu->cantInstEjecutadas = 0;
		break;
	}

	case (RESUL_LEER_OK_CPU): {
		//fin de la instruccion leer
		cpu->cantInstEjecutadas += 1;
		cpu->estadoEjecucion = NO_USO;
		//se cuenta aca para tener en cuenta el retraso de pedirle a memoria
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
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se ejecuta resultado leer ok ");
		log_info(logger,
				string_from_format("Id del proceso %i ", datosDesdeMem->PID));
		log_info(logger,
				string_from_format("numero de pagina leida %i",
						datosDesdeMem->numeroPagina));
		log_info(logger,
				string_from_format("contenido leido %s \n",
						datosDesdeMem->contenido));
		pthread_mutex_unlock(&mutexCPULogs);
		//se ejecuta la siguiente instruccion si corresponde
		if (cpu->pcbPlanificador->tieneDesalojo == true) {
			if (cpu->pcbPlanificador->tamanioRafaga
					>= cpu->cantInstEjecutadas) {

				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
//				printf("dsp de ejecuta inst\n");
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resultadoAlPlanificador(cpu);
				cpu->estado = DISPONIBLE;
				cpu->cantInstEjecutadas = 0;
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
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se ejecuta resultado escribir proceso");
		log_info(logger,
				string_from_format("Id del proceso %i",
						datosdesdeMEmoria->PID));
		log_info(logger,
				string_from_format("numero de pagina escrita %i",
						datosdesdeMEmoria->numeroPagina));
		log_info(logger,
				string_from_format("contenido escrito %s \n",
						datosdesdeMEmoria->contenido));
		pthread_mutex_unlock(&mutexCPULogs);

		//se ejecuta la siguiente instruccion si corresponde
		if (cpu->pcbPlanificador->tieneDesalojo == true) {
			if (cpu->pcbPlanificador->tamanioRafaga
					>= cpu->cantInstEjecutadas) {
				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resultadoAlPlanificador(cpu);
				cpu->estado = DISPONIBLE;
				cpu->cantInstEjecutadas = 0;
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
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se ejecuta resultado fin de proceso");
		log_info(logger,
				string_from_format("Id del proceso %i", datosDesdeMem->PID));
		log_info(logger,
				string_from_format("resultado instrucciones: %s \n",
						cpu->mCodCPU->respEjec->resultadosInstrucciones));
		pthread_mutex_unlock(&mutexCPULogs);
//		printf("%s\n", cpu->mCodCPU->respEjec->resultadosInstrucciones);
		enviarStruct(socketPlanificador, RESUL_EJECUCION_OK,
				cpu->mCodCPU->respEjec);
		cpu->estado = DISPONIBLE;
		cpu->cantInstEjecutadas = 0;
		free(cpu->mCodCPU->respEjec);
		cpu->mCodCPU->respEjec = creaRespuestaEjecucion();
		break;
	}

	case (STRING): {
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se va a recibir un string ");
		pthread_mutex_unlock(&mutexCPULogs);
		break;
	}
	case (TIEMPO_CPU): {
		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;

		t_porcentajeCPUs* porcentajeCPUs = malloc(sizeof(t_porcentajeCPUs));

		porcentajeCPUs->respuestasPorcentaje = list_create();

		void tomaPorcentaje(t_cpu* cpu) {
			t_respuesta_porcentaje* respuestaPorcentaje = malloc(
					sizeof(t_respuesta_porcentaje));
			respuestaPorcentaje->res_porcentaje = cpu->porcentajeUso;
			respuestaPorcentaje->idCpu = cpu->idCPU;
			list_add(porcentajeCPUs->respuestasPorcentaje, respuestaPorcentaje);
		}

		list_iterate(procCPU->listaCPU, (void*) tomaPorcentaje);

		porcentajeCPUs->cantidadDeElementos = list_size(
				porcentajeCPUs->respuestasPorcentaje);
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se ejecuta pedido de porcentaje de CPUS ");
		t_respuesta_porcentaje *contenido = malloc(
				sizeof(t_respuesta_porcentaje));
		int a;
		for (a = 0; a < porcentajeCPUs->cantidadDeElementos; a++) {
			contenido = list_get(porcentajeCPUs->respuestasPorcentaje, a);
			log_info(logger,
					string_from_format(
							"ESTOY ENVIANDO A PLANIF CPU:  PORCENTAJE:%i",
							contenido->res_porcentaje));
//			printf("ESTOY ENVIANDO A PLANIF CPU:  PORCENTAJE: %i\n",
//					contenido->res_porcentaje);
		}
		log_info(logger, "fin envio porcentajes\n");
		pthread_mutex_unlock(&mutexCPULogs);
		enviarStruct(socketPlanificador, TIEMPO_CPU_RESUL, porcentajeCPUs);
		break;
	}
	}

}

