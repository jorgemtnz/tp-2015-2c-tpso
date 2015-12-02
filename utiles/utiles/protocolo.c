#include "protocolo.h"
#include "sockets.h"
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
	//puts("Serializando serializar_CONTEXTO_MPROC");
	serializar_t_pcb(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_CONTEXTO_MPROC(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_pcb* estructura = deserializar_t_pcb(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_CONTEXTO_MPROC");
	return estructura;
}
void* serializar_HANDSHAKE_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	serializar_string(fdCliente, estructura);
	return 0;
}
void* deserializar_HANDSHAKE_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return deserializar_string(fdCliente);
}
void* serializar_HANDSHAKE_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	return 0;
}
void* deserializar_HANDSHAKE_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return NULL;
}

void* serializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_INICIAR_PROC_OK");
	serializar_t_iniciar_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = deserializar_t_iniciar_swap(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_INICIAR_PROC_OK");
	return estructura;
}

void* serializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_INICIAR_PROC_OK");
	serializar_t_iniciar_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = deserializar_t_iniciar_swap(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_INICIAR_PROC_OK");
	return estructura;
}

void* serializar_FIN_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_FIN_PROCESO_MEM");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_FIN_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_FIN_PROCESO_MEM");
	return estructura;
}

void* serializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_ESCRIBIR_OK");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_ESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_ESCRIBIR_OK");
	return estructura;
}

void* serializar_RESUL_LEER_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_LEER_OK_CPU");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_LEER_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_LEER_OK_CPU");
	return estructura;
}

void* serializar_SOBREESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_SOBREESCRIBIR_SWAP");
	serializar_t_sobreescribir_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_SOBREESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_sobreescribir_swap* estructura = deserializar_t_sobreescribir_swap(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_SOBREESCRIBIR_SWAP");
	return estructura;
}

void* serializar_RESUL_SOBREESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_SOBREESCRIBIR_SWAP");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_SOBREESCRIBIR_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_SOBREESCRIBIR_SWAP");
	return estructura;
}

void* serializar_LEER_SWAP_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_LEER_SWAP_POR_ESCRIBIR");
	//serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	serializar_t_leerDeProcesoPorEscribir(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_LEER_SWAP_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	//t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_LEER_SWAP_POR_ESCRIBIR");
	t_leerDeProcesoPorEscribir* estructura = deserializar_t_leerDeProcesoPorEscribir(fdCliente, tipoMensaje);
	return estructura;
}

void* serializar_RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_LEER_SWAP_POR_ESCRIBIR");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_TRAER_PAG_SWAP_OK_POR_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_LEER_SWAP_POR_ESCRIBIR");
	return estructura;
}

void* serializar_RESUL_TRAER_PAG_SWAP_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_TRAER_PAG_SWAP_OK");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_TRAER_PAG_SWAP_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_RESUL_TRAER_PAG_SWAP_OK");
	return estructura;
}

void* serializar_RESUL_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_TRAER_PAG_SWAP_OK");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_ESCRIBIR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_RESUL_TRAER_PAG_SWAP_OK");
	return estructura;
}

