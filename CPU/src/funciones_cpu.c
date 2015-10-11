#include "CPU.h"
//lee el archivo de configuracion
void leerArchivoDeConfiguracion() {
	char* nombreArchivoConfig;
	t_config* archivoConfig = NULL;
	int result = 0;
	log_info(logger, "se va a ejecutar leerArchivoDeConfiguracion ");
	nombreArchivoConfig = strdup(
			"/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg");
	result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		exit(-1);
	} else {
		archivoConfig = config_create(nombreArchivoConfig);
		configuracion = malloc(sizeof(t_configuracion));
		configuracion->vg_puertoPlanificador = config_get_int_value(
				archivoConfig, "PUERTO_PLANIFICADOR");
		configuracion->vg_ipPlanificador = strdup(
				config_get_string_value(archivoConfig, "IP_PLANIFICADOR"));
		configuracion->vg_puertoMemoria = config_get_int_value(archivoConfig,
				"PUERTO_MEMORIA");
		configuracion->vg_ipMemoria = strdup(
				config_get_string_value(archivoConfig, "IP_MEMORIA"));
		configuracion->cantidad_hilos = config_get_int_value(archivoConfig,
				"CANTIDAD_HILOS");
		configuracion->retardo = config_get_int_value(archivoConfig, "RETARDO");
		log_info(logger,
				"[INFO]: Archivo de configuracion leido correctamente");
	}
	config_destroy(archivoConfig);

}
//es la funcion madre para cuando llega el Contexto de un proceso. Se inicia  el circuito de mCod
int preparaCPU(t_pcb* pcbPlanificador) {
	log_info(logger, "se va a ejecutar preparaCPU");
	t_cpu* cpu;

	cpu = list_get(procCPU->listaCPU, procCPU->contadorIdCPU - 1);
	cpu->pcbPlanificador = pcbPlanificador;
	return EXIT_SUCCESS;
}
// se debe agregar instruccion que mape el archivo -> devuelva estructura
int cargaProcesoaCPU(char* dirArchivo, t_mCod* mCodCPU) { //solo hay un CPU en este hilo por lo que se crea en el hilo  mCodCPU
	int fdmCod;
	log_info(logger, "se va a ejecutar cargaProcesoaCPU ");
	fdmCod = abrirArchivoLecturaEscritura(dirArchivo, logger);
	mCodCPU->ptrTamPagina = tamanio_archivo(fdmCod);
	mCodCPU->ptrCMemoriaMap = (char*) crearEspacioDeDatos(fdmCod,
			mCodCPU->ptrTamPagina, logger);
	mCodCPU->bufferInstrucciones = string_split(mCodCPU->ptrCMemoriaMap, "\n");
	if (mCodCPU->bufferInstrucciones == NULL)
		log_error(logger,
				"[ERROR] al cargar las instrucciones en CPU>..>bufferInstrucciones");
	return EXIT_SUCCESS;

}

// se debe agregar funcion que intreprente instruccion ya mapeada
int ejecuta_Instruccion(char* instruccion_origen, t_cpu* cpu) {
	int resultado = 0;

	t_instruccion* instruccion = creaInstruccion();

	log_info(logger, "se va a ejecutar interpretaInstruccion ");
	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = instruccion->instruccion_separada[0];

	int token;
	token = reconoceTokenInstruccion(instruccion->instruccion_separada[0]);
	//le estoy mandando solo la instruccion sin el token
	log_info(logger, "ejecutar ");
	if (ejecutar(token, instruccion->instruccion_separada[1],
			cpu) != EXIT_SUCCESS) {
		perror("[ERROR] al ejecutar la instruccion CPU");
		log_error(logger, "[ERROR] al ejecutar la instruccion CPU");
		exit(-1);
	}

	return EXIT_SUCCESS;
}
// ejecuta las instrucciones del mCod

//carga codigo, interpreta y ejecuta las instrucciones
int procesaCodigo(t_cpu* cpu) {
	log_info(logger, "se va a ejecutar procesaCodigo");
	t_mCod* mCodCPU = crearmCod();
	t_respuesta_ejecucion* respEjec = creaRespuestaEjecucion();
	mCodCPU->respEjec = respEjec;

//++++++++++++++++++++++++++++++++
	cargaProcesoaCPU(cpu->pcbPlanificador->rutaArchivoMcod, mCodCPU);
	mCodCPU->cantidadInstrucciones = devuelveCantidadElementosArreglo(
			mCodCPU->bufferInstrucciones);
	cpu->mCodCPU = mCodCPU;

	int i = 0;
	int result;
//++++++++++++++++++++++++++++++++
	log_info(logger, "se va a ejecutar  cada una de las Instrucciones");
	while ((cpu->mCodCPU->bufferInstrucciones[i] != NULL)
			&& (i != cpu->pcbPlanificador->tamanioRafaga)) {
		result = ejecuta_Instruccion(cpu->mCodCPU->bufferInstrucciones[i], cpu);
		if (result != EXIT_SUCCESS) {
			perror("[ERROR] al interpretar la instruccion en CPU");
			log_error(logger, "[ERROR] al interpretar la instruccion en CPU");
			exit(-1);
		} // se actualiza la proxima instruccion
		cpu->pcbPlanificador->proximaInstruccion = ++i;

	}

	return EXIT_SUCCESS;
}

