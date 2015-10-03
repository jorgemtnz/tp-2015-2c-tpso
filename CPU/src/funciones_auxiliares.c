#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccion(char* string) {
log_info(logger, "se va a ejecutar reconoce token segun el comando de la instruccion ");
	string_to_lower(string);
	if (string_equals(string, "iniciar"))
		return INICIAR_PROCESO_MEM;
	if (string_equals(string, "leer"))
		return LEER_MEM;
	if (string_equals(string, "escribir"))
		return ESCRIBIR_MEM;
	if (string_equals(string, "entrada-salida"))
		return ENTRADA_SALIDA;
	if (string_equals(string, "finalizar"))
		return FIN_PROCESO_MEM;

//si no entro a ningun if
	return -1;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++preguntar
char** separaInstruccion(char* instruccionCompleta) {
	log_info(logger, "se va a ejecutar separaInstruccion ");
	char** vector_Instruccion = malloc(sizeof(char**));
	vector_Instruccion = string_n_split(instruccionCompleta, 2, " ");
	return vector_Instruccion;
}

//recibe la instruccion que se debe interpretar como una cadena de char
int leerInstruccion(char** instruccion, t_pcb* pcbPlanificador, int socket) {
	log_info(logger, "se va a ejecutar leerInstruccion");
	int i = 0;
	int token;
    int resultado=0;
	token = reconoceTokenInstruccion(instruccion[i]);
	resultado = ejecutaInstruccion(token, instruccion[i + 1], pcbPlanificador, socket);
	if (resultado== -1)
		log_error(logger, "[ERROR] al ejecutarInstruccion en CPU");

	return EXIT_SUCCESS;
}
//desmapea el mCod cargado para CPU
int descargaProcesoCPU(t_map* mCod) {
	eliminarEspacioDeDatos(mCod->ptrCMemoriaMap, mCod->ptrTamPagina, logger);
	return EXIT_SUCCESS;
}

char* pedirRutaArchivo() {
	char* nombreArchivoConfig = '\0';
        log_info(logger, "se va a ejecutar pedirRutaArchivo");
	printf("Ingrese la ruta del archivo de configuracion");
	fflush(stdin);
	scanf("%s", nombreArchivoConfig);
	if (nombreArchivoConfig == '\0')
		perror("[ERROR] nombre archivo configuracion no ingresado");
	log_error(logger, "[ERROR] nombre archivo configuracion no ingresado");

	return nombreArchivoConfig;
}

int devuelveCantidadElementosArreglo(char** arreglo) {
	log_info(logger, "se va a ejecutar devuelveCantidadElementosArreglo");
	int contador = 0;
	while (arreglo[contador] != NULL) {
		contador++;
	}
	if (contador == 0)
		perror("[ERROR] arreglo vacio");
	log_error(logger, "[ERROR] arreglo vacio");

	return contador;
}


