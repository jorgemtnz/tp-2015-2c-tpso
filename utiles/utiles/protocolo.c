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