//OK
void serializar_t_sobreescribir_swap(int fdCliente, t_tipo_mensaje tipoMensaje, t_sobreescribir_swap* estructura) {
	serializar_int8_t(fdCliente, estructura->PIDAReemplazar);debug("%s Socket: %d, PIDaReemplazar enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PIDAReemplazar);
	serializar_int8_t(fdCliente, estructura->PIDAResponderleAMemoria);debug("%s Socket: %d, PIDaResponderleAMemoria enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PIDAResponderleAMemoria);
	serializar_string(fdCliente, estructura->contenido);
	serializar_int8_t(fdCliente, estructura->numeroPagina);
}

//OK
t_sobreescribir_swap* deserializar_t_sobreescribir_swap(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_sobreescribir_swap* estructura = malloc(sizeof(t_sobreescribir_swap));
	estructura->PIDAReemplazar = deserializar_int8_t(fdCliente);debug("%s Socket %d, PIDaReemplazar recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PIDAReemplazar);
	estructura->PIDAResponderleAMemoria = deserializar_int8_t(fdCliente);debug("%s Socket %d, PIDaResponderleAMemoria recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PIDAResponderleAMemoria);
	estructura->contenido = deserializar_string(fdCliente);
	estructura->numeroPagina = deserializar_int8_t(fdCliente);

	return estructura;
}

//OK
void serializar_t_contenido_pagina(int fdCliente, t_tipo_mensaje tipoMensaje, t_contenido_pagina* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	serializar_string(fdCliente, estructura->contenido);
	serializar_int8_t(fdCliente, estructura->numeroPagina);
}

//OK
t_contenido_pagina* deserializar_t_contenido_pagina(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	estructura->contenido = deserializar_string(fdCliente);
	estructura->numeroPagina = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_RESUL_FIN(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_FIN");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_RESUL_FIN(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_FIN");
	return estructura;
}

void* serializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_LEER_OK");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_RESUL_LEER_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_LEER_OK");
	return estructura;
}


void* serializar_FIN_PROCESO_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_FIN_PROCESO_SWAP");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_FIN_PROCESO_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_FIN_PROCESO_SWAP");
	return estructura;
}
void* serializar_t_PID(int fdCliente, t_tipo_mensaje tipoMensaje, t_PID* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);

	return 0;
}
t_PID* deserializar_t_PID(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = malloc(sizeof(t_PID));
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);

	return estructura;
}

void* serializar_LEER_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_LEER_SWAP");
	serializar_t_leerDeProceso(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_LEER_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerDeProceso* estructura = deserializar_t_leerDeProceso(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_LEER_SWAP");
	return estructura;
}

void* serializar_ERROR_EJECUCION(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_ERROR");
	serializar_t_error(fdCliente, tipoMensaje, estructura);
	return 0;
}


void* deserializar_ERROR_EJECUCION(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_error* estructura = deserializar_t_error(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_LEER_SWAP");
	return estructura;
}


void* serializar_ESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_ESCRIBIR_SWAP");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_ESCRIBIR_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_ESCRIBIR_SWAP");
	return estructura;
}

void* serializar_LEER_MEM(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_LEER_MEM");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_LEER_MEM(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_LEER_MEM");
	return estructura;
}

void* serializar_ESCRIBIR_MEM(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_ESCRIBIR_MEM");
	serializar_t_contenido_pagina(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_ESCRIBIR_MEM(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_contenido_pagina* estructura = deserializar_t_contenido_pagina(fdCliente, tipoMensaje);
	//puts("Deserializando deserializar_ESCRIBIR_MEM");
	return estructura;
}

void serializar_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	serializar_t_respuesta_ejecucion(fdCliente, tipoMensaje, estructura);
}

void* deserializar_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return deserializar_t_respuesta_ejecucion(fdCliente, tipoMensaje);
}

void serializar_TIEMPO_CPU_RESUL(int fdCliente, t_tipo_mensaje tipoMensaje, t_porcentajeCPUs*  estructura){
	serializar_int8_t(fdCliente, estructura->cantidadDeElementos);
	serializar_lista_porcentajes(fdCliente, estructura->respuestasPorcentaje );
}
void serializar_TIEMPO_CPU(int fdCliente, t_tipo_mensaje tipoMensaje,t_PID*  estructura){
	serializar_t_PID(fdCliente, tipoMensaje, estructura);

}
void* deserializar_TIEMPO_CPU(int fdCliente, t_tipo_mensaje tipoMensaje		){
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	return estructura;

}
void serializar_NOTIFICACION_HILO_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje,t_PID*  estructura){

}
void* deserializar_NOTIFICACION_HILO_ENTRADA_SALIDA(int fdCliente, t_tipo_mensaje tipoMensaje		){
	return NULL;
}
void* deserializar_TIEMPO_CPU_RESUL(int fdCliente, t_tipo_mensaje tipoMensaje		){
	t_porcentajeCPUs* estructura = malloc(sizeof(t_porcentajeCPUs));
	estructura->cantidadDeElementos = deserializar_int8_t(fdCliente);
	estructura->respuestasPorcentaje = deserializar_lista_porcentajes(fdCliente,estructura->cantidadDeElementos);
	return estructura ;
}


