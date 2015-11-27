/*
 * funciones_plan.c
 *
 *  Created on: 1/9/2015
 *      Author: utnso
 */

#include "Planificador.h"
#include <stdarg.h>

void leerArchivoDeConfiguracion(int argc, char *argv[]) {

	char* logMsg = NULL;

	if (argc < 2) {
		logMsg =
				string_from_format(
						"Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./Planificador /home/utnso/tp-2015-2c-tpso/Planificador/config_plan.cfg\n");
		putsConsola(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	}

	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
	int result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		logMsg = string_from_format("Archivo de configuracion no encontrado. Parametro especificado: %s\n", nombreArchivoConfig);
		putsConsola(logMsg);
		log_error(logger, logMsg);
		exit(-1);
	} else {
		t_config* archivoConfig = config_create(nombreArchivoConfig);

		configuracion = (t_configuracion*) malloc(sizeof(t_configuracion));
		configuracion->puertoEscucha = strdup(config_get_string_value(archivoConfig, "PUERTO_ESCUCHA"));
		configuracion->algorimoPlanificacion = strdup(config_get_string_value(archivoConfig, "ALGORITMO_PLANIFICACION"));
		configuracion->quantum = strdup(config_get_string_value(archivoConfig, "QUANTUM"));
		crearPlanificacion(configuracion->algorimoPlanificacion, configuracion->quantum);

		logMsg = string_from_format("Archivo de configuracion leido correctamente\n");
		putsConsola(logMsg);
		log_error(logger, logMsg);

		config_destroy(archivoConfig);
	}
}

void validarYCrearFIFO(char* nombreAlgoritmo) {
	planificacion->nombreAlgoritmo = nombreAlgoritmo;
	planificacion->tamanioRafaga = -1;
	planificacion->tieneDesalojo = false;
}

void validarYCrearRoundRobin(char* nombreAlgoritmo, char* quantum) {
	if (string_is_empty(quantum)) {
		putsConsola("No se especifico el parametro QUANTUM en el archivo de configuracion. Debe ser un numero entero mayor que cero\n");
		exit(-1);
	} else {
		uint16_t tamanioRafaga = atoi(quantum);
		if (tamanioRafaga < 1) {
			putsConsola("No se especifico el parametro QUANTUM valido en el archivo de configuracion. Debe ser un numero entero mayor que cero\n");
			exit(-1);
		} else {
			planificacion->nombreAlgoritmo = nombreAlgoritmo;
			planificacion->tamanioRafaga = tamanioRafaga;
			planificacion->tieneDesalojo = true;
		}
	}
}

void crearPlanificacion(char* nombreAlgoritmo, char* quantum) {
	planificacion = malloc(sizeof(t_planificacion));

	if (string_equals(nombreAlgoritmo, "FIFO")) {
		validarYCrearFIFO(nombreAlgoritmo);
	} else if (string_equals(nombreAlgoritmo, "RR")) {
		validarYCrearRoundRobin(nombreAlgoritmo, quantum);
	} else {
		printConsola("No se reconoce nombre de algoritmo especificado: '%s'.\nLos algoritmos validos son FIFO y Round Robin\n");
		exit(-1);
	}

	//creamos la cola de listos
	colaDeListos = list_create();
	colaDeFinalizados = list_create();
	colaDeNuevos = list_create();
	colaDeEntradaSalida = list_create();
}

void registrarNuevaCPU(int socket, char* nombre) {
	t_cpu_ref* cpu = malloc(sizeof(t_cpu_ref));
	cpu->socket = socket;
	cpu->nombre = nombre;
	cpu->conectada = true;
	cpu->ejecutando = false;
	cpu->pcb = NULL;
	list_add(listaCPUs, cpu);
}

void desregistrarCPUConectada(int socket) {
	t_cpu_ref* cpu = NULL;

	int var;

	int listaCPUsCount = list_size(listaCPUs);
	for (var = 0; var < listaCPUsCount && cpu == NULL; var++) {
		t_cpu_ref* cpu_actual = list_get(listaCPUs, var);
		if (cpu_actual->socket == socket) {
			cpu = cpu_actual;
		}
	}

	if (cpu != NULL) {
		cpu->conectada = false;
	} else {
		printConsola("No se pudo registrar la desconexion de la CPU conectada por el socket: %d\n", socket);
	}
}

bool cpuConSocket(void *cpu, int socket) {
	return ((t_cpu_ref *) cpu)->socket == socket;
}

char* crearNombreCPU() {
	int nroCPU = list_size(listaCPUs) + 1;
	return string_from_format("CPU-%d", nroCPU);
}

int putsConsola(const char *msg) {
//	puts("putsConsola\n");
	log_info(logger, msg);
	return puts(msg);
}
int printConsola(const char *formato, ...) {
//	puts("printConsola\n");
	va_list arguments;
	va_start(arguments, formato);
	int res = vprintf(formato, arguments);
	va_end(arguments);

	char* nuevo = string_from_vformat(formato, arguments);
	log_info(logger, nuevo);
	return res;
}

