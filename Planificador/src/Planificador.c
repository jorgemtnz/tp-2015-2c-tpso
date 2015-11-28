#include "Planificador.h"
#include <time.h>

int main(int argc, char *argv[]) {
	//conexiones = dictionary_create();

	crearLogger();
	putsConsola("Iniciando programa");
	listaCPUs = list_create();

	leerArchivoDeConfiguracion(argc, argv);
	if (hayQueEjecutarTests(argc, argv)) {
		return ejecutarTests();
	}

	lockEstadoEntradaSalida();
	estadoEntradaSalida.cantidadCiclos = 5;
	estadoEntradaSalida.finalizoEntradaSalida = false;
	unlockEstadoEntradaSalida();
	lockHayEntradaSalidaParaEjecutar();
	pthread_t idHilo;
	pthread_create(&idHilo, NULL, ejecutarEntradaSalida, &estadoEntradaSalida);

	escucharConexiones(configuracion->puertoEscucha, 0, 0, 0, procesarMensajes, NULL, logger);
	levantarConsola();

	putsConsola("Programa finalizado");
	return EXIT_SUCCESS;
}

void crearLogger() {
	logger = log_create("LOG_PLAN.log", "Planificador", false, LOG_LEVEL_INFO);
}


int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger) {
	//putsConsola("Planificador procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, tipoNotificacion, extra, logger);
	if(tipoNotificacion == NEW_CONNECTION) {
		//dictionary_put(conexiones, "CPU", string_itoa(socket));
		//falta reconocer si ya estuvo conectada y es una reconexion
		//registrarNuevaCPU(socket);

	} else if(tipoNotificacion == TERMINAL_MESSAGE) {
		procesarMensajesConsola(socket, header, buffer);
	} else if(tipoNotificacion == MESSAGE) {
//		printConsola("Recibi un mensaje de tipo %s\n", getNombreTipoMensaje(header->tipoMensaje));
		procesarMensajesSegunTipo(socket, header, buffer);

	} else if(tipoNotificacion == HANG_UP) {
		desregistrarCPUConectada(socket);
	}
	return 0;
}

bool conectadoAPadre = false;
int socketPadre;
bool finalizarProcesoEntradaSalida = false;

void *ejecutarEntradaSalida(void *param) {

	while (!finalizarProcesoEntradaSalida) {
		int cantidadCiclos;
		t_pcb* pcb;
		lockHayEntradaSalidaParaEjecutar();

		if(!conectadoAPadre) {
			conectar(string_duplicate("127.0.0.1"), configuracion->puertoEscucha,
						&socketPadre);
		}

		lockEstadoEntradaSalida();
		cantidadCiclos = estadoEntradaSalida.cantidadCiclos;
		pcb = estadoEntradaSalida.pcb;
		printConsola("Proceso pid: %d empieza su E/S\n", pcb->pid);
		unlockEstadoEntradaSalida();
		while(cantidadCiclos > 0) {
			//time_t t;
			//time(&t);
			//printf("\n current time is : %s ",ctime(&t));
			printConsola("Cantidad ciclos restantes %d\n", cantidadCiclos);
			usleep(1000000);
			lockEstadoEntradaSalida();
			estadoEntradaSalida.cantidadCiclos--;
			cantidadCiclos = estadoEntradaSalida.cantidadCiclos;
			unlockEstadoEntradaSalida();
		}

		lockEstadoEntradaSalida();
		estadoEntradaSalida.finalizoEntradaSalida = true;
		unlockEstadoEntradaSalida();

		printConsola("Proceso pid: %d finaliza su E/S\n", pcb->pid);
		enviarStruct(socketPadre, NOTIFICACION_HILO_ENTRADA_SALIDA, NULL);
	}

	putsConsola("Fin del hilo de entrada salida");
	return NULL;
}

char* decirHolaMundo() {
	return "Hola Mundo";
}

char* getNombre() {
	return "Planificador";
}
