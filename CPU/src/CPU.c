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
	t_cpu* cpu;
if(procCPU->contadorIdCPU != 0){
	cpu = list_get(procCPU->listaCPU, procCPU->contadorIdCPU - 1);
}else{
	cpu = list_get(procCPU->listaCPU, 0);
}
	int resultConexion_planif = 0;
	int resultConexion_Memoria = 0;
	puts("CPU procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra,
			logger);
	log_info(logger, "se va a procesar un mensaje");
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

		//como espero varios resultados, entonces los deje en una sola fucnion+++++++++++++++++++++++++++++++++++++++++
		// devuelve la estructura que se envia, la que se crea por la ejecucion de los comandos ++++++++++++++++++++++++++++++++++++++
		// se encuentra en enviaryrecibir.c  esta funcion++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
