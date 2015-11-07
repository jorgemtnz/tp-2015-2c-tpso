/*
 * funcionesConsola.c
 *
 *  Created on: 5/11/2015
 *      Author: utnso
 */
#include "Memoria.h"

void levantarConsola() {
	char comando[20];
	int idFunc;

	mostrarComandos();
	while (1) {
		puts("Ingrese un comando >> ");
		fgets(comando, 20 ,stdin);
		//scanf("%s", comando);
		idFunc = idFuncion(comando);
		aplicarFuncion(idFunc);
	}
}

int idFuncion(char* funcion) {
	int i;
	char* funcionesConsola[] = {  "TLB_FLUSH", "MEM_FLUSH", "MEM_DUMP"};
	for (i=0;(i < 3) && (strcmp(funcion, funcionesConsola[i]) != 0); i++);
		return (i <= 3 - 1) ? (i + 1) : -1;
}

void aplicarFuncion(int idFuncion) {
	enum nomFun {
		TLB_FLUSH = 1, MEM_FLUSH, MEM_DUMP, MOSTRAR_COMANDOS
	};
	switch (idFuncion) {
	case TLB_FLUSH: {
		puts("Limpiar la TLB\n");
		break;
	}
	case MEM_FLUSH:
		puts("Limpiar la Memoria Principal\n");
		break;

	case MEM_DUMP:
		puts("Volcado de la Memoria a un Log\n");
		break;

	case MOSTRAR_COMANDOS: {
		mostrarComandos();
	}
		break;
	case -1:
		puts("--Ojo ese comando no existe!! proba con mostrarComandos \n");
	}
}

void mostrarComandos() {
	char* funcionesConsola[] = { "TLB_FLUSH", "MEM_FLUSH", "MEM_DUMP"};

	char* descripcionesConsola[] = { "Limpiar la TLB",
			"Limpiar la Memoria Principal", "Volcado de la Memoria a un Log"};

	int contador = 0;
	while (contador <= 2) {
		printf("*------------------------------------------*\n");
		printf("COMANDO 	= %s\n", funcionesConsola[contador]);
		printf("DESCRIPCION = %s\n", descripcionesConsola[contador]);
		printf("*------------------------------------------*\n");
		contador += 1;
	}
}

int procesarMensajesConsola(int socket, t_header* header, char* buffer) {
	if(buffer != NULL && strstr(buffer, "\n")) {
		if(string_starts_with(buffer, "\n")){
			buffer = "";
		} else {
			char** split = string_split(buffer, "\n");
			buffer = split[0];
		}
	}
	if(string_equals(buffer, "TLB_FLUSH")) {
		puts("Limpiar TLB");
		//limpiarTLB();
	} else if(string_equals(buffer, "MEM_FLUSH")) {
		puts("Limpiar Memoria");
		//limpiarMemoria();
	} else if(string_equals(buffer, "MEM_DUMP")) {
		puts("Volcar Memoria");
		//volcarMemoria();
	} else {
		puts("Comando no reconocido");
	}
	return 0;
}


