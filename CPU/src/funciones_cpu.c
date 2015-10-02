#include "CPU.h"
//lee el archivo de configuracion
void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfig;
	t_config* archivoConfig = NULL;
	int result = 0;
        log_info(logger, "se va a ejecutar leerArchivoDeConfiguracion ");
	nombreArchivoConfig = strdup("/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg");
	result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {
		archivoConfig = config_create(nombreArchivoConfig);
		configuracion = malloc(sizeof(t_configuracion));
		configuracion->vg_puertoPlanificador = config_get_int_value(archivoConfig, "PUERTO_PLANIFICADOR");
		configuracion->vg_ipPlanificador = strdup(config_get_string_value(archivoConfig, "IP_PLANIFICADOR"));
		configuracion->vg_puertoMemoria = config_get_int_value(archivoConfig, "PUERTO_MEMORIA");
		configuracion->vg_ipMemoria = strdup(config_get_string_value(archivoConfig, "IP_MEMORIA"));
		configuracion->cantidad_hilos = config_get_int_value(archivoConfig, "CANTIDAD_HILOS");
		configuracion->retardo = config_get_int_value(archivoConfig, "RETARDO");
		log_info(logger, "[INFO]: Archivo de configuracion leido correctamente");
	}
	config_destroy(archivoConfig);

}
//es la funcion madre para cuando llega el Contexto de un proceso. Se inicia todo el circuito de mCod
int preparaCPU(t_pcb* pcbPlanificador, int socket) {
	log_info(logger, "se va a ejecutar preparaCPU");
	t_cpu* cpu = crearCPU();

	//recibe la info del sockect
	//procesa la info del sockect

	//llama a procesa codigo
	procesaCodigo(pcbPlanificador, socket);

	return EXIT_SUCCESS;

}
// se debe agregar instruccion que mape el archivo -> devuelva estructura
int cargaProcesoaCPU(char* dirArchivo, t_map* mCodCPU) {	//solo hay un CPU en este hilo por lo que se crea en el hilo  mCodCPU
	int fdmCod;
        log_info(logger, "se va a ejecutar cargaProcesoaCPU "); 
	fdmCod = abrirArchivoLecturaEscritura(dirArchivo, logger);
	mCodCPU->ptrTamPagina = tamanio_archivo(fdmCod);
	mCodCPU->ptrCMemoriaMap = (char*) crearEspacioDeDatos(fdmCod, mCodCPU->ptrTamPagina, logger);
	mCodCPU->bufferInstrucciones = string_split(mCodCPU->ptrCMemoriaMap, "\n");
	if (mCodCPU->bufferInstrucciones == NULL)
		log_error(logger, "[ERROR] al cargar las instrucciones en CPU>..>bufferInstrucciones");
	return EXIT_SUCCESS;

}

// se debe agregar funcion que intreprente instruccion ya mapeada
int interpretaInstruccion(char* instruccion_origen,   t_pcb* pcbPlanificador, int socket) {
	int resultado = 0;
	t_instruccion* instruccion = creaInstruccion();
        
        log_info(logger, "se va a ejecutar interpretaInstruccion ");
	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = &instruccion->instruccion_separada[0];
//	instruccion->ptrParteLeida = &instruccion->instruccion_separada[0];
	resultado = leerInstruccion(instruccion->instruccion_separada,    pcbPlanificador, socket);
	if (resultado== -1)
	log_error(logger, "[ERROR] al leer instruccion en CPU");

	return EXIT_SUCCESS;
}
// ejecuta las instrucciones del mCod
int ejecutaInstrucciones(t_map* mCodCPU, t_pcb* pcbPlanificador, int socket) {
	int i = 0;
	int result;
	log_info(logger, "se va a ejecutar ejecutaInstrucciones");
	while (mCodCPU->bufferInstrucciones[i] != NULL) {
		result = interpretaInstruccion( mCodCPU->bufferInstrucciones[i],  pcbPlanificador, socket );
		if (result != EXIT_SUCCESS) {
			perror("[ERROR] al interpretar la instruccion en CPU");
			log_error(logger, "[ERROR] al interpretar la instruccion en CPU");
			exit(-1);
		}
		pcbPlanificador->proximaInstruccion = ++i;

	}
	return EXIT_SUCCESS;
}

//carga codigo, interpreta y ejecuta las instrucciones
int procesaCodigo(t_pcb* pcbPlanificador, int socket) {

	t_map* mCodCPU = crearMapeo();

        log_info(logger, "se va a ejecutar procesaCodigo");
	cargaProcesoaCPU(pcbPlanificador->rutaArchivoMcod, mCodCPU);
	mCodCPU->cantidadInstrucciones = devuelveCantidadElementosArreglo(mCodCPU->bufferInstrucciones);
	ejecutaInstrucciones(mCodCPU, pcbPlanificador, socket);
	return EXIT_SUCCESS;
}

//funcion que ejecuta la instruccion
int ejecutaInstruccion(int token, char* separada_instruccion,   t_pcb* pcbPlanificador, int socket) {


          log_info(logger, "se va a ejecutar ejecutaInstruccion ");
	if (ejecutar(token, separada_instruccion,pcbPlanificador, socket ) != EXIT_SUCCESS) {
		perror("[ERROR] al ejecutar la instruccion CPU");
		log_error(logger, "[ERROR] al ejecutar la instruccion CPU");
		exit(-1);
	}
	return EXIT_SUCCESS;
}

