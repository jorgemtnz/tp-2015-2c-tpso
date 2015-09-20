#include "Planificador.h"

/*---------------------------------------------------------------------- CONSOLA------------------------------*/
//========================funciones consola=====================

int procesarMensajesConsola(int socket, t_header* header, char* buffer) {
	if(buffer != NULL && strstr(buffer, "\n")) {
		char** split = string_split(buffer, "\n");
		buffer = split[0];
	} else {
		printConsola("Recibi el mensaje por consola: %s\n", buffer);
	}
	if(string_equals(buffer, "correr programa")) {
		correrPrograma(socket, header, buffer);
	} else if(string_starts_with(buffer, "correr")) {
		correrPath(socket, header, buffer);
	} else if(string_starts_with(buffer, "finalizar")) {
		finalizarPid(socket, header, buffer);
	} else if(string_equals(buffer, "ps")) {
		ps(socket, header, buffer);
	} else if(string_equals(buffer, "cpu")) {
		cpu(socket, header, buffer);
	} else {
		printConsola("Comando no reconocido: %s\n", buffer);
	}
	return 0;
}

void mostrarComandos() {
	char* funcionesConsola[] = { "correr_PATH", "finalizar_PID", "ps", "cpu" };

	char* descripcionesConsola[] = { "Descrpcion de la funcion 1",
			"Descripcion de la funcion 2", "Descripcion de la funcion 3",
			"Descripcion de la funcion 4" };

	int contador = 0;
	while (contador <= NUMEROFUNCIONESCONSOLA - 1) {
		printConsola("*------------------------------------------*\n");
		printConsola("COMANDO 	= %s\n", funcionesConsola[contador]);
		printConsola("DESCRIPCION = %s\n", descripcionesConsola[contador]);
		printConsola("*------------------------------------------*\n");
		contador += 1;
	}
}

// ---------CONSOLA IMPLEMENTACION------
void limpiarPantalla() {
	system("clear");
}

int idFuncion(char* funcion) {
	int i;

	char* funcionesConsola[] = { "correr_PATH", "finalizar_PID", "ps", "cpu" };

	for (i = 0;
			(i < NUMEROFUNCIONESCONSOLA)
					&& (strcmp(funcion, funcionesConsola[i]) != 0); i++)
		;
	return (i <= NUMEROFUNCIONESCONSOLA - 1) ? (i + 1) : -1;
}

void aplicarFuncion(int idFuncion) { //selecciona un case en base al numero que llevaba el contador y aplica la funcion recibe el dir

	enum nomFun {
		CORRER_PATH = 1, FINALIZAR_PID, PS, CPU, MOSTRAR_COMANDOS
	};
// Lo que hace el enum es convertirme lo que dice en enteros
	switch (idFuncion) {
	case CORRER_PATH: {
		printConsola("soy correr path \n");

		break;
	}
	case FINALIZAR_PID:
		printConsola("soy finalizar pid \n");

		break;

	case PS:
		printConsola("soy ps \n");

		break;

	case CPU:
		printConsola("soy cpu \n");

		break;

	case MOSTRAR_COMANDOS: {

		mostrarComandos();

	}
		break;

	case -1:
		printConsola("--Ojo ese comando no existe!! proba con mostrarComandos \n");

	}
}

void levantarConsola() {
	char comando[50];
	int idFunc;

	mostrarComandos();

	while (1) {
		printConsola("Ingrese un comando >> ");
//		fgets(comando, 50, stdin);
		scanf("%s", comando);
		idFunc = idFuncion(comando);
		aplicarFuncion(idFunc);
	}

}
