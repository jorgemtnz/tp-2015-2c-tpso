#include "Planificador.h"

/*---------------------------------------------------------------------- CONSOLA------------------------------*/
//========================funciones consola=====================

void mostrarComandos() {
	char* funcionesConsola[] = { "correr_PATH", "finalizar_PID", "ps", "cpu" };

	char* descripcionesConsola[] = { "Descrpcion de la funcion 1",
			"Descripcion de la funcion 2", "Descripcion de la funcion 3",
			"Descripcion de la funcion 4" };

	int contador = 0;
	while (contador <= NUMEROFUNCIONESCONSOLA - 1) {
		printf("*------------------------------------------*\n");
		printf("COMANDO 	= %s\n", funcionesConsola[contador]);
		printf("DESCRIPCION = %s\n", descripcionesConsola[contador]);
		printf("*------------------------------------------*\n");
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
		printf("soy correr path \n");

		break;
	}
	case FINALIZAR_PID:
		printf("soy finalizar pid \n");

		break;

	case PS:
		printf("soy ps \n");

		break;

	case CPU:
		printf("soy cpu \n");

		break;

	case MOSTRAR_COMANDOS: {

		mostrarComandos();

	}
		break;

	case -1:
		printf("--Ojo ese comando no existe!! proba con mostrarComandos \n");

	}
}

void levantarConsola() {
	char comando[50];
	int idFunc;

	mostrarComandos();

	while (1) {
		printf("Ingrese un comando >> ");
//		fgets(comando, 50, stdin);
		scanf("%s", comando);
		idFunc = idFuncion(comando);
		aplicarFuncion(idFunc);
	}

}
