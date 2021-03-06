#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccion(char* string) {
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

char** separaInstruccion(char* instruccionCompleta) {
	char ** vector_Instruccion = string_split(instruccionCompleta, " ");

	if (string_equals_ignore_case(vector_Instruccion[0], "escribir")) {
		char * vector_escribir = string_new();
		string_append(&vector_escribir, "[escribir,");
		char* aux = vector_Instruccion[1];
		string_append(&vector_escribir, aux);
		string_append(&vector_escribir, ",");
		int a;
		int longitud;
		char* b = string_new();
		char* aux2 = string_new();
		int cont = 0;
		a = 0;
		while (vector_Instruccion[a] != NULL) {
			cont++;
			a++;
		}
		a = 2;
		while (vector_Instruccion[a] != NULL) {
			aux2 = vector_Instruccion[a];
			longitud = strlen(aux2) + 1;
			if ((a == 2) && (a != cont - 1)) {
				aux2 = string_substring(vector_Instruccion[a], 1, longitud - 2);
			}
			if ((a == cont - 1) && (a != 2)) {
				aux2 = string_substring(vector_Instruccion[a], 0, longitud - 2);
			}
			if ((a == 2) && (cont - 1 == a)) {
				aux2 = string_substring(vector_Instruccion[a], 1, longitud - 3);
			}

			string_append(&b, aux2);
			string_append(&b, " ");
			a++;
		}
		string_append(&b, ",NULL]");
		string_append(&vector_escribir, b);
		char** vectorADevolver = string_get_string_as_array(vector_escribir);
		return vectorADevolver;
	}
	return vector_Instruccion;
}

int devuelveCantidadElementosArreglo(char** arreglo) {
	int contador = 0;
	while (arreglo[contador] != NULL) {
		contador++;
	}
	if (contador == 0) {
		perror("[ERROR] arreglo vacio");
	}
	return contador;
}

int putsConsola(const char *msg) {
	return puts(msg);
}

char* queCPUsoy(t_cpu* cpu) {
	char* resultado = string_new();
	resultado = string_from_format("soy la  %s \n", cpu->nombre);
	return resultado;

}

pthread_t queHiloSoy() {
	return pthread_self();
}

char* identificaCPU(pthread_t idHilo) {
	char* resultado = string_new();
	resultado = string_from_format("soy la CPU de PID  %lu ", idHilo);
//	puts(resultado);
	return resultado;
}

double instEquivalenteCienPorciento(uint8_t retardoTotal) {
	double divisor = 0;
	if (retardoTotal == 0) {
		divisor = 1;
	} else {
		divisor = retardoTotal;
	}

	int cienPorcien = 0;
	cienPorcien = (uint8_t) 60 / divisor;
	return cienPorcien;
}

bool primeraMayorque(time_t unaFecha, time_t otraFecha) {
	double seconds = difftime(unaFecha, otraFecha);
	if (seconds > 0)
		//unaFEcha > otraFecha
		return true;
	return false;
}

bool fechasIguales(time_t unaFecha, time_t otraFecha) {
	double seconds = difftime(unaFecha, otraFecha);
	if (seconds == 0)
		//unaFEcha > otraFecha
		return true;
	return false;
}

double dameDiferencia(time_t* unaFecha, time_t *otraFecha) {
	if (unaFecha == NULL || otraFecha == NULL) {
//		printf("diferencia cero fechaAnterior %s fechaactual %s \n, ", ctime( unaFecha),ctime( otraFecha));
		return 0;
	}
	double retorno = abs(difftime(*unaFecha, *otraFecha));
	;
//	printf("diferencia en segundos %f \n",retorno);
	return retorno;
}

void resetValPorcentaje(t_cpu* cpu) {
	cpu->cantInstEjecutadasPorcentaje = 0;
	cpu->acumuladoSegundos = 0;
//	printf("cantInstEjecutadasPorcentaje % d\n",
//			cpu->cantInstEjecutadasPorcentaje);
//	printf("acumuladoSegundos %f \n", cpu->acumuladoSegundos);
//cpu->finInstruccion = time(&cpu->inicioInstruccion);//ambas con la misma fecha
}

void calculaAcumulado(t_cpu* cpu) {
	time_t *now = malloc(sizeof(time_t));
	time(now); //fin de una instruccion

	pthread_mutex_lock(&mutexCPUPorcentaje);
	cpu->acumuladoSegundos += dameDiferencia(cpu->inicioInstruccion, now);
	pthread_mutex_unlock(&mutexCPUPorcentaje);

}

void calculafecha(t_cpu* cpu) {
	time_t *now = malloc(sizeof(time_t));
	time(now); //fin de una instruccion
	cpu->inicioInstruccion = now;
}

void incrementaInstPorcentaje(t_cpu* cpu) {
	pthread_mutex_lock(&mutexCPUPorcentaje);
	cpu->cantInstEjecutadasPorcentaje += 1;
	pthread_mutex_unlock(&mutexCPUPorcentaje);
}

void decrementeInstProcentaje(t_cpu* cpu) {
	pthread_mutex_lock(&mutexCPUPorcentaje);
	cpu->cantInstEjecutadasPorcentaje -= 1;
	pthread_mutex_unlock(&mutexCPUPorcentaje);
}

int redondea(double tiempoPromedio) {

	double parte_entera = 0;
	double result = 60 / tiempoPromedio;
	double parte_decimal = modf(result, &parte_entera);
//	printf("++++++++++++PARTE DECIMAL %f\n", parte_decimal);
	if (parte_decimal > 0) {

		return (int) parte_entera + 1;
	} else {
		return (int) parte_entera;
	}

}
