#include "CPU.h"
//escribir las funciones aqui

void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfig;
	t_config* archivoConfig = NULL;
	int result = 0;

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

int preparaCPU() {
	t_cpu* cpu = crearCPU();

	//recibe la info del sockect
	//procesa la info del sockect

	//llama a procesa codigo

	return EXIT_SUCCESS;

}
// se debe agregar instruccion que mape el archivo -> devuelva estructura
int cargaProcesoaCPU(char* dirArchivo, t_map* mCodCPU) {	//solo hay un CPU en este hilo por lo que se crea en el hilo  mCodCPU
	int fdmCod;

	fdmCod = abreArchivo(dirArchivo);
	mCodCPU->ptrTamPagina = tamanio_archivo(fdmCod);
	mCodCPU->ptrCMemoriaMap = (char*) crearEspacioDeDatos(fdmCod, mCodCPU->ptrTamPagina, logger);
	mCodCPU->bufferInstrucciones = string_split(mCodCPU->ptrCMemoriaMap, "\n");
	if (mCodCPU->bufferInstrucciones == NULL)
		log_error(logger, "[ERROR] al cargar las instrucciones en CPU>..>bufferInstrucciones");
	return EXIT_SUCCESS;

}

// se debe agregar funcion que intreprente instruccion ya mapeada
int interpretaInstruccion(char* instruccion_origen) {
	int resultado = 0;
	t_instruccion* instruccion = creaInstruccion();

	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = &instruccion->instruccion_separada[0];
//	instruccion->ptrParteLeida = &instruccion->instruccion_separada[0];
	resultado = leerInstruccion(instruccion->instruccion_separada);

	return EXIT_SUCCESS;
}

int ejecutaInstrucciones(t_map* mCodCPU, t_PCB* pcbCPU) {
	int i = 0;
	int result;
	while (mCodCPU->bufferInstrucciones[i] != NULL) {
		result = interpretaInstruccion(mCodCPU->bufferInstrucciones[i]);
		if (result != EXIT_SUCCESS) {
			perror("[ERROR] al interpretar la instruccion en CPU");
			log_error(logger, "[ERROR] al interpretar la instruccion en CPU");
			exit(-1);
		}
		pcbCPU->ptr_proximaInstruccion = mCodCPU->bufferInstrucciones[++i];

	}
	return EXIT_SUCCESS;
}

//carga codigo, interpreta y ejecuta las instrucciones
int procesaCodigo(char* dirArchivo) {

	t_map* mCodCPU = crearMapeo();
	t_mProc* mProc = crear_mProc();// este dentro ya crea el PCB

	strcpy(mProc->pcb->path,dirArchivo);

	cargaProcesoaCPU(dirArchivo, mCodCPU);
	mProc->pcb->cantInstruc = devuelveCantidadElementosArreglo(mCodCPU->bufferInstrucciones);
	ejecutaInstrucciones(mCodCPU, mProc->pcb);
	return EXIT_SUCCESS;
}

//funcion que ejecuta la instruccion
int ejecutaInstruccion(int token, char* separada_instruccion) {

	if (ejecutar(token, separada_instruccion) != EXIT_SUCCESS) {
		perror("[ERROR] al ejecutar la instruccion CPU");
		log_error(logger, "[ERROR] al ejecutar la instruccion CPU");
		exit(-1);
	}
	return EXIT_SUCCESS;
}

