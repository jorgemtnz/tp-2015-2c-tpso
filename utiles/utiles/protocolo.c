#include "protocolo.h"

//+++++++++++++++++++++++++funciones para el manejo del protocolo++++++++++++++++++++++++++++++++++

//objetivo devolver el paquete para el envio por socket

PaqueteEnvio* serializar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad) {
	PaqueteEnvio* paqueteAEnviar = malloc(sizeof(PaqueteEnvio));
	size_t tamanioDelMensaje = tamanioPayLoad + sizeof(int); //+4 por el total del mensaje, le sumo un sizeof(int) por el header

	paqueteAEnviar->mensaje = malloc(tamanioDelMensaje);
	memcpy(paqueteAEnviar->mensaje, &tipoDeMensaje, sizeof(int));
	size_t desplazameinto = sizeof(int);
	memcpy(paqueteAEnviar->mensaje + desplazameinto, payLoad, tamanioPayLoad);

	paqueteAEnviar->tamanioMensaje = tamanioDelMensaje;
	return paqueteAEnviar;
}

// devolver el paquete para recepcion( y luego interpretar el paquete, referido al switch con los tokens)
Paquete* deserializar(void* buffer, int tamnioMensaje) {
	Paquete* paqueteDeserial = malloc(sizeof(Paquete));
	memcpy(&(paqueteDeserial->tipoDeMensaje), buffer, sizeof(int));
	int dezplazamiento = sizeof(int);
	size_t tamanioALeerDelBuffer = tamnioMensaje - dezplazamiento;
	paqueteDeserial->payLoad = malloc(tamanioALeerDelBuffer);
	memcpy(paqueteDeserial->payLoad, buffer + dezplazamiento, tamanioALeerDelBuffer);
	return paqueteDeserial;
}

void* serializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_CONTEXTO_MPROC");
	serializar_t_pcb(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_pcb* estructura = deserializar_t_pcb(fdCliente, tipoMensaje);
	puts("Deserializando serializar_CONTEXTO_MPROC");
	return estructura;
}
void* serializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_OK");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_INICIAR_PROC_OK");
	return estructura;
}

void* serializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_OK");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_INICIAR_PROC_OK");
	return estructura;
}

void* serializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_ESCRIBIR_OK");
	serializar_t_respuesta_escribir(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* serializar_RESUL_ESCRIBIR_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_ESCRIBIR_ERROR");
	serializar_t_respuesta_escribir(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_ESCRIBIR_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_escribir* estructura = deserializar_t_respuesta_escribir(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_ESCRIBIR_ERROR");
	return estructura;
}

void* deserializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_escribir* estructura = deserializar_t_respuesta_escribir(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_ESCRIBIR_OK");
	return estructura;
}
void* serializar_t_respuesta_escribir(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_escribir* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);
	serializar_string(fdCliente, estructura->contenido);
	serializar_int8_t(fdCliente, estructura->numeroPagina);

	return 0;
}

void* serializar_RESUL_LEER_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_LEER_OK_CPU");
	serializar_t_respuesta_leer_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_LEER_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_leer* estructura = deserializar_t_respuesta_leer_CPU(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_LEER_OK_CPU");
	return estructura;
}

void* serializar_t_respuesta_leer_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_leer* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);
	serializar_string(fdCliente, estructura->contenido);
	serializar_int8_t(fdCliente, estructura->numeroPagina);

	return 0;
}
t_respuesta_leer_CPU* deserializar_t_respuesta_leer_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_leer_CPU* estructura = malloc(sizeof(t_respuesta_leer_CPU));
	estructura->PID = deserializar_int8_t(fdCliente);
	estructura->contenido = deserializar_string(fdCliente);
	estructura->numeroPagina = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_RESUL_FIN(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_FIN");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_FIN(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_FIN");
	return estructura;
}


void* serializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_LEER_OK");
	serializar_t_respuesta_leer(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* serializar_RESUL_LEER_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_LEER_ERROR");
	serializar_t_respuesta_leer(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_LEER_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_leer* estructura = deserializar_t_respuesta_leer(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_LEER_ERROR");
	return estructura;
}

void* deserializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_leer* estructura = deserializar_t_respuesta_leer(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_LEER_OK");
	return estructura;
}

void* serializar_RESUL_TRAER_PAG_SWAP_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	serializar_t_lectura_Swap(fdCliente, tipoMensaje, estructura);
}
void* deserializar_RESUL_TRAER_PAG_SWAP_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return deserializar_t_lectura_Swap(fdCliente, tipoMensaje);
}
void* serializar_t_lectura_Swap(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	t_lectura_Swap* lecturaSwap = (t_lectura_Swap*) estructura;
	serializar_int8_t(fdCliente, lecturaSwap->idProc);
	serializar_int8_t(fdCliente, lecturaSwap->pag);
	serializar_string(fdCliente, lecturaSwap->contenido);
}
void* deserializar_t_lectura_Swap(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_lectura_Swap* lecturaSwap = malloc(sizeof(t_lectura_Swap));
	lecturaSwap->idProc = deserializar_int8_t(fdCliente);
	lecturaSwap->pag = deserializar_int8_t(fdCliente);
	lecturaSwap->contenido = deserializar_string(fdCliente);
	return lecturaSwap;
}