void procesarMensajesSegunTipo(int socket, t_header* header, char* buffer) {
	switch (header->tipoMensaje) {
	case (HANDSHAKE_CPU): {
		registrarNuevaCPU(socket, buffer);
		break;
	}
	case (HANDSHAKE_ENTRADA_SALIDA): {
		break;
	}
	case (RESUL_EJECUCION_OK): {
		procesar_RESUL_EJECUCION_OK(socket, header, (t_respuesta_ejecucion*) buffer);
		break;
	}
	case (ENTRADA_SALIDA): {
		procesar_ENTRADA_SALIDA(socket, header, (t_respuesta_ejecucion*) buffer);
		break;
	}
	case (RESUL_INICIAR_PROC_NO_OK_CPU): {
		printf("proceso no iniciado en CPU\n");
		break;
	}
	case (TIEMPO_CPU_RESUL): {
		t_porcentajeCPUs* listaRecibida = (t_porcentajeCPUs*) buffer;

		int f;
		t_respuesta_porcentaje* porcentaje = malloc(sizeof(t_respuesta_porcentaje));
		for (f = 0; f < listaRecibida->cantidadDeElementos; f++) {
			porcentaje = list_get(listaRecibida->respuestasPorcentaje, f);
			printf("cpu %lu: %i porciento \n", porcentaje->idCpu, porcentaje->res_porcentaje);
		}
		break;
	}
	case (NOTIFICACION_HILO_ENTRADA_SALIDA): {
		ejecutarPlanificadorLargoPlazo();
		break;
	}
	default: {
		printConsola("No se reconoce el mensaje de tipo %s\n", getNombreTipoMensaje(header->tipoMensaje));
	}
	}
}

void procesar_RESUL_EJECUCION_OK(int socket, t_header* header, t_respuesta_ejecucion* respuestaEjecucion) {
	putsConsola("Resultado de la ejecucion:\n");
	printConsola("Socket: %d, PID: %d\n", socket, respuestaEjecucion->pcb->pid);
	//ESTO ES PARA EL COMANDO FINALIZAR
	if (respuestaEjecucion->pcb->finalizar == true) {
		respuestaEjecucion->pcb->proximaInstruccion = respuestaEjecucion->pcb->instruccionFinal;
	}

	printConsola("Finalizo OK: %s\n", respuestaEjecucion->finalizoOk ? "Si" : "No");

	imprimirRespuestasDeInstrucciones(respuestaEjecucion);

	if (respuestaEjecucion->finalizoOk) {
		ejecucionAFinalizado(respuestaEjecucion->pcb);
	} else {
		ejecucionAColaDeListos(respuestaEjecucion->pcb);
	}
	ejecutarPlanificadorLargoPlazo();

}

void imprimirRespuestasDeInstrucciones(t_respuesta_ejecucion* respuestaEjecucion) {
	char** respuestaDeCadaInstruccion = string_split(respuestaEjecucion->resultadosInstrucciones, "\0");
	int a = 0;
	int b = 0;
	while (respuestaDeCadaInstruccion[a] != NULL) {
		char** respuestaDeUnaInstruccion = string_split(respuestaDeCadaInstruccion[a], ";");
		while (respuestaDeUnaInstruccion[b] != NULL) {
			printConsola("%s\n", respuestaDeUnaInstruccion[b]);
			b++;
		}
		a++;
	}
}

void procesar_ENTRADA_SALIDA(int socket, t_header* header, t_respuesta_ejecucion* respuestaEjecucion) {
	//por ahora para debugguear visual, pero se debe poner en cola bloqueados y hacerlo esperar
	putsConsola("Resultado de la ejecucion:\n");
	printConsola("PID: %d\n", respuestaEjecucion->pcb->pid);

//ESTO ES PARA EL COMANDO FINALIZAR
	if (respuestaEjecucion->pcb->finalizar == true) {
		respuestaEjecucion->pcb->proximaInstruccion = respuestaEjecucion->pcb->instruccionFinal;
	}

	imprimirRespuestasDeInstrucciones(respuestaEjecucion);

	t_cpu_ref* cpu = obtenerCPUEjecutandoPcb(respuestaEjecucion->pcb);

	if (cpu != NULL) {
		quitarProcesoDeCpu(cpu);
	} else {
		printConsola("Hay una inconsistencia, recibi una entrada salida y el pcb pid: %d, no se encuentra en ninguna CPU\n", respuestaEjecucion->pcb->pid);
	}

	t_pcb_entrada_salida* pcbEntradaSalida = malloc(sizeof(t_pcb_entrada_salida));
	pcbEntradaSalida->pcb = respuestaEjecucion->pcb;
	pcbEntradaSalida->cantidadCiclos = respuestaEjecucion->cant_entrada_salida;
	list_add(getColaDeEntradaSalida(), pcbEntradaSalida);

	printConsola("tiempo espera %d\n", respuestaEjecucion->cant_entrada_salida);

	ejecutarPlanificadorLargoPlazo();

}

bool existePID(uint8_t pid) {
	int a;
	t_cpu_ref * cpu = crearCpuRef();
	char* ruta = string_new();

	t_pcb* pcb = crearPcb(ruta);
	t_pcb_entrada_salida* pcbES = malloc(sizeof(t_pcb_entrada_salida));

	for (a = 0; a < list_size(listaCPUs); a++) {

		cpu = list_get(listaCPUs, a);

		if (cpu->pcb != NULL) {
			if (cpu->pcb->pid == pid) {
				return true;
			}
		}

		t_pcb* pcbEnEntradaSalida = NULL;
		pthread_mutex_lock(&mutexEstadoEntradaSalida);

		pcbEnEntradaSalida = estadoEntradaSalida.pcb;

		pthread_mutex_unlock(&mutexEstadoEntradaSalida);

		if (pcbEnEntradaSalida->pid == pid) {

			return true;
		}

	}

	for (a = 0; a < list_size(colaDeEntradaSalida); a++) {

		pcbES = list_get(colaDeEntradaSalida, a);

		if (pcbES->pcb->pid == pid) {
			return true;
		}
	}
	for (a = 0; a < list_size(colaDeListos); a++) {
		pcb = list_get(colaDeListos, a);

		if (pcb->pid == pid) {
			return true;
		}
	}

	return false;

}
