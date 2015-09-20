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
	l_espacioLibre* espacioLibre2;
	l_espacioLibre* espacioLibre3;
	proceso = crearProceso();
	espacioLibre = crearEspacioLibre();
	espacioLibre2 = crearEspacioLibre();
	espacioLibre3 = crearEspacioLibre();
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

	/*
	 prueba para borrar el contenido mapeado cuando se va un proceso
	 iniciar(cantidadPaginas, listaDeEspaciosLibres, listaDeProcesosCargados, pid);
	 procesoAEscribir->PID = pid;
	 procesoAEscribir->contenido = "PASO USTED POR MI CASA , POR SU CASA YO PASE";
	 procesoAEscribir->numeroPagina = 1;

	 escribir(listaDeProcesosCargados, procesoAEscribir);
	 finalizar(pid, listaDeProcesosCargados, listaDeEspaciosLibres);
	 procesoRecibido->numeroPaginaFin = 2;
	 procesoRecibido->numeroPaginaInicio = 1;
	 procesoRecibido->PID = pid;


	 char* datosLeidos4 = leer(procesoRecibido, listaDeProcesosCargados);

	 printf("los datos leidos : %s\n", datosLeidos4);

	 */

	iniciar(cantidadPaginas, listaDeEspaciosLibres, listaDeProcesosCargados, pid);

	iniciar(cantidadPaginas3, listaDeEspaciosLibres, listaDeProcesosCargados, pid3);

	iniciar(cantidadPaginas4, listaDeEspaciosLibres, listaDeProcesosCargados, pid4);

	finalizar(pid3, listaDeProcesosCargados, listaDeEspaciosLibres);

	iniciar(cantidadPaginas5, listaDeEspaciosLibres, listaDeProcesosCargados, pid5);

	finalizar(pid5, listaDeProcesosCargados, listaDeEspaciosLibres);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "HOLA DON PEPITO HOLA DON JOSE";
	procesoAEscribir->numeroPagina = 5;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "PASO USTED POR MI CASA , POR SU CASA YO PASE";
	procesoAEscribir->numeroPagina = 6;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "los wachiturros";
	procesoAEscribir->numeroPagina = 10;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "que tp del ortooo";
	procesoAEscribir->numeroPagina = 9;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	finalizar(pid4, listaDeProcesosCargados, listaDeEspaciosLibres);

	iniciar(cantidadPaginas4, listaDeEspaciosLibres, listaDeProcesosCargados, pid4);

	procesoRecibido->numeroPaginaFin = 11;
		procesoRecibido->numeroPaginaInicio = 10;
		procesoRecibido->PID = pid4;

		char* datosLeidos5 = leer(procesoRecibido, listaDeProcesosCargados);

		printf("los datos leidos5 : %s\n", datosLeidos5);


	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "my name is juan";
	procesoAEscribir->numeroPagina = 11;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoRecibido->PID = pid4;
	procesoRecibido->numeroPaginaFin = 12;
	procesoRecibido->numeroPaginaInicio = 11;

	char* datosLeidos = leer(procesoRecibido, listaDeProcesosCargados);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "lalalalallalalalallaa";
	procesoAEscribir->numeroPagina = 6;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "nose que poneer";
	procesoAEscribir->numeroPagina = 7;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoAEscribir->PID = pid4;
	procesoAEscribir->contenido = "pepepepeppepee";
	procesoAEscribir->numeroPagina = 8;

	escribir(listaDeProcesosCargados, procesoAEscribir);

	procesoRecibido->numeroPaginaFin = 9;
	procesoRecibido->numeroPaginaInicio = 6;

	char* datosLeidos2 = leer(procesoRecibido, listaDeProcesosCargados);

	procesoRecibido->numeroPaginaFin = 6;
	procesoRecibido->numeroPaginaInicio = 6;

	char* datosLeidos3 = leer(procesoRecibido, listaDeProcesosCargados);

	procesoRecibido->numeroPaginaFin = 11;
	procesoRecibido->numeroPaginaInicio = 10;

	char* datosLeidos4 = leer(procesoRecibido, listaDeProcesosCargados);

	printf("los datos leidos : %s\n", datosLeidos);
	printf("los datos leidos2 : %s\n", datosLeidos2);
	printf("los datos leidos3 : %s\n", datosLeidos3);
	printf("los datos leidos4 : %s \n", datosLeidos4);

	printf("\nEMPIEZA LISTA DE PROCESOS\n\n");
	for (a = 0; a < list_size(listaDeProcesosCargados); a++) {
		proceso = list_get(listaDeProcesosCargados, a);
		printf("\n\n");
		printf("el pid :  %i\n", proceso->PID);
		printf("la ubicacion es : %i\n", proceso->ubicacion);
		printf("cantidad paginas : %i \n", proceso->cantPagsUso);

	}
	for (a = 0; a < list_size(listaDeEspaciosLibres); a++) {
		espacioLibre = list_get(listaDeEspaciosLibres, a);
		printf("\nempieza espacio libre\n\n\n");
		printf("ubicacion espacio libre : %i \n", espacioLibre->ubicacion);
		printf("cant pag libres :  %i\n", espacioLibre->cantPagsLibres);

	}

	//termina prueba
	//escucharConexiones(string_itoa(configuracion->puertoEscucha), 0, 0, 0, procesarMensajes, NULL, logger);

	//	recv(socketMemoria, &prueba, sizeof(int), 0);
	//	printf("%i\n", prueba);
	//	prueba = 5;
	//	send(socketMemoria, &prueba, sizeof(int), 0);

	return EXIT_SUCCESS;
}

int procesarMensajes(int socket, t_header* header, char* buffer, bool nuevaConexion, void* extra, t_log* logger) {
	puts("Swap procesar mensajes");
	defaultProcesarMensajes(socket, header, buffer, nuevaConexion, extra, logger);
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