void* serializar_t_respuesta_leer(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_leer* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);
	serializar_string(fdCliente, estructura->contenido);
	serializar_int8_t(fdCliente, estructura->numeroPagina);

	return 0;
}
t_respuesta_leer* deserializar_t_respuesta_leer(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_leer* estructura = malloc(sizeof(t_respuesta_leer));
	estructura->PID = deserializar_int8_t(fdCliente);
	estructura->contenido = deserializar_string(fdCliente);
	estructura->numeroPagina = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_FIN_PROCESO_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_FIN_PROCESO_SWAP");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_FIN_PROCESO_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_FIN_PROCESO_SWAP");
	return estructura;
}
void* serializar_t_PID(int fdCliente, t_tipo_mensaje tipoMensaje, t_PID* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);

	return 0;
}
t_PID* deserializar_t_PID(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = malloc(sizeof(t_PID));
	estructura->PID = deserializar_int8_t(fdCliente);

	return estructura;
}


void* serializar_t_rtaLecturaCpu(int fdCliente, t_tipo_mensaje tipoMensaje,t_rtaLecturaCpu* estructura){

	serializar_int8_t(fdCliente, estructura->idProc);
	serializar_int8_t(fdCliente, estructura->pag);
	serializar_string(fdCliente, estructura->contenido);
}

t_rtaLecturaCpu* deserializar_t_rtaLecturaCpu(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_rtaLecturaCpu* estructura = malloc(sizeof(t_rtaLecturaCpu));
	estructura->idProc = deserializar_int8_t(fdCliente);
	estructura->pag = deserializar_int8_t(fdCliente);
	estructura->contenido = deserializar_string(fdCliente);

	return estructura;
}

void* serializar_LEER_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_LEER_SWAP");
	serializar_t_leerDeProceso(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_LEER_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerDeProceso* estructura = deserializar_t_leerDeProceso(fdCliente, tipoMensaje);
	puts("Deserializando serializar_LEER_SWAP");
	return estructura;
}

void* serializar_LEER_MEM(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_LEER_MEM");
	serializar_t_leerMem(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_LEER_MEM(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerMem* estructura = deserializar_t_leerMem(fdCliente, tipoMensaje);
	puts("Deserializando serializar_LEER_MEM");
	return estructura;
}

//typedef struct{
//	uint8_t PID;
//	uint8_t pagina;
//	char* texto;
//}t_leerMem;

void* serializar_t_leerMem(int fdCliente, t_tipo_mensaje tipoMensaje, t_leerMem* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);
	serializar_int8_t(fdCliente, estructura->pagina);
	serializar_string(fdCliente, estructura->texto);

	return 0;
}
t_leerMem* deserializar_t_leerMem(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerMem* estructura = malloc(sizeof(t_leerMem));
	estructura->PID = deserializar_int8_t(fdCliente);
	estructura->pagina = deserializar_int8_t(fdCliente);
	estructura->texto = deserializar_string(fdCliente);

	return estructura;
}

void* serializar_t_leerDeProceso(int fdCliente, t_tipo_mensaje tipoMensaje, t_leerDeProceso* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);
	serializar_int8_t(fdCliente, estructura->numeroPaginaFin);
	serializar_int8_t(fdCliente, estructura->numeroPaginaInicio);

	return 0;
}
t_leerDeProceso* deserializar_t_leerDeProceso(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerDeProceso* estructura = malloc(sizeof(t_leerDeProceso));
	estructura->PID = deserializar_int8_t(fdCliente);
	estructura->numeroPaginaFin = deserializar_int8_t(fdCliente);
	estructura->numeroPaginaInicio = deserializar_int8_t(fdCliente);

	return estructura;
}
void serializar_RESUL_EJECUCION_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	serializar_t_respuesta_ejecucion(fdCliente, tipoMensaje, estructura);
}
void* deserializar_RESUL_EJECUCION_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return deserializar_t_respuesta_ejecucion(fdCliente, tipoMensaje);
}

