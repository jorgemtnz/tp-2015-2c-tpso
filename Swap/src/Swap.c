#include "Swap.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();
	int aux;

	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	crearArchivo();
	t_list* listaDeProcesosCargados;
	t_list* listaDeEspaciosLibres;
	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();
	l_procesosCargados* procesoAInsertarEnLista;
	l_espacioLibre* espacioLibre;
	procesoAInsertarEnLista = (l_procesosCargados*) malloc(sizeof(l_procesosCargados));
	espacioLibre = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));

	int socketMemoria, entero, a, cantidadPaginas, cantidadDePagLibres, respuesta;
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);

	aux = 0;
	while (aux != 1) {
		if (recv(socketMemoria, &entero, sizeof(int), 0) < 0)
			return EXIT_SUCCESS;
		switch (entero) {
		case 30: // SENTENCIA INICIAR

			if (recv(socketMemoria, &cantidadPaginas, sizeof(int), 0) < 0)
				return EXIT_SUCCESS;
			if (list_size(listaDeEspaciosLibres) != 0) {
				for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
					espacioLibre = list_get(listaDeEspaciosLibres, a);
					cantidadDePagLibres += espacioLibre->cantPagsLibres;
				}
			} else { //SI ESTAN TODAS LAS PAG LIBRES
				cantidadDePagLibres = configuracion->cantidadPaginas;
			}
			if (cantidadDePagLibres >= cantidadPaginas) {

				respuesta = 0;
				send(socketMemoria, &respuesta, sizeof(int), 0); // SI LE MANDA UN 0 SIGNIFICA OK
			} else {
				respuesta = 1;
				send(socketMemoria, &respuesta, sizeof(int), 0); // SI LE MANDA UN 1 SIGNIFICA ERROR
			}
			break;

		case 35: // escribir
			/*t_paqueteDelProceso* proceso;
			 proceso = (t_paqueteDelProceso*) malloc( sizeof(t_paqueteDelProceso) );
			 if(recv(socketMemoria, paquete, sizeof(int),0)<0) return EXIT_SUCCESS;
			 //DESERIALIZAR y guardar en proceso


			 */
			break;

		case 42: //leer

			break;

		case 99: //finalizar

			break;

		}
	}

//	recv(socketMemoria, &prueba, sizeof(int), 0);
//	printf("%i\n", prueba);
//	prueba = 5;
//	send(socketMemoria, &prueba, sizeof(int), 0);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("Swap procesar mensajes");
	defaultProcesarMensajes(socket, buffer, nuevaConexion, extra, logger);
	if (nuevaConexion) {

	} else {
		printf("Recibi el mensaje: %s\n", buffer);
	}
	return 0;
}
