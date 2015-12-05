#include "CPU.h"
//lee el archivo de configuracion
void leerArchivoDeConfiguracion(int argc, char *argv[]) {
	t_config* archivoConfig = NULL;
	int result = 0;
	char* logMsg = NULL;

//	log_info(logger,identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar leerArchivoDeConfiguracion ");

	if (argc < 2) {
		logMsg =
				string_from_format(
						"Debe especificar la ruta al archivo de configuracion, al invocar al programa, por ejemplo: ./CPU /home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg\n");
		putsConsola(logMsg);
//		pthread_mutex_lock(&mutexCPULogs);
//		log_info(logger,identificaCPU(queHiloSoy()));
//		log_error(logger, logMsg);
//		pthread_mutex_unlock(&mutexCPULogs);
		abort();
	}
	char* nombreArchivoConfig = nombreArchivoConfig = strdup(argv[1]);
//	nombreArchivoConfig = strdup(
//			"/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg");
	result = checkearRutaArchivoConfig(nombreArchivoConfig);
	if (result == -1) {
		perror("[ERROR]: Archivo de configuracion no encontrado");
//	log_info(logger,identificaCPU(queHiloSoy()));
		log_error(logger, "[ERROR]: Archivo de configuracion no encontrado");
		abort();
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
//		log_info(logger,identificaCPU(queHiloSoy()));
		log_info(logger,
				"[INFO]: Archivo de configuracion leido correctamente");
	}
	config_destroy(archivoConfig);
}

// se debe agregar funcion que intreprente instruccion ya mapeada
void ejecuta_Instruccion(char* instruccion_origen, t_cpu* cpu) {

	t_instruccion* instruccion = creaInstruccion();

	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);

	int token = 0;
	token = reconoceTokenInstruccion(instruccion->instruccion_separada[0]);

	//le estoy mandando solo la instruccion sin el token
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar interpretaInstruccion ");
	log_info(logger, string_from_format("token:  %i \n", token));
	log_info(logger,
			string_from_format("\n================ Ejecutando %s\n",
					instruccion->instruccion_separada[0]));
	pthread_mutex_unlock(&mutexCPULogs);
//	printf("\n================ Ejecutando %s, en cpu %lu\n", instruccion->instruccion_separada[0], cpu->idCPU);
	ejecutar(token, instruccion->instruccion_separada, cpu);
}
// ejecuta las instrucciones del mCod
//carga codigo, interpreta y ejecuta las instrucciones
void procesaCodigo(t_cpu* cpu) {
//	puts(
//						string_from_format(
//								"se esta procesando el codigo  %s  %s PID %i \n",
//								queCPUsoy(cpu), identificaCPU(cpu->idCPU),
//								cpu->pcbPlanificador->pid));

	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar procesaCodigo");
	pthread_mutex_unlock(&mutexCPULogs);

	int fd = open(cpu->pcbPlanificador->rutaArchivoMcod, O_RDONLY);
	if (fd == -1) {
		perror("Error al abrir el archivo");
	}
	int tamanio = file_get_size(cpu->pcbPlanificador->rutaArchivoMcod);
	char buffer[tamanio];
	int numbytes = read(fd, buffer, sizeof(buffer));
	buffer[tamanio] = '\0';
	if (numbytes == -1) {
		perror("fallo en read");
		exit(EXIT_FAILURE);
	}
	char** sinEnter; //uso auxiliar
	sinEnter = string_split(buffer, "\n");
	int i = 0;
	char* sin_enter = string_new(); //cadena sin los enter
	while (sinEnter[i] != NULL) {
		string_append(&sin_enter, sinEnter[i]);
		i++;
	} //el buffer solo contiene las instrucciones, sin punto y coma
//	printf("%s sin enter\n", sin_enter);
	t_mCod* mCodCPU = crearmCod();
	mCodCPU->bufferInstrucciones = string_split(sin_enter, ";");
	mCodCPU->cantidadInstrucciones = devuelveCantidadElementosArreglo(
			mCodCPU->bufferInstrucciones);
	cpu->mCodCPU = mCodCPU;
	cpu->mCodCPU->bufferInstrucciones = mCodCPU->bufferInstrucciones;
	cpu->mCodCPU->cantidadInstrucciones = mCodCPU->cantidadInstrucciones;
	//se resta uno, porque se conto el cero anteriormente
	cpu->pcbPlanificador->instruccionFinal = mCodCPU->cantidadInstrucciones - 1;

	//++++++++++++++++++++++++++++++++sin_enter ya tengo un string con las instrucciones
//aca estoy haciendo la ejecucion de una de las instrucciones
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a ejecutar una Instruccion en donde quedo");
	pthread_mutex_unlock(&mutexCPULogs);
//	printf("antes de ejecuta instruccion e if \n");
	if(cpu->pcbPlanificador->proximaInstruccion>cpu->pcbPlanificador->instruccionFinal){
		printf("[[ERROR]]  LA PROXIMA INSTRUCCION EXCEDE EL BUFFER");
		abort();
	}
	ejecuta_Instruccion(
			cpu->mCodCPU->bufferInstrucciones[cpu->pcbPlanificador->proximaInstruccion],
			cpu);
}