void serializar_t_respuesta_ejecucion(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
//	t_pcb* pcb; //aca dentro ya esta el PID del proceso
//		t_list* resultadosInstrucciones;
//		bool finalizoOk;
	t_respuesta_ejecucion* respuestaEjecucion = (t_respuesta_ejecucion*) estructura;
	serializar_t_pcb(fdCliente, tipoMensaje, respuestaEjecucion->pcb);

	serializar_bool(fdCliente, respuestaEjecucion->finalizoOk);


	serializar_int8_t(fdCliente, list_size(respuestaEjecucion->resultadosInstrucciones));
	void serializar_t_resultado_instruccion_adapter(void* elemento) {
		serializar_t_resultado_instruccion(fdCliente, tipoMensaje, elemento);
	}

	list_iterate(respuestaEjecucion->resultadosInstrucciones, serializar_t_resultado_instruccion_adapter);
}

t_respuesta_ejecucion* deserializar_t_respuesta_ejecucion(int fdCliente, t_tipo_mensaje tipoMensaje) {

	t_respuesta_ejecucion* respuestaEjecucion = malloc(sizeof(t_respuesta_ejecucion));
	respuestaEjecucion->pcb = deserializar_t_pcb(fdCliente, tipoMensaje);
	respuestaEjecucion->finalizoOk = deserializar_bool(fdCliente);

	int8_t cantidadResultadosInstrucciones = deserializar_int8_t(fdCliente);

	respuestaEjecucion->resultadosInstrucciones = list_create();
	int var;
	for (var = 0; var < cantidadResultadosInstrucciones; var++) {
		list_add(respuestaEjecucion->resultadosInstrucciones, deserializar_t_resultado_instruccion(fdCliente, tipoMensaje));
	}

	return respuestaEjecucion;
}

void serializar_t_resultado_instruccion(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {

	t_resultado_instruccion* resultadoInstruccion = (t_resultado_instruccion*) estructura;
	// confuso por que la estructura tambien es un tipo mensaje (pero de una ejecucion anterior)
	serializar_t_tipo_mensaje(fdCliente, tipoMensaje, resultadoInstruccion->tipoMensaje);
	serializar_string(fdCliente, resultadoInstruccion->comandoInstruccion);
	serializar_string(fdCliente, resultadoInstruccion->expresion);
}
t_resultado_instruccion* deserializar_t_resultado_instruccion(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_resultado_instruccion* resultadoInstruccion = malloc(sizeof(t_resultado_instruccion));
	resultadoInstruccion->tipoMensaje = deserializar_t_tipo_mensaje(fdCliente, tipoMensaje);
	resultadoInstruccion->comandoInstruccion = deserializar_string(fdCliente);
	resultadoInstruccion->expresion = deserializar_string(fdCliente);
	return resultadoInstruccion;
}

void serializar_t_tipo_mensaje(int fdCliente, t_tipo_mensaje tipoMensaje, t_tipo_mensaje estructura) {
	enviarSimple(fdCliente, estructura, sizeof(t_tipo_mensaje));
}
t_tipo_mensaje deserializar_t_tipo_mensaje(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_tipo_mensaje* tipoMensajeARecibir = malloc(sizeof(t_tipo_mensaje));
	recibirPorSocket(fdCliente, tipoMensajeARecibir, sizeof(t_tipo_mensaje));
	return *tipoMensajeARecibir;
}

void* serializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_TRAER_PAG_SWAP");
	//serializar_lecturaProc_desde_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
t_PID* deserializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_TRAER_PAG_SWAP");
	return estructura;
}

void* serializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_FIN_OK");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* serializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_FIN_ERROR");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_FIN_ERROR");
	return estructura;
}

void* deserializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_FIN_OK");
	return estructura;
}

void* serializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_NO_OK_CPU");
	serializar_t_rta_iniciar_no_ok_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
t_respuesta_escribir* deserializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_escribir* estructura = deserializar_t_rta_iniciar_no_ok_CPU(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_INICIAR_PROC_NO_OK_CPU");
	return estructura;
}

void* serializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_OK_CPU");
	serializar_t_rta_iniciar_ok_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
t_rta_iniciar_CPU* deserializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_rta_iniciar_CPU* estructura = deserializar_t_rta_iniciar_ok_CPU(fdCliente, tipoMensaje);
	puts("Deserializando serializar_RESUL_INICIAR_PROC_OK_CPU");
	return estructura;
}

