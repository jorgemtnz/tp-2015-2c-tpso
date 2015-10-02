#include "CPU.h"
//agregar comportamiento en cada break
//TODO conflictua con sockets.h
//void enviar(int tipoDeMensaje, void* t_estructura, int fdDestinatario) {
//
//	switch (tipoDeMensaje) {
//	case (INICIAR_PROCESO_MEM): {
//
//		break;
//	}
//	case (ESCRIBIR_MEM): {
//
//		break;
//	}
//	case (LEER_MEM): {
//
//		break;
//	}
//	case (FIN_PROCESO_MEM): {
//
//		break;
//	}
//
//	}
//
//}

int ejecutar(int token, char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {

	switch (token) {
	case (INICIAR_PROCESO_MEM): {
		ejecutaIniciarProceso(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (ESCRIBIR_MEM): {
		ejecutaEscribirMemoria(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (LEER_MEM): {
		ejecutaLeerMemoria(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (FIN_PROCESO_MEM): {
		ejecutaFinProcesoMemoria(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	case (ENTRADA_SALIDA): {
		ejecutaEntradaSalida(separada_instruccion, pcbPlanificador, socket);
		break;
	}
	}

	return EXIT_SUCCESS;
}
//recibe las respuestas
int recibirMensajeVarios(int socket, int token) {

	switch (token) {
	case (RESUL_ERROR): {
		ejecutaResultError(socket);

		break;
	}
	case (RESUL_ESCRIBIR): {
		ejecutaResultEscribir(socket);
		break;
	}
	case (RESUL_FIN): {
		ejecutaResulFin(socket);
		break;
	}
	case (RESUL_INICIAR_PROC): {
		ejecutaResulIniciarProc(socket);
		break;
	}
	case (RESUL_INSTR_EJEC): {
		ejecutaResulInstrEjec(socket);
		break;
	}
	case (RESUL_LEER): {
		ejecutaResultLeer(socket);
		break;
	}
	case (RESUL_OK): {
		ejecutaResulOk(socket);
		break;
	}
	case (CONTEXTO_MPROC):{
		log_info(logger, "llega mensaje CONTEXTO_MPROC ");
-			t_pcb* pcbPlanificador = (t_pcb*) buffer;
-			printf("Ruta recibida del planificador: %s\n", pcbPlanificador->rutaArchivoMcod);
-			preparaCPU(pcbPlanificador, socket);
	}
	
	case(STRING):{
		if (header->tipoMensaje == STRING) {
//
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
	}
	
	case(TIEMPO_CPU):{
			log_info(logger, "llega mensaje del planificador pidiendo el porcentaje del tiempo al CPU ");
	}
	
	}
	return EXIT_SUCCESS;
}

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


//debe mandar el inicio a memoria con serializacion correspondiente
int ejecutaIniciarProceso(char* separada_instruccion, t_pcb*pcbPlanificador, int socket) {

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
