#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccion(char* string) {

	string_to_lower(string);
	if (string_equals(string, "inicializar"))
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
	char** vector_Instruccion = malloc(sizeof(char**));
	vector_Instruccion = string_n_split(instruccionCompleta, 2, " ");
	return vector_Instruccion;
}

int ejecutar(int token, char* separada_instruccion) {

	switch (token) {
	case (INICIAR_PROCESO_MEM): {
		ejecutaIniciarProceso();
		break;
	}
	case (ESCRIBIR_MEM): {
		ejecutaEscribirMemoria();
		break;
	}
	case (LEER_MEM): {
		ejecutaLeerMemoria();
		break;
	}
	case (FIN_PROCESO_MEM): {
		ejecutaFinProcesoMemoria();
		break;
	}
	case (ENTRADA_SALIDA): {
		ejecutaEntradaSalida();
	}

	}

	return EXIT_SUCCESS;
}
//recibe la instruccion que se debe interpretar como una cadena de char
int leerInstruccion(char** instruccion) {
	int i = 0;
	int token;

	token = reconoceTokenInstruccion(instruccion[i]);
	ejecutaInstruccion(token, instruccion[i + 1]);
	return EXIT_SUCCESS;
}
//desmapea el mCod cargado para CPU
int descargaProcesoCPU(t_map* mCod) {
	eliminarEspacioDeDatos(mCod->ptrCMemoriaMap, mCod->ptrTamPagina, logger);
	return EXIT_SUCCESS;
}

char* pedirRutaArchivo() {
	char* nombreArchivoConfig = '\0';

	printf("Ingrese la ruta del archivo de configuracion");
	fflush(stdin);
	scanf("%s", nombreArchivoConfig);
	if (nombreArchivoConfig == '\0')
		perror("[ERROR] nombre archivo configuracion no ingresado");
	log_error(logger, "[ERROR] nombre archivo configuracion no ingresado");

	return nombreArchivoConfig;
}

int devuelveCantidadElementosArreglo(char** arreglo) {
	int contador = 0;
	while (arreglo[contador] != NULL) {
		contador++;
	}
	if (contador == 0)
		perror("[ERROR] arreglo vacio");
	log_error(logger, "[ERROR] arreglo vacio");

	return contador;
}

int ejecutaIniciarProceso() {

	return EXIT_SUCCESS;
}

int ejecutaEscribirMemoria() {

	return EXIT_SUCCESS;
}

int ejecutaLeerMemoria() {

	return EXIT_SUCCESS;
}

int ejecutaFinProcesoMemoria() {

	return EXIT_SUCCESS;
}

int ejecutaEntradaSalida() {

	return EXIT_SUCCESS;
}


