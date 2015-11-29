#include "CPU.h"
//agregar comportamiento en cada break
//ejecutar todo tipo de comandos del mCod
void ejecutar(int token, char** separada_instruccion, t_cpu* cpu) {

	//ya tengo creado un mCod y con respEjec que vinieron en el contexto

	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar la funcion ejecutar");
	pthread_mutex_unlock(&mutexCPULogs);

	switch (token) {
	case (INICIAR_PROCESO_MEM): { //aca entra por primera vez
		ejecuta_IniciarProceso(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, INICIAR_PROCESO_MEM,
				cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		cpu->quantumReloj += 1;
		break;
	}
	case (ESCRIBIR_MEM): {
		ejecuta_EscribirMemoria(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, ESCRIBIR_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		cpu->quantumReloj += 1;
		break;
	}
	case (LEER_MEM): {
		ejecuta_LeerMemoria(separada_instruccion, cpu);
		//int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
		int socketMemoria = cpu->socketMemoria;
		enviarStruct(socketMemoria, LEER_MEM, cpu->estructuraSolicitud);
		free(cpu->estructuraSolicitud);
		cpu->estructuraSolicitud = NULL;
		cpu->quantumReloj += 1;
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
		cpu->quantumReloj += 1;
		break;
	}
	case (ENTRADA_SALIDA): {
//		printf("estoy en entrada salida\n");

		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		ejecuta_EntradaSalida(separada_instruccion, cpu, socketPlanificador);

//		destmCod(cpu->mCodCPU);
		cpu->estado = SI_TERMINO_RAFAGA;
//		printf("\n %i \n", cpu->pcbPlanificador->pid);
//		printf("cpu nombre%s, id %lu\n", cpu->nombre, cpu->idCPU);

		puts(
				string_from_format(
						"se ponen en NULL pcbPlanificador y MCodCPU %s  %s PID %i \n",
						queCPUsoy(cpu), identificaCPU(cpu->idCPU),
						cpu->pcbPlanificador->pid));
//		cpu->mCodCPU=NULL;
		cpu->pcbPlanificador = NULL;
		cpu->quantumReloj += 1;
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
	case (CONTEXTO_MPROC): {//cuando llega aca es porque es la primera vez sea FIFO o RR
//		cpu->pcbPlanificador->proximaInstruccion = 0 que viene del planificador
		t_pcb* pcbPlanificador = (t_pcb*) buffer;
		printf("contexto recibido pcb-> PID recibido %i \n",
				pcbPlanificador->pid);
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
		cpu->estado = NO_TERMINO_RAFAGA;
		if (pcbPlanificador->proximaInstruccion != 0) {
			//significa que no es la primra vez que llega
			destmCod(cpu->mCodCPU);
			destPCB(cpu->pcbPlanificador);
			printf("AAA\n");
		}

		cpu->pcbPlanificador = pcbPlanificador;
		//en procesaCodigo() se crea la respEjec del mCod
		printf("BBBBBB\n");
		procesaCodigo(cpu);	//aca dentro se actualiza la cpu->pcbPlanificador->proximaInstruccion y será distinta de cero
		break;
	}

	case (RESUL_INICIAR_PROC_OK_CPU): {
		//se cuenta aca como terminado de ejecutar la instruccion iniciar
		cpu->cantInstEjecutadasPorcentaje += 1;
		cpu->terminaInstruccion = SI_TERMINO;
		//recibe desde memoria y debe continuar con la ejecucion
//primera vez que se esta usando la respuesta , fue creada creaRespuestaEjecucion()
		t_PID* datosDesdeMem = (t_PID*) buffer;

		if (cpu->pcbPlanificador->pid!=datosDesdeMem->PID) {
			printf(
					"recibo de memoria PID %i que no es el que esta ejecutando la cpu \n",
					datosDesdeMem->PID);
			break;
		}
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
		if (cpu->pcbPlanificador->tieneDesalojo == true) {	//estoy en RR
			if (cpu->pcbPlanificador->tamanioRafaga > cpu->quantumReloj) {
				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas
//espera que regrese el proceso en un futuro porque no ha terminado
				resul_noTerminoAlPlanificador(cpu);
				cpu->estado = SI_TERMINO_RAFAGA;

				cpu->quantumReloj = 0;
				puts(
						string_from_format(
								"se envia por Quantum %s  %s PID %i\n",
								queCPUsoy(cpu), identificaCPU(cpu->idCPU),
								cpu->pcbPlanificador->pid));

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
		cpu->cantInstEjecutadasPorcentaje += 1;
		cpu->terminaInstruccion = SI_TERMINO;
		//al dar error se debe devolver el proceso

		t_PID* datosDesdeMem = (t_PID*) buffer;
		if (cpu->pcbPlanificador->pid!=datosDesdeMem->PID) {
			printf(
					"recibo de memoria PID %i que no es el que esta ejecutando la cpu \n",
					datosDesdeMem->PID);
			break;
		}
		//la respuesta ya fue creada antes y ya se hizo resultadosInstrucciones = string_new()
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

		cpu->mCodCPU->respEjec->finalizoOk = true; //finalizo entonces ya no se manda nada de regreso
		cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;

		enviarStruct(socketPlanificador, RESUL_EJECUCION_ERROR,
				cpu->mCodCPU->respEjec);
//		destmCod(cpu->mCodCPU); //liero espacio porque luego reservo cuando me llega otro proceso
		cpu->quantumReloj = 0;
		cpu->estado = SI_TERMINO_RAFAGA;
		puts(
				string_from_format(
						"se envia por proceso iniciar no ok %s  %s PID %i\n",
						queCPUsoy(cpu), identificaCPU(cpu->idCPU),
						cpu->pcbPlanificador->pid));
//		puts(
//				string_from_format(
//						"se ponen en NULL pcbPlanificador y MCodCPU %s  %s PID %i \n",
//						queCPUsoy(cpu), identificaCPU(cpu->idCPU),
//						cpu->pcbPlanificador->pid));
//		cpu->pcbPlanificador = NULL;
//		cpu->mCodCPU = NULL;
		break;
	}

	case (RESUL_LEER_OK_CPU): {
		//fin de la instruccion leer
		cpu->cantInstEjecutadasPorcentaje += 1;
		cpu->terminaInstruccion = SI_TERMINO;
		//se cuenta aca para tener en cuenta el retraso de pedirle a memoria
		t_contenido_pagina* datosDesdeMem = (t_contenido_pagina*) buffer;
		if (cpu->pcbPlanificador->pid!=datosDesdeMem->PID){
			printf(
					"recibo de memoria PID %i que no es el que esta ejecutando la cpu \n",
					datosDesdeMem->PID);
			break;
		}

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
			if (cpu->pcbPlanificador->tamanioRafaga > cpu->quantumReloj) { //estoy en RR

				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
//				printf("dsp de ejecuta inst\n");
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas
//estoy en RR y se debe ir
				resul_noTerminoAlPlanificador(cpu);
				cpu->estado = SI_TERMINO_RAFAGA;

				cpu->quantumReloj = 0;
				puts(
						string_from_format(
								"se envia por Quantum %s  %s PID %i\n",
								queCPUsoy(cpu), identificaCPU(cpu->idCPU),
								cpu->pcbPlanificador->pid));
//				puts(
//						string_from_format(
//								"se ponen en NULL pcbPlanificador y MCodCPU %s  %s PID %i \n",
//								queCPUsoy(cpu), identificaCPU(cpu->idCPU),
//								cpu->pcbPlanificador->pid));
//				cpu->pcbPlanificador = NULL;
//				cpu->mCodCPU = NULL;
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
		cpu->cantInstEjecutadasPorcentaje += 1;
		cpu->terminaInstruccion = SI_TERMINO;
		//++++++++++++++cpu libre

		t_contenido_pagina* datosdesdeMEmoria = (t_contenido_pagina*) buffer;
//		printf("\n contenido: %s // PID:%i //PAG:%i\n",datosdesdeMEmoria->contenido,datosdesdeMEmoria->PID,datosdesdeMEmoria->numeroPagina);
//		printf("\n %i \n",cpu->pcbPlanificador->pid);
//		printf("cpu nombre%s, id %lu\n", cpu->nombre, cpu->idCPU);
		if (cpu->pcbPlanificador->pid!=datosdesdeMEmoria->PID) {
			printf(
					"recibo de memoria PID %i que no es el que esta ejecutando la cpu \n",
					datosdesdeMEmoria->PID);
			break;
		}

		// se asigna espacio contiguo para los datos del resultado
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_contenido_pagina) + 1
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc; - Pagina; escrita:\0"));

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
		if (cpu->pcbPlanificador->tieneDesalojo == true) { //RR
			if (cpu->pcbPlanificador->tamanioRafaga > cpu->quantumReloj) {
				ejecuta_Instruccion(
						cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
						cpu);
			} else { //devuelve el resultado con el string de las instrucciones ya ejecutadas

				resul_noTerminoAlPlanificador(cpu);
				cpu->estado = SI_TERMINO_RAFAGA;
				cpu->quantumReloj = 0;
				puts(
						string_from_format(
								"se envia por Quantum %s  %s PID %i\n",
								queCPUsoy(cpu), identificaCPU(cpu->idCPU),
								cpu->pcbPlanificador->pid));
//				puts(
//						string_from_format(
//								"se ponen en NULL pcbPlanificador y MCodCPU %s  %s PID %i \n",
//								queCPUsoy(cpu), identificaCPU(cpu->idCPU),
//								cpu->pcbPlanificador->pid));
//				cpu->pcbPlanificador = NULL;
//				cpu->mCodCPU = NULL;
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
		cpu->cantInstEjecutadasPorcentaje += 1;
		cpu->terminaInstruccion = SI_TERMINO;
		t_PID* datosDesdeMem = (t_PID*) buffer;
		if (cpu->pcbPlanificador->pid!=datosDesdeMem->PID){
			printf(
					"recibo de memoria PID %i que no es el que esta ejecutando la cpu \n",
					datosDesdeMem->PID);
			break;
		}
		if (cpu->mCodCPU->respEjec == NULL) {
			cpu->mCodCPU->respEjec = malloc(sizeof(t_respuesta_ejecucion));
		}
		cpu->mCodCPU->respEjec->resultadosInstrucciones = realloc(
				cpu->mCodCPU->respEjec->resultadosInstrucciones,
				sizeof(t_PID)
						+ strlen(
								cpu->mCodCPU->respEjec->resultadosInstrucciones)
						+ 1 + strlen("mProc %i - finalizado ;\0"));
		//int socketPlanificador = atoi((char*) dictionary_get(conexiones, "Planificador"));
		int socketPlanificador = cpu->socketPlanificador;
		//		++++++++++++++++++++++funcion finalizar
		cpu->mCodCPU->respEjec->finalizoOk = true; //finalizo entonces ya no se manda nada
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
		cpu->estado = SI_TERMINO_RAFAGA;
//		destmCod(cpu->mCodCPU); //libero para esperar al proximo

		puts(
				string_from_format("se envia por finalizar %s  %s PID %i \n",
						queCPUsoy(cpu), identificaCPU(cpu->idCPU),
						cpu->pcbPlanificador->pid));

//		puts(
//				string_from_format(
//						"se ponen en NULL pcbPlanificador y MCodCPU %s  %s PID %i \n",
//						queCPUsoy(cpu), identificaCPU(cpu->idCPU),
//						cpu->pcbPlanificador->pid));
//		puts(
//				string_from_format(
//						"se ponen en NULL pcbPlanificador y MCodCPU %s  %s PID %i \n",
//						queCPUsoy(cpu), identificaCPU(cpu->idCPU),
//						cpu->pcbPlanificador->pid));
//		cpu->pcbPlanificador = NULL;
//		cpu->mCodCPU = NULL;
		cpu->quantumReloj = 0;
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

