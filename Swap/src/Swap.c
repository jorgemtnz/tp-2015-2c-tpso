#include "Swap.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();

	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	crearArchivo();

	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);

//	recv(socketMemoria, &prueba, sizeof(int), 0);
//	printf("%i\n", prueba);
//	prueba = 5;
//	send(socketMemoria, &prueba, sizeof(int), 0);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("Swap procesar mensajes");
	defaultProcesarMensajes(socket, buffer, nuevaConexion, extra, logger);
	t_list* listaDeProcesosCargados;
	t_list* listaDeEspaciosLibres;
	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();

	pid_t pid;

	t_escribirEnProceso* procesoAEscribir;
	t_leerDeProceso* procesoRecibido;
	procesoAEscribir = crearEscribirEnProceso();
	int cantidadPaginas;
	int buff; // PONGO EL BUFFER COMO INT PQ NO ME ANDA CON STRING
	switch (buff) {
	case 1: // "iniciar":
		//TENGO QUE RECIBIR DE MEMORIA EL PID Y LA CANTIDADPAGINAS

		iniciar(cantidadPaginas, listaDeEspaciosLibres, listaDeProcesosCargados, pid);
		break;
	case 2: // "escribir":
		//TENGO QUE RECIBIR DE MEMORIA EL PID  LA pagina a escribir y el contenido
		//RECIBIR PAQUETE DE MEMORIA
		//guardar en procesoAEscribir
		escribir(listaDeProcesosCargados, procesoAEscribir);
		break;
	case 3: //"leer":
		//TENGO QUE RECIBIR DE MEMORIA EL PID, NUMEROPAGINICIO , NUMEROPAGFIN(leer de proceso es el struct)
		leer(procesoRecibido, listaDeProcesosCargados);
		break;
	case 4: // "finalizar":
		//tengo que recibir de memoria el pid
		finalizar(pid, listaDeProcesosCargados, listaDeEspaciosLibres);

		break;
	}
	return 0;
}
