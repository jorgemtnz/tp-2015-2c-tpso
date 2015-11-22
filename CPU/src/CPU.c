#include "CPU.h"

int main(int argc, char *argv[]) {

	procCPU = crearProcCPU();
	logger = log_create("LOG_CPU.log", "CPU", false, LOG_LEVEL_INFO); //Inicializacion logger

	if (hayQueEjecutarTests(argc, argv)) {
		return ejecutarTests();
	}
	leerArchivoDeConfiguracion(argc, argv);

	levantarHilosCPU();

	log_info(logger, "se destruye proceso CPU");
	destProcCPU(procCPU);
	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer,
		t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {

	log_info(logger, "se va a procesar un mensaje");
	t_cpu* cpu;
	int tamanio = 0;
	tamanio = list_size(procCPU->listaCPU);
	if (tamanio != 0) {
		pthread_t hiloactual = queHiloSoy();

		bool buscaHilo(t_cpu* unaCPU) {
			return hiloactual == unaCPU->idCPU;
		}

		cpu = list_find(procCPU->listaCPU, (void*) buscaHilo);
	} else {
		printf("no hay cpu conectadas \n");
	}

	int resultConexion_planif = 0;
	int resultConexion_Memoria = 0;

	puts("CPU procesar mensajes");
	printf("%s\n", queCPUsoy(cpu));
	   printf("la id del hilo es %lu \n", cpu->idCPU);

	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra,
			logger);

	if (tipoNotificacion == NEW_CONNECTION) {
		log_info(logger, "se realizo nueva conección");
	} else if (tipoNotificacion == TERMINAL_MESSAGE) {
		//comando auxiliar para reconectar al planificador manualmente
		//escribir cp en la terminal y enter (cp= conectar planificador)
		if (string_starts_with(buffer, "cp")) {
			int socketPlanificador;
			resultConexion_planif = conectar(configuracion->vg_ipPlanificador,
					string_itoa(configuracion->vg_puertoPlanificador),
					&socketPlanificador);
			if (resultConexion_planif == -1)
				log_error(logger,
						"[ERROR]no se reconecto el CPU al Planificador");
			dictionary_put(conexiones, "Planificador",
					string_itoa(socketPlanificador));
		}

		if (string_starts_with(buffer, "cm")) {
			int socketMemoria;
			resultConexion_Memoria = conectar(configuracion->vg_ipMemoria,
					string_itoa(configuracion->vg_puertoMemoria),
					&socketMemoria);
			if (resultConexion_Memoria == -1)
				log_error(logger, "[ERROR]no se reconecto el CPU a la Memoria");
			dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));
		}

	} else if (tipoNotificacion == MESSAGE) {

		recibirMensajeVarios(header, buffer, extra, cpu);

	} else if (tipoNotificacion == HANG_UP) {
		log_error(logger, "[ERROR] se desconecto un proceso");
	}

	return 0;
}

char* decirHolaMundo() {
	return "Hola Mundo";
}

char* getNombre() {
	return "CPU"; //ver si conviene responder un nombre distinto por cada hilo de CPU
}
