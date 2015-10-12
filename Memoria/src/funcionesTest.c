/*
 * funcionesTest.c
 *
 *  Created on: 12/10/2015
 *      Author: utnso
 */
#include "Memoria.h"

void inicializacionDesdeCero(){
	listaMemoria = list_create();
	listaTLB = list_create();
	listaTablaDePag = list_create();
	contadorPagTP = 0;
	variableIdMarco = 0;
}

