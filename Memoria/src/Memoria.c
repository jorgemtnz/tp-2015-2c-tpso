#include "Memoria.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();
	listaMemoria = list_create();
	listaTLB = list_create();
    listaTablaDePag = list_create();
	contadorPagTP = 0;

	logger = log_create("LOG_Memoria.log", "Memoria", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	int socketSwap;
	conectar(configuracion->ipSwap, string_itoa(configuracion->puertoSwap), &socketSwap);
	dictionary_put(conexiones, "Swap", string_itoa(socketSwap));
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, socketSwap, procesarMensajes, NULL, logger);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	pthread_mutex_lock(&mutexProcesadorMensajes);
	puts("Memoria procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, nuevaConexion, extra, logger);
	if (nuevaConexion) {
		dictionary_put(conexiones, "CPU", string_itoa(socket));
	} else {
		printf("Recibi el mensaje: %s\n", buffer);
		if (string_starts_with(buffer, "correr programa")) {
			char* socketCPU = (char*) dictionary_get(conexiones, "Swap");
			/* EJEMPLO
			 puts("Enviando \"correr programa\" al Swap");
			 enviar(atoi(socketCPU), "correr programa", strlen("correr programa"));
			 puts("Enviado al Swap");
			 */
			int buff, idProc, cantPag,nroPag,pagIn,pagFin; // seria el buffer ques e deba poner
			char* textoAEscribir;
			switch (buff) {
			case 1: // va a ser iniciar este
				pthread_mutex_lock(&mutexParaInicializado);
				iniciar(idProc, cantPag, socketCPU);
				break;
			case 2: // va a ser escribir este
				escribir(idProc, nroPag, textoAEscribir);
				break;
			case 3: // va a ser leer este
				leer(idProc, pagIn, pagFin);
				break;
			case 4: // va a ser finalizar este
				finalizar(idProc);
				break;
			}
		}


	}
	return 0;
	pthread_mutex_unlock(&mutexProcesadorMensajes);
}

