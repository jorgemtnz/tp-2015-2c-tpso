#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccion(char* string) {
	log_info(logger,
			"se va a ejecutar reconoce token segun el comando de la instruccion ");
//	if (string_ends_with(string, ";")) {
//		int tamanioString = strlen(string);
//		string[tamanioString - 1] = '\0';
//	}
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
	//char** vector_Instruccion = malloc(sizeof(char**));


	char ** vector_Instruccion = string_split(instruccionCompleta, " ");

	if(string_equals_ignore_case(vector_Instruccion[0], "escribir")){
		char * vector_escribir = string_new();
		string_append(&vector_escribir , "[escribir,");
		char* aux = vector_Instruccion[1];
		string_append(&vector_escribir , aux);
		string_append(&vector_escribir , ",");
		int a ;
		int longitud;
		char* b = string_new();
		char* aux2 = string_new();
		int cont = 0;
		a = 0;
		while(vector_Instruccion[a] != NULL){
			cont++;
			a++;
		}
		a = 2;
		while( vector_Instruccion[a] != NULL){
			aux2 = vector_Instruccion[a];
			longitud = strlen(aux2) +1;
			if((a == 2 ) && (a != cont -1)){
			aux2 =     string_substring(vector_Instruccion[a], 1, longitud - 2);}
			if((a == cont -1) && (a != 2 )){
				aux2 =     string_substring(vector_Instruccion[a], 0, longitud - 2);
			}
			if((a == 2) && (cont -1 == a)){
				aux2 =     string_substring(vector_Instruccion[a], 1, longitud - 3);
			}


			string_append(&b , aux2);
			string_append(&b , " ");

	a++;
		}
		string_append(&b , ",NULL]");
		string_append(&vector_escribir , b);
		char** vectorADevolver =  string_get_string_as_array(vector_escribir);
		return vectorADevolver ;

	}

	return vector_Instruccion;
}
//desmapea el mCod cargado para CPU
int descargaProcesoCPU(t_mCod* mCod) {
//	eliminarEspacioDeDatos(mCod->ptrCMemoriaMap, mCod->ptrTamPagina, logger);
	return EXIT_SUCCESS;
}

int devuelveCantidadElementosArreglo(char** arreglo) {
	log_info(logger, "se va a ejecutar devuelveCantidadElementosArreglo");
	int contador = 0;
	while (arreglo[contador] != NULL) {
		contador++;
	}
	if (contador == 0){
		perror("[ERROR] arreglo vacio");
	log_error(logger, "[ERROR] arreglo vacio");
	}
	return contador;
}

int putsConsola (const char *msg) {
	log_info(logger, msg);
	return puts(msg);
}

t_cpu* ejecutarResul_Fin(t_cpu* cpu){
//t_resultado_instruccion* resultado = creaResultadoInstruccion();
//
//char* temporal;
////strcpy(resultado->comandoInstruccion, "finalizar");
//
//resultado->comandoInstruccion = "finalizar";
//resultado->tipoMensaje = RESUL_FIN_OK;
//char* finalizado = "finalizado";
////		temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid,
////				finalizado);
//
//temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid);
//resultado->expresion = temporal;
//
////		strcpy(resultado->expresion, temporal);
//list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
//cpu->mCodCPU->respEjec->finalizoOk = true;
//
//cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
return cpu;
}

char* queCPUsoy(t_cpu* cpu){
	char* resultado = string_new();
	resultado = string_from_format("soy la %s con numero %d \n",cpu->nombre,cpu->idCPU );
	return resultado;
}