void* serializar_t_leerDeProcesoPorEscribir(int fdCliente, t_tipo_mensaje tipoMensaje, t_leerDeProcesoPorEscribir* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	serializar_int8_t(fdCliente, estructura->numeroPaginaFin);
	serializar_int8_t(fdCliente, estructura->numeroPaginaInicio);
	serializar_string(fdCliente, estructura->textoAEscribir);

	return 0;
}
t_leerDeProcesoPorEscribir* deserializar_t_leerDeProcesoPorEscribir(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerDeProcesoPorEscribir* estructura = malloc(sizeof(t_leerDeProcesoPorEscribir));
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	estructura->numeroPaginaFin = deserializar_int8_t(fdCliente);
	estructura->numeroPaginaInicio = deserializar_int8_t(fdCliente);
	estructura->textoAEscribir = deserializar_string(fdCliente);
	return estructura;
}

void* serializar_t_leerDeProceso(int fdCliente, t_tipo_mensaje tipoMensaje, t_leerDeProceso* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	serializar_int8_t(fdCliente, estructura->numeroPaginaFin);
	serializar_int8_t(fdCliente, estructura->numeroPaginaInicio);

	return 0;
}
t_leerDeProceso* deserializar_t_leerDeProceso(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_leerDeProceso* estructura = malloc(sizeof(t_leerDeProceso));
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	estructura->numeroPaginaFin = deserializar_int8_t(fdCliente);
	estructura->numeroPaginaInicio = deserializar_int8_t(fdCliente);

	return estructura;
}

void* serializar_t_error(int fdCliente, t_tipo_mensaje tipoMensaje, t_error* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	serializar_int8_t(fdCliente, estructura->pag);

	return 0;
}
t_error* deserializar_t_error(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_error* estructura = malloc(sizeof(t_error));
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	estructura->pag = deserializar_int8_t(fdCliente);

	return estructura;
}
void serializar_RESUL_EJECUCION_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	serializar_t_respuesta_ejecucion(fdCliente, tipoMensaje, estructura);
}
void* deserializar_RESUL_EJECUCION_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return deserializar_t_respuesta_ejecucion(fdCliente, tipoMensaje);
}

void serializar_RESUL_EJECUCION_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	serializar_t_respuesta_ejecucion(fdCliente, tipoMensaje, estructura);

}
void* deserializar_RESUL_EJECUCION_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	return deserializar_t_respuesta_ejecucion(fdCliente, tipoMensaje);
}

void* serializar_t_respuesta_ejecucion(int fdCliente,
		t_tipo_mensaje tipoMensaje, void* estructura) {
	t_respuesta_ejecucion* respuestaEjecucion =
			(t_respuesta_ejecucion*) estructura;
	serializar_t_pcb(fdCliente, tipoMensaje, respuestaEjecucion->pcb);
	serializar_string(fdCliente, respuestaEjecucion->resultadosInstrucciones);
	serializar_bool(fdCliente, respuestaEjecucion->finalizoOk);
	serializar_int8_t(fdCliente,respuestaEjecucion->cant_entrada_salida);
	return 0;
}

t_respuesta_ejecucion* deserializar_t_respuesta_ejecucion(int fdCliente,
		t_tipo_mensaje tipoMensaje) {
	t_respuesta_ejecucion* respuestaEjecucion = malloc(
			sizeof(t_respuesta_ejecucion));
	respuestaEjecucion->pcb = deserializar_t_pcb(fdCliente, tipoMensaje);
	respuestaEjecucion->resultadosInstrucciones = deserializar_string(fdCliente);
	respuestaEjecucion->finalizoOk = deserializar_bool(fdCliente);
	respuestaEjecucion->cant_entrada_salida = deserializar_int8_t(fdCliente);
	return respuestaEjecucion;
}


void serializar_t_tipo_mensaje(int fdCliente, t_tipo_mensaje tipoMensaje, t_tipo_mensaje estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(t_tipo_mensaje));
}
t_tipo_mensaje deserializar_t_tipo_mensaje(int fdCliente) {
	t_tipo_mensaje* tipoMensajeARecibir = malloc(sizeof(t_tipo_mensaje));
	recibirPorSocket(fdCliente, tipoMensajeARecibir, sizeof(t_tipo_mensaje));
	return *tipoMensajeARecibir;
}

void* serializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_TRAER_PAG_SWAP");
	//serializar_lecturaProc_desde_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
t_PID* deserializar_TRAER_PAG_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_TRAER_PAG_SWAP");
	return estructura;
}

void* serializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_FIN_OK");
	serializar_t_PID(fdCliente, tipoMensaje, estructura);
	return 0;
}

void* deserializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_PID(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_FIN_OK");
	return estructura;
}

void* serializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_INICIAR_PROC_NO_OK_CPU");
	serializar_t_rta_iniciar_no_ok_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
t_PID* deserializar_RESUL_INICIAR_PROC_NO_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_rta_iniciar_no_ok_CPU(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_INICIAR_PROC_NO_OK_CPU");
	return estructura;
}

void* serializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_RESUL_INICIAR_PROC_OK_CPU");
	serializar_t_rta_iniciar_ok_CPU(fdCliente, tipoMensaje, estructura);
	return 0;
}
t_PID* deserializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = deserializar_t_rta_iniciar_ok_CPU(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_RESUL_INICIAR_PROC_OK_CPU");
	return estructura;
}

void* serializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_PID* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);

	return 0;
}
t_PID* crearRespuestaIniciarOkCPU() {
	t_PID* contenido = malloc(sizeof(t_PID));
	if (contenido == NULL) {
		perror("[ERROR] No se reservo memoria para crearRespuestaIniciarOkCPU");
//		log_error(logger, "[ERROR] No se reservo memoria para crearRespuestaIniciarOkCPU");
		exit(-1);
	}
	return contenido;
}

t_PID* deserializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = crearRespuestaIniciarOkCPU();
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);

	return estructura;
}

void* serializar_t_rta_iniciar_no_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_PID* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);

	return 0;
}

t_PID* deserializar_t_rta_iniciar_no_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_PID* estructura = crearRespuestaIniciarOkCPU();
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);

	return estructura;
}

void* serializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje, t_pcb* estructura) {
	serializar_int8_t(fdCliente, estructura->pid);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->pid);
	serializar_string(fdCliente, estructura->rutaArchivoMcod);
	serializar_bool(fdCliente, estructura->tieneDesalojo);
	serializar_int16_t(fdCliente, estructura->tamanioRafaga);
	serializar_int16_t(fdCliente, estructura->proximaInstruccion);
	serializar_int16_t(fdCliente, estructura->instruccionFinal);
	serializar_bool(fdCliente, estructura->finalizar);
	serializar_time_t(fdCliente, estructura->tiempoInicial);
	serializar_time_t(fdCliente, estructura->tiempoInicioUltimaEjecucion);
	serializar_time_t(fdCliente, estructura->tiempoInicioUltimaEntradaSalida);
	serializar_time_t(fdCliente, estructura->tiempoFinal);
	serializar_int(fdCliente, estructura->tiempoEjecucion);
	serializar_int(fdCliente, estructura->tiempoEntradaSalida);

	return 0;
}
t_pcb* deserializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_pcb* estructura = malloc(sizeof(t_pcb));
	estructura->pid = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->pid);
	estructura->rutaArchivoMcod = deserializar_string(fdCliente);
	estructura->tieneDesalojo = deserializar_bool(fdCliente);
	estructura->tamanioRafaga = deserializar_int16_t(fdCliente);
	estructura->proximaInstruccion = deserializar_int16_t(fdCliente);
	estructura->instruccionFinal = deserializar_int16_t(fdCliente);

	estructura->finalizar = deserializar_bool(fdCliente);
	estructura->tiempoInicial = deserializar_time_t(fdCliente);
	estructura->tiempoInicioUltimaEjecucion = deserializar_time_t(fdCliente);
	estructura->tiempoInicioUltimaEntradaSalida = deserializar_time_t(fdCliente);
	estructura->tiempoFinal = deserializar_time_t(fdCliente);
	estructura->tiempoEjecucion = deserializar_int(fdCliente);
	estructura->tiempoEntradaSalida = deserializar_int(fdCliente);


	return estructura;
}

void* serializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando serializar_INICIAR_PROC_SWAP");
	serializar_t_iniciar_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_INICIAR_PROC_SWAP(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = deserializar_t_iniciar_swap(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_INICIAR_PROC_SWAP");
	return estructura;
}

void* serializar_INICIAR_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	//puts("Serializando INICIAR_PROCESO_MEM");
	serializar_t_iniciar_swap(fdCliente, tipoMensaje, estructura);
	return 0;
}
void* deserializar_INICIAR_PROCESO_MEM(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = deserializar_t_iniciar_swap(fdCliente, tipoMensaje);
	//puts("Deserializando serializar_INICIAR_PROCESO_MEM");
	return estructura;
}

void* serializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje, t_iniciar_swap* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);debug("%s Socket: %d, PID enviado: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	serializar_int16_t(fdCliente, estructura->cantidadPaginas);
	return 0;
}
t_iniciar_swap* deserializar_t_iniciar_swap(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	estructura->PID = deserializar_int8_t(fdCliente);debug("%s Socket %d, PID recibido: %i\n", getNombreTipoMensaje(tipoMensaje), fdCliente, estructura->PID);
	estructura->cantidadPaginas = deserializar_int16_t(fdCliente);
	return estructura;
}
void serializar_string(int fdCliente, char* estructura) {
	if (estructura != NULL) {
		int16_t tamanioString = strlen(estructura);
		serializar_int16_t(fdCliente, tamanioString);
		enviarSimple(fdCliente, estructura, tamanioString);
	} else {
		serializar_int16_t(fdCliente, 0);
	}
}
char* deserializar_string(int fdCliente) {
	int16_t tamanioString = deserializar_int16_t(fdCliente);
	char* string = malloc(tamanioString + 1);
	if (tamanioString > 0) {
		recibirPorSocket(fdCliente, string, tamanioString);
	}
	string[tamanioString] = '\0';
	return string;
}
void 	serializar_lista_porcentajes(int fdCliente, t_list* estructura){

	int a;
	for(a=0;a<list_size(estructura);a++){
		t_respuesta_porcentaje* respuesta = malloc(sizeof(t_respuesta_porcentaje));
		respuesta = list_get(estructura,a);
		serializar_int8_t( fdCliente, respuesta->res_porcentaje);
//		serializar_int8_t(fdCliente, respuesta->idCpu);
		serializar_pthread(fdCliente, respuesta->idCpu);
	}

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

void serializar_int(int fdCliente, int estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(int));
}
int8_t deserializar_int(int fdCliente) {
	int* res = malloc(sizeof(int));
	recibirPorSocket(fdCliente, res, sizeof(int));
	return *res;
}

void serializar_time_t(int fdCliente, time_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(time_t));
}
time_t deserializar_time_t(int fdCliente) {
	time_t* res = malloc(sizeof(time_t));
	recibirPorSocket(fdCliente, res, sizeof(time_t));
	return *res;
}
t_list* deserializar_lista_porcentajes(int fdCliente,int8_t cantidad){
	int a;
	t_list* lista = list_create();


	for(a= 0 ; a<cantidad;a++){
		t_respuesta_porcentaje* res = malloc(sizeof(t_respuesta_porcentaje));
		res->res_porcentaje = deserializar_int8_t( fdCliente);
//		res->idCpu = deserializar_int8_t( fdCliente);
		res->idCpu =  deserializar_pthread(fdCliente);
		list_add(lista,res);
	}

	return lista;
}

void serializar_bool(int fdCliente, bool estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(bool));
}
bool deserializar_bool(int fdCliente) {
	bool* res = malloc(sizeof(bool));
	recibirPorSocket(fdCliente, res, sizeof(bool));
	return *res;
}

void serializar_pthread(int fdCliente, pthread_t pthread){
	enviarSimple(fdCliente, &pthread, sizeof(pthread_t));
}

pthread_t deserializar_pthread(int fdCliente){
	pthread_t* pthread = malloc(sizeof(pthread_t));
	recibirPorSocket(fdCliente,pthread, sizeof(pthread_t));
	return *pthread;
}

void serializar_pid_t(int fdCliente, pid_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(pid_t));
}
pid_t deserializar_pid_t(int fdCliente) {
	pid_t* res = malloc(sizeof(pid_t));
	recibirPorSocket(fdCliente, res, sizeof(pid_t));
	return *res;
}

