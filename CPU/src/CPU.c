#include "CPU.h"

int main(int argc, char *argv[]) {

	procCPU = crearProcCPU();
	logger = log_create("LOG_CPU.log", "CPU", false, LOG_LEVEL_INFO); //Inicializacion logger

	if (hayQueEjecutarTests(argc, argv)) {
		return ejecutarTests();
	}
	leerArchivoDeConfiguracion(argc, argv);

	levantarHilosCPU();

	destProcCPU(procCPU);
	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer,
		t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a procesar un mensaje");
	pthread_mutex_unlock(&mutexCPULogs);
	t_cpu* cpu;
	int tamanio = 0;
	tamanio = list_size(procCPU->listaCPU);
	if (tamanio != 0) {
//		pthread_t hiloactual = queHiloSoy();

		bool buscaHilo(t_cpu* unaCPU) {
			//return hiloactual == unaCPU->idCPU;
			return unaCPU->socketMemoria == socket
					|| unaCPU->socketPlanificador == socket;
		}

		cpu = list_find(procCPU->listaCPU, (void*) buscaHilo);
		if (cpu != NULL) {
			debug(
					"CPU procesar mensajes CPU: %s, pid: i, actualPid %d, socketMemoria %d\n",
					cpu->nombre, /*cpu->pcbPlanificador!= NULL?cpu->pcbPlanificador->pid:-100,*/
					cpu->actualPID, cpu->socketMemoria);
		}
	} else {
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger, "no hay cpu conectadas");
		log_info(logger, "se va a procesar un mensaje");
		pthread_mutex_unlock(&mutexCPULogs);
		puts("no hay cpu conectadas \n");
	}

	int resultConexion_planif = 0;
	int resultConexion_Memoria = 0;

	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra,
			logger);

	if (tipoNotificacion == NEW_CONNECTION) {
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_info(logger, "se realizo nueva conección");
		pthread_mutex_unlock(&mutexCPULogs);
	} else if (tipoNotificacion == TERMINAL_MESSAGE) {
		//comando auxiliar para reconectar al planificador manualmente
		//escribir cp en la terminal y enter (cp= conectar planificador)
		if (string_starts_with(buffer, "cp")) {
			int socketPlanificador;
			resultConexion_planif = conectar(configuracion->vg_ipPlanificador,
					string_itoa(configuracion->vg_puertoPlanificador),
					&socketPlanificador);
			if (resultConexion_planif == -1)
				pthread_mutex_lock(&mutexCPULogs);
			log_info(logger, identificaCPU(queHiloSoy()));
			log_error(logger, "[ERROR]no se reconecto el CPU al Planificador");
			pthread_mutex_unlock(&mutexCPULogs);
			//dictionary_put(conexiones, "Planificador",
			//		string_itoa(socketPlanificador));
			cpu->socketPlanificador = socketPlanificador;
		}

		if (string_starts_with(buffer, "cm")) {
			int socketMemoria;
			resultConexion_Memoria = conectar(configuracion->vg_ipMemoria,
					string_itoa(configuracion->vg_puertoMemoria),
					&socketMemoria);
			if (resultConexion_Memoria == -1)
				pthread_mutex_lock(&mutexCPULogs);
			log_info(logger, identificaCPU(queHiloSoy()));
			log_error(logger, "[ERROR]no se reconecto el CPU a la Memoria");
			pthread_mutex_unlock(&mutexCPULogs);
			//dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));
			cpu->socketMemoria = socketMemoria;
		}

	} else if (tipoNotificacion == MESSAGE) {
//printf("llega a recibir mensajes varios\n");
		recibirMensajeVarios(header, buffer, extra, cpu);

	} else if (tipoNotificacion == HANG_UP) {
		pthread_mutex_lock(&mutexCPULogs);
		log_info(logger, identificaCPU(queHiloSoy()));
		log_error(logger, "[ERROR] se desconecto un proceso");
		pthread_mutex_unlock(&mutexCPULogs);
	}

	return 0;
}

char* decirHolaMundo() {
	return "Hola Mundo";
}

char* getNombre() {

	t_cpu* cpu;
	pthread_t hiloactual = queHiloSoy();

	bool buscaHilo(t_cpu* unaCPU) {
		return hiloactual == unaCPU->idCPU;
	}

	cpu = list_find(procCPU->listaCPU, (void*) buscaHilo);

	if (cpu != NULL) {
		return cpu->nombre;
	}
	return "CPU-main";
}
