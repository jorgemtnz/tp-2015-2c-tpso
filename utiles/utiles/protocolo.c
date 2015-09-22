#include "protocolo.h"

//+++++++++++++++++++++++++funciones para el manejo del protocolo++++++++++++++++++++++++++++++++++

//objetivo devolver el paquete para el envio por socket

PaqueteEnvio* serializar(int tipoDeMensaje, void* payLoad, int tamanioPayLoad){
	PaqueteEnvio* paqueteAEnviar = malloc(sizeof(PaqueteEnvio));
	size_t tamanioDelMensaje = tamanioPayLoad + sizeof(int); //+4 por el total del mensaje, le sumo un sizeof(int) por el header

	paqueteAEnviar->mensaje = malloc(tamanioDelMensaje);
	memcpy(paqueteAEnviar->mensaje,&tipoDeMensaje,sizeof(int));
	size_t desplazameinto = sizeof(int);
	memcpy(paqueteAEnviar->mensaje + desplazameinto,payLoad,tamanioPayLoad);

	paqueteAEnviar->tamanioMensaje = tamanioDelMensaje;
	return paqueteAEnviar;
}

// devolver el paquete para recepcion( y luego interpretar el paquete, referido al switch con los tokens)
Paquete* deserializar(void* buffer, int tamnioMensaje) {
	Paquete* paqueteDeserial = malloc(sizeof(Paquete));
	memcpy(&(paqueteDeserial->tipoDeMensaje),buffer,sizeof(int));
	int dezplazamiento = sizeof(int);
	size_t tamanioALeerDelBuffer = tamnioMensaje - dezplazamiento;
	paqueteDeserial->payLoad = malloc(tamanioALeerDelBuffer);
	memcpy(paqueteDeserial->payLoad, buffer + dezplazamiento,tamanioALeerDelBuffer);
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

void* serializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje, t_pcb* estructura) {
	serializar_string(fdCliente, estructura->rutaArchivoMcod);
	return 0;
}
t_pcb* deserializar_t_pcb(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_pcb* estructura = malloc(sizeof(t_pcb));
	char* string = deserializar_string(fdCliente);
	estructura->rutaArchivoMcod = string;
	return estructura;
}

void* serializar_string(int fdCliente, char* estructura) {
	int16_t tamanioString = strlen(estructura);
	serializar_int16_t(fdCliente, tamanioString);
	enviarSimple(fdCliente, estructura, tamanioString);
}
char* deserializar_string(int fdCliente) {
	int16_t tamanioString = deserializar_int16_t(fdCliente);
	char* string = malloc(tamanioString);
	recibirPorSocket(fdCliente, string, tamanioString);
	return string;
}

void* serializar_int16_t(int fdCliente, int16_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(int16_t));
}
int16_t deserializar_int16_t(int fdCliente) {
	int16_t* res = malloc(sizeof(int16_t));
	recibirPorSocket(fdCliente, res, sizeof(int16_t));
	return *res;
}
