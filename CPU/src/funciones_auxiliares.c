#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccion(char* string) {
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

	return -1;
}

char** separaInstruccion(char* instruccionCompleta) {
	char** vector_Instruccion = malloc(sizeof(char**));
	vector_Instruccion = string_n_split(instruccionCompleta, 2, " ");
	return vector_Instruccion;
}

int ejecutar(int token, char* separada_instruccion) {

	switch (token) {
	case (INICIAR_PROCESO_MEM): {

		break;
	}
	case (ESCRIBIR_MEM): {

		break;
	}
	case (LEER_MEM): {

		break;
	}
	case (FIN_PROCESO_MEM): {

		break;
	}
	case (ENTRADA_SALIDA): {

	}

	}

	return EXIT_SUCCESS;
}
//recibe la instruccion que se debe interpretar como una cadena de char
int leerInstruccion(char** instruccion) {
	int i = 0;
	int token;

	token = reconoceTokenInstruccion(instruccion[i]);
	ejecutar(token, instruccion[i + 1]);
	return EXIT_SUCCESS;
}
