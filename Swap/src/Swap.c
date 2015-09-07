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
	l_espacioLibre* espacioLibreAInsertar;
	procesoAInsertarEnLista = (l_procesosCargados*) malloc(sizeof(l_procesosCargados));
	espacioLibre = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	espacioLibreAInsertar = (l_espacioLibre*) malloc(sizeof(l_espacioLibre));
	pid_t pid;
	l_procesosCargados* unProceso;
	l_procesosCargados* procesoAleer;
	unProceso = (l_procesosCargados*) malloc(sizeof(l_procesosCargados));
	procesoAleer = (l_procesosCargados*) malloc(sizeof(l_procesosCargados));
	t_paqueteDelProceso* proceso;
	proceso = (t_paqueteDelProceso*) malloc(sizeof(t_paqueteDelProceso));
	int socketMemoria, entero, a, cantidadPaginas, cantidadDePagLibres, respuesta, paginasLibresRestantes;
	escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);

	aux = 0;
	while (aux != 1) {
		if (recv(socketMemoria, &entero, sizeof(int), 0) < 0)
			return EXIT_SUCCESS;
		switch (entero) {
		case 30: // SENTENCIA INICIAR

			if (recv(socketMemoria, &cantidadPaginas, sizeof(int), 0) < 0)
				return EXIT_SUCCESS;
			espacioLibre->ubicacion = 0;
			espacioLibre->cantPagsLibres = configuracion->cantidadPaginas;
			if (list_size(listaDeEspaciosLibres) != 0) {
				for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
					espacioLibre = list_get(listaDeEspaciosLibres, a);
					cantidadDePagLibres += espacioLibre->cantPagsLibres;
				}
			} else { //SI ESTAN TODAS LAS PAG LIBRES
				cantidadDePagLibres = configuracion->cantidadPaginas;
				espacioLibre->ubicacion = 0;
				espacioLibre->cantPagsLibres = configuracion->cantidadPaginas;
				list_add(listaDeEspaciosLibres, espacioLibre);
			}
			if (cantidadDePagLibres >= cantidadPaginas) {
				if (recv(socketMemoria, &pid, sizeof(int), 0) < 0)
					return EXIT_SUCCESS;
				acomodarEspaciosLibres(listaDeEspaciosLibres); // POR SI QUEDARON DON BLOQUES LIBRES CONTINUOS
				for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
					espacioLibre = list_get(listaDeEspaciosLibres, a);
					if (espacioLibre->cantPagsLibres >= cantidadPaginas) { // SI ENCUENTRO UN ESPACIO EN EL QE ENTRE LO METO y actualizo la lista de espacios libres
						procesoAInsertarEnLista->PID = pid;
						procesoAInsertarEnLista->ubicacion = espacioLibre->ubicacion;
						procesoAInsertarEnLista->cantPagsUso = cantidadPaginas;
						list_add(listaDeProcesosCargados, procesoAInsertarEnLista);
						paginasLibresRestantes = espacioLibre->cantPagsLibres - cantidadPaginas;
						list_remove(listaDeEspaciosLibres, a);
						espacioLibreAInsertar->cantPagsLibres = paginasLibresRestantes;
						espacioLibreAInsertar->ubicacion = espacioLibre->ubicacion + cantidadPaginas;
						list_add_in_index(listaDeEspaciosLibres, a, espacioLibreAInsertar);
					} else {
						//HACER ALGORITMO COMPACTACION
					}

					respuesta = 0;
					send(socketMemoria, &respuesta, sizeof(int), 0); // SI LE MANDA UN 0 SIGNIFICA OK
				}
			} else {
				respuesta = 1;
				send(socketMemoria, &respuesta, sizeof(int), 0); // SI LE MANDA UN 1 SIGNIFICA ERROR
			}
			break;

		case 35: // escribir

			//if(recv(socketMemoria, paquete, sizeof(int),0)<0) return EXIT_SUCCESS;
			//DESERIALIZAR y guardar en proceso

			break;

		case 42: //leer

			if (recv(socketMemoria, &pid, sizeof(int), 0) < 0)
				return EXIT_SUCCESS;
			for (a = 0; a < list_size(listaDeProcesosCargados); a++) { //BUSCO EL PROCESO CON EL MISMO PID EN LA LISTA
				unProceso = list_get(listaDeProcesosCargados, a);
				if (unProceso->PID == pid) {
					procesoAleer->PID = unProceso->PID;
					procesoAleer->cantPagsUso = unProceso->cantPagsUso;
					procesoAleer->ubicacion = unProceso->ubicacion;
				}
			}
			char* datosLeidos = leerEspacioDatos(espacioDatos, procesoAleer->ubicacion, procesoAleer->cantPagsUso);
			int tamanio = strlen(datosLeidos);
			send(socketMemoria, &tamanio, sizeof(int), 0);
			send(socketMemoria, datosLeidos, tamanio, 0);

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
