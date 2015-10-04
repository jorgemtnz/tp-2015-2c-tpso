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
void* serializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_OK");
		serializar_t_respuesta_iniciar(fdCliente, tipoMensaje, estructura);
		return 0;
}
void* deserializar_RESUL_INICIAR_PROC_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_iniciar* estructura = deserializar_t_respuesta_iniciar(fdCliente, tipoMensaje);
		puts("Deserializando serializar_RESUL_INICIAR_PROC_OK");
		return estructura;
}

void* serializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_OK");
		serializar_t_respuesta_iniciar(fdCliente, tipoMensaje, estructura);
		return 0;
}
void* deserializar_RESUL_INICIAR_PROC_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_iniciar* estructura = deserializar_t_respuesta_iniciar(fdCliente, tipoMensaje);
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
void* serializar_t_respuesta_leer(int fdCliente, t_tipo_mensaje tipoMensaje,t_respuesta_leer* estructura) {
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











void* serializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_FIN_OK");
		serializar_t_respuesta_finalizar(fdCliente, tipoMensaje, estructura);
		return 0;
}
void* serializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_FIN_ERROR");
		serializar_t_respuesta_finalizar(fdCliente, tipoMensaje, estructura);
		return 0;
}
void* deserializar_RESUL_FIN_ERROR(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_finalizar* estructura = deserializar_t_respuesta_finalizar(fdCliente, tipoMensaje);
		puts("Deserializando serializar_RESUL_FIN_ERROR");
		return estructura;
}

void* deserializar_RESUL_FIN_OK(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_finalizar* estructura = deserializar_t_respuesta_finalizar(fdCliente, tipoMensaje);
		puts("Deserializando serializar_RESUL_FIN_OK");
		return estructura;
}
void* serializar_t_respuesta_finalizar(int fdCliente, t_tipo_mensaje tipoMensaje,t_respuesta_finalizar* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);


	return 0;
}
t_respuesta_finalizar* deserializar_t_respuesta_finalizar(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_finalizar* estructura = malloc(sizeof(t_respuesta_finalizar));
	estructura->PID = deserializar_int8_t(fdCliente);


	return estructura;
}

















void* serializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, void* estructura) {
	puts("Serializando serializar_RESUL_INICIAR_PROC_OK_CPU");
		serializar_t_rta_iniciar_ok_CPU(fdCliente, tipoMensaje, estructura);
		return 0;
}
void* deserializar_RESUL_INICIAR_PROC_OK_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_escribir* estructura = deserializar_t_rta_iniciar_ok_CPU(fdCliente, tipoMensaje);
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

void* serializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_iniciar* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);

	return 0;
}

t_respuesta_iniciar* deserializar_t_rta_iniciar_ok_CPU(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_rta_iniciar_ok_CPU* estructura = crearRespuestaIniciarOkCPU();
	estructura->PID = deserializar_int8_t(fdCliente);

	return estructura;
}


void* serializar_t_respuesta_iniciar(int fdCliente, t_tipo_mensaje tipoMensaje, t_respuesta_iniciar* estructura) {
	serializar_int8_t(fdCliente, estructura->PID);

	return 0;
}

t_respuesta_iniciar* deserializar_t_respuesta_iniciar(int fdCliente, t_tipo_mensaje tipoMensaje) {
	t_respuesta_iniciar* estructura = malloc(sizeof(t_respuesta_iniciar));
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
void* serializar_string(int fdCliente, char* estructura) {
	int16_t tamanioString = strlen(estructura);
	int tamanioAlter = strlen(estructura);
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

void* serializar_int16_t(int fdCliente, int16_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(int16_t));
}
int16_t deserializar_int16_t(int fdCliente) {
	int16_t* res = malloc(sizeof(int16_t));
	recibirPorSocket(fdCliente, res, sizeof(int16_t));
	return *res;
}

void* serializar_int8_t(int fdCliente, int8_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(int8_t));
}
int8_t deserializar_int8_t(int fdCliente) {
	int8_t* res = malloc(sizeof(int8_t));
	recibirPorSocket(fdCliente, res, sizeof(int8_t));
	return *res;
}

void* serializar_bool(int fdCliente, bool estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(bool));
}
bool deserializar_bool(int fdCliente) {
	bool* res = malloc(sizeof(bool));
	recibirPorSocket(fdCliente, res, sizeof(bool));
	return *res;
}

void* serializar_pid_t(int fdCliente, pid_t estructura) {
	enviarSimple(fdCliente, &estructura, sizeof(pid_t));
}
pid_t deserializar_pid_t(int fdCliente) {
	pid_t* res = malloc(sizeof(pid_t));
	recibirPorSocket(fdCliente, res, sizeof(pid_t));
	return *res;
}
