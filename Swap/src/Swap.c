#include "Swap.h"

t_dictionary* conexiones;
int main(int argc, char *argv[]) {
	conexiones = dictionary_create();

	logger = log_create("LOG_SWAP.log", "Swap", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion(argc, argv);

	crearArchivo();

	//empieza prueba(NO BORRAR)
	int cantidadPaginas = 8;
	int cantidadPaginas3 = 5;
	int cantidadPaginas4 = 15;
	int cantidadPaginas5 = 3;
	int a;
	l_procesosCargados* proceso;
	l_espacioLibre* espacioLibre;
	proceso = crearProceso();
	espacioLibre = crearEspacioLibre();
	t_list* listaDeProcesosCargados;
	t_list* listaDeEspaciosLibres;
	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();
	pid_t pid = 2;
	pid_t pid3 = 3;
	pid_t pid4 = 5;
	pid_t pid5 = 6;
	t_escribirEnProceso* procesoAEscribir;
	procesoAEscribir = crearEscribirEnProceso();
	t_leerDeProceso *procesoRecibido;
	procesoRecibido = crearLeerDeProceso();


	iniciar(cantidadPaginas, listaDeEspaciosLibres, listaDeProcesosCargados, pid);
	iniciar(cantidadPaginas3, listaDeEspaciosLibres, listaDeProcesosCargados, pid3);

	iniciar(cantidadPaginas4, listaDeEspaciosLibres, listaDeProcesosCargados, pid4);
	finalizar(pid3, listaDeProcesosCargados, listaDeEspaciosLibres);
	iniciar(cantidadPaginas5, listaDeEspaciosLibres, listaDeProcesosCargados, pid5);
	finalizar(pid5, listaDeProcesosCargados, listaDeEspaciosLibres);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "HOLA DON PEPITO HOLA DON JOSE";
	procesoAEscribir->numeroPagina = 5;

	escribir(listaDeProcesosCargados, procesoAEscribir) ;

	procesoAEscribir->PID = pid4;
		procesoAEscribir->contenido = "PASO USTED POR MI CASA , POR SU CASA YO PASE";
		procesoAEscribir->numeroPagina = 6;

		escribir(listaDeProcesosCargados, procesoAEscribir) ;

	procesoRecibido->PID = pid4;
	procesoRecibido->numeroPaginaFin = 7;
	procesoRecibido->numeroPaginaInicio = 5;

	char* datosLeidos = leer(procesoRecibido,listaDeProcesosCargados);

	printf("los datos leidos : %s\n", datosLeidos);

	for (a = 0; a < list_size(listaDeProcesosCargados); a++) {
		proceso = list_get(listaDeProcesosCargados, a);

		printf("el pid :  %i\n", proceso->PID);
		printf("cantidad paginas : %i \n", proceso->cantPagsUso);
		printf("la ubicacion es : %i\n", proceso->ubicacion);
	}
	for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
		espacioLibre = list_get(listaDeEspaciosLibres, a);
		printf("empieza espacio libre\n");
		printf("cant pag libres :  %i\n", espacioLibre->cantPagsLibres);
		printf("ubicacion espacio libre : %i \n", espacioLibre->ubicacion);

	}

	//termina prueba
	//escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);

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
