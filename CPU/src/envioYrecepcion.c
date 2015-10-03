#include "CPU.h"
//agregar comportamiento en cada break
//ejecutar todo tipo de comandos del mCod
int ejecutar(int token, char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {
        log_info(logger, "se va a ejecutar la funcion ejecutar");
	switch (token) {
	case (INICIAR_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar iniciar proceso memoria ");
		ejecutaIniciarProceso(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (ESCRIBIR_MEM): {
		log_info(logger, "se va a ejecutar escribir memoria");
		ejecutaEscribirMemoria(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (LEER_MEM): {
		log_info(logger, "se va a ejecutar leer memoria ");
		ejecutaLeerMemoria(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (FIN_PROCESO_MEM): {
		log_info(logger, "se va a ejecutar fin proceso memoria ");
		ejecutaFinProcesoMemoria(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (ENTRADA_SALIDA): {
		log_info(logger, "se va a ejecutar entrada salida ");
		ejecutaEntradaSalida(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	}

	return EXIT_SUCCESS;
}
//recibe las respuestas
int recibirMensajeVarios(int socket, int token,char* buffer, void* extra ) {
        log_info(logger, "se va a ejecutar recibirMensajeVarios ");
	switch (token) {
	case (RESUL_ERROR): {
		log_info(logger, "se va a ejecutar result error");
		ejecutaResultError(socket);

		break;
	}
	case (RESUL_ESCRIBIR): {
		log_info(logger, "se va a ejecutar result escribir ");
		ejecutaResultEscribir(socket);
		break;
	}
	case (RESUL_FIN): {
		log_info(logger, "se va a ejecutar result fin de proceso ");
		ejecutaResulFin(socket);
		break;
	}
	case (RESUL_INICIAR_PROC): {
		log_info(logger, "se va a ejecutar result iniciar proceso ");
		ejecutaResulIniciarProc(socket);
		break;
	}
	case (RESUL_INSTR_EJEC): {
		log_info(logger, "se va a ejecutar result instruccion ejecutar ");
		ejecutaResulInstrEjec(socket);
		break;
	}
	case (RESUL_LEER): {
		log_info(logger, "se va a ejecutar resultLeer ");
		ejecutaResultLeer(socket);
		break;
	}
	case (RESUL_OK): {
		log_info(logger, "se va a ejecutar resultOK ");
		ejecutaResulOk(socket);
		
		break;
	}
	case (CONTEXTO_MPROC):{
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
		//me marca error
//		t_pcb pcbProc = malloc(sizeof(t_pcb));
//		pcbProc = (t_pcb) buffer;
//		t_pcb*	pcbPlanificador = &pcbProc;
		t_pcb*	pcbPlanificador = (t_pcb*) buffer;
			printf("Ruta recibida del planificador: %s\n", pcbPlanificador->rutaArchivoMcod);
			preparaCPU(pcbPlanificador, socket);
			break;
	}
	
	case(STRING):{
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
	
	case(TIEMPO_CPU):{
			log_info(logger, "llega mensaje del planificador pidiendo el porcentaje del tiempo al CPU ");
	}
	
	}
	return EXIT_SUCCESS;
}
// todas las funciones de resultados de ejecucion deben devolver un string segun su tipo al planificador
int ejecutaResultError(int socket){
	return EXIT_SUCCESS;
}
int  ejecutaResultEscribir(int socket)
{
	return EXIT_SUCCESS;
}

int  ejecutaResulFin(int socket){
	return EXIT_SUCCESS;
}

int  ejecutaResulIniciarProc(int socket){
	return EXIT_SUCCESS;
}

int  ejecutaResulInstrEjec(int socket)
{
	return EXIT_SUCCESS;
}
int  ejecutaResultLeer(int socket){
	return EXIT_SUCCESS;
}

int  ejecutaResulOk(int socket){
	return EXIT_SUCCESS;
}

// en todas estas funciones de ejecutar se debe mandar al palnificador su estructura del PCB del proceso en cuestion
//debe mandar el inicio a memoria con serializacion correspondiente
int ejecutaIniciarProceso(char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {
	//TODO HACK usar un tipo especifico
	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura->PID = pcbPlanificador->pid;
	estructura->cantidadPaginas = atoi(string_split(separada_instruccion, ";")[0]);
	int socketMemoria = atoi((char*) dictionary_get(conexiones, "Memoria"));
	enviarStruct(socketMemoria, INICIAR_PROC_SWAP, estructura);

	return EXIT_SUCCESS;
}
//mandar comando a memoria con los datos y la pagina donde debe ser escrita
int ejecutaEscribirMemoria(char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {

	return EXIT_SUCCESS;
}
//mandar comando a memoria y  el numero de pagina que se debe leer
int ejecutaLeerMemoria(char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {

	return EXIT_SUCCESS;
}
//mandar el comando de finalizar y el respectivo PID IP del proceso
int ejecutaFinProcesoMemoria(char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {

	return EXIT_SUCCESS;
}
// mandar el proceso al planificador para que lo  ponga a dormir y en su cola de bloqueados
int ejecutaEntradaSalida(char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {

	return EXIT_SUCCESS;
}