t_respuesta_escribir* deserializar_t_respuesta_escribir(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_escribir* estructura = malloc(sizeof(t_respuesta_escribir));
	estructura->PID = deserializar_int8_t(fdCliente);
	estructura->contenido = deserializar_string(fdCliente);
	estructura->numeroPagina = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_PID* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);

	return 0;
}
t_rta_iniciar_CPU* crearRespuestaIniciarOkCPU() {
	t_rta_iniciar_CPU* contenido = malloc(sizeof(t_rta_iniciar_CPU));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearRespuestaIniciarOkCPU");
//		log_error(logger, "[ERROR] No se reservo memoria para crearRespuestaIniciarOkCPU");
		exit(-1);
	}
	return contenido;
}

t_rta_iniciar_CPU* deserializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_rta_iniciar_CPU* estructura = crearRespuestaIniciarOkCPU();
	estructura->PID = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_t_rta_iniciar_no_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_PID* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);

	return 0;
}

t_rta_iniciar_CPU* deserializar_t_rta_iniciar_no_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_rta_iniciar_CPU* estructura = crearRespuestaIniciarOkCPU();
	estructura->PID = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje, t_pcb* estructura) {
	serializar_int8_t(fdCliente, estructura->pid);
	serializar_string(fdCliente, estructura->rutaArchivoMcod);
	serializar_bool(fdCliente, estructura->tieneDesalojo);
	serializar_int16_t(fdCliente, estructura->tamanioRafaga);
	serializar_int16_t(fdCliente, estructura->proximaInstruccion);
	return 0;
}
t_pcb* deserializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_pcb* estructura = malloc(sizeof(t_pcb));
	estructura->pid = deserializar_int8_t(fdCliente);
	estructura->rutaArchivoMcod = deserializar_string(fdCliente);
	estructura->tieneDesalojo = deserializar_bool(fdCliente);
	estructura->tamanioRafaga = deserializar_int16_t(fdCliente);
	estructura->proximaInstruccion = deserializar_int16_t(fdCliente);
	return estructura;
}

void* serializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_INICIAR_PROC_SWAP");
	serializar_t_iniciar_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = deserializar_t_iniciar_swap(fdCliente, tipoMensaje);
	puts("Deserializando serializar_INICIAR_PROC_SWAP");
	return estructura;
}

void* serializar_INICIAR_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando INICIAR_PROCESO_MEM");
	serializar_t_iniciar_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_INICIAR_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = deserializar_t_iniciar_swap(fdCliente, tipoMensaje);
	puts("Deserializando serializar_INICIAR_PROCESO_MEM");
	return estructura;
}

void* serializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje, t_iniciar_swap* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);
	serializar_int16_t(fdCliente, estructura->cantidadPaginas);
	return 0;
}
t_iniciar_swap* deserializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura->PID = deserializar_int8_t(fdCliente);
	estructura->cantidadPaginas = deserializar_int16_t(fdCliente);
	return estructura;
}
void serializar_string(int fdCliente, char* estructura) {
	int16_t tamanioString = strlen(estructura);
	serializar_int16_t(fdCliente, tamanioString);
	enviarSimple(fdCliente, estructura, tamanioString);
}
char* deserializar_string(int fdCliente) {
	int16_t tamanioString = deserializar_int16_t(fdCliente);
	char* string = malloc(tamanioString + 1);
	recibirPorSocket(fdCliente, string, tamanioString);
	string[tamanioString] = '\0';
	return string;
}

void serializar_int16_t(int fdCliente, int16_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(int16_t));
}
int16_t deserializar_int16_t(int fdCliente) {
	int16_t* res = malloc(sizeof(int16_t));
	recibirPorSocket(fdCliente, res, sizeof(int16_t));
	return *res;
}

void serializar_int8_t(int fdCliente, int8_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(int8_t));
}
int8_t deserializar_int8_t(int fdCliente) {
	int8_t* res = malloc(sizeof(int8_t));
	recibirPorSocket(fdCliente, res, sizeof(int8_t));
	return *res;
}

void serializar_bool(int fdCliente, bool estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(bool));
}
bool deserializar_bool(int fdCliente) {
	bool* res = malloc(sizeof(bool));
	recibirPorSocket(fdCliente, res, sizeof(bool));
	return *res;
}

void serializar_pid_t(int fdCliente, pid_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(pid_t));
}
pid_t deserializar_pid_t(int fdCliente) {
	pid_t* res = malloc(sizeof(pid_t));
	recibirPorSocket(fdCliente, res, sizeof(pid_t));
	return *res;
}
