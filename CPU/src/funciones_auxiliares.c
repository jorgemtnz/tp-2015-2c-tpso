#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccion(char* string) {
	log_info(logger,
			"se va a ejecutar reconoce token segun el comando de la instruccion ");
	if (string_ends_with(string, ";")) {
		int tamanioString = strlen(string);
		string[tamanioString - 1] = '\0';
	}
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

//desmapea el mCod cargado para CPU
int descargaProcesoCPU(t_mCod* mCod) {
	eliminarEspacioDeDatos(mCod->ptrCMemoriaMap, mCod->ptrTamPagina, logger);
	return EXIT_SUCCESS;
}

//char* pedirRutaArchivo() {
//	char buf[4096] = { };
//	int numbytes;
//	int intentos = 0;
//	log_info(logger, "se va a ejecutar pedirRutaArchivo");
//	printf("Ingrese la ruta del archivo de configuracion");
//	fflush(stdin);
//
//	numbytes = read(STDIN_FILENO, buf, sizeof(buf));
//	while (numbytes > 0) {
//		/* va a estar leyendo constantemente desde pantalla
//		 * AQUI VA EL TRATAMIENTO QUE PIDA EL EJERCICIO.
//		 */
//
//		numbytes = read(STDIN_FILENO, buf, sizeof(buf));
//	}
//	if (numbytes == -1) {
//		perror("fallo en read");
//		log_error(logger, " nombre archivo configuracion no ingresado");
//		exit(EXIT_FAILURE);
//	}
//
//	return buf;
//}

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
t_resultado_instruccion* resultado = creaResultadoInstruccion();

char* temporal;
//strcpy(resultado->comandoInstruccion, "finalizar");

resultado->comandoInstruccion = "finalizar";
resultado->tipoMensaje = RESUL_FIN_OK;
char* finalizado = "finalizado";
//		temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid,
//				finalizado);

temporal = string_from_format("mProc %d", cpu->pcbPlanificador->pid);
resultado->expresion = temporal;

//		strcpy(resultado->expresion, temporal);
list_add(cpu->mCodCPU->respEjec->resultadosInstrucciones, resultado);
cpu->mCodCPU->respEjec->finalizoOk = true;

cpu->mCodCPU->respEjec->pcb = cpu->pcbPlanificador;
return cpu;
}
