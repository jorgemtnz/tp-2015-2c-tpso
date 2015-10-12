/*
 * test_suite1.c
 *
 *  Created on: 12/4/2015
 *      Author: Alejandro Zalazar
 */

#include <string.h> /* memset */
#include <unistd.h> /* close */

#include "../src/Swap.h"

static int init_suite() {
	return 0;
}

static int clean_suite() {
	return 0;
}

static void test_debe_devolver_hola_mundo() {
	CU_ASSERT_STRING_EQUAL("Hola Mundo", decirHolaMundo());
}


static void test_iniciar_un_proceso(){


	t_iniciar_swap* estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
	t_list* listaDeEspaciosLibres;
	t_list* listaDeProcesosCargados;
	listaDeEspaciosLibres = list_create();
	listaDeProcesosCargados = list_create();
	estructuraIniciar->PID = 3;
	estructuraIniciar->cantidadPaginas = 30;
	t_respuesta_iniciar_o_finalizar* respuesta;
	respuesta = crearDevolucionIniciarOFinalizar();
	respuesta = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
	CU_ASSERT_EQUAL(respuesta->PID, 3);

}

static void test_escribir_en_proceso(){
	t_iniciar_swap* estructuraIniciar;
		estructuraIniciar = crearEstructuraIniciar();
		t_list* listaDeEspaciosLibres;
		t_list* listaDeProcesosCargados;
		listaDeEspaciosLibres = list_create();
		listaDeProcesosCargados = list_create();
		estructuraIniciar->PID = 3;
		estructuraIniciar->cantidadPaginas = 30;
		t_respuesta_iniciar_o_finalizar* respuesta;
		respuesta = crearDevolucionIniciarOFinalizar();
		respuesta = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
		t_contenido_pagina* procesoAEscribir;
		procesoAEscribir = crearContenidoPagina();
		procesoAEscribir->PID = 3;
		procesoAEscribir->contenido = "PROBANDO ESCRITURA EN EL PROCESO";
		procesoAEscribir->numeroPagina = 5;
		t_devolucion_escribir_o_leer* resultadoEscribir;
		 resultadoEscribir = crearDevolucionEscribirOLeer();
		resultadoEscribir = escribir(listaDeProcesosCargados, procesoAEscribir);
		CU_ASSERT_EQUAL(resultadoEscribir->contenido, "PROBANDO ESCRITURA EN EL PROCESO");
		CU_ASSERT_EQUAL(resultadoEscribir->PID, 3);
		CU_ASSERT_EQUAL(resultadoEscribir->numeroPagina, 5);
		CU_ASSERT_EQUAL(resultadoEscribir->resultado, OK);
}


static CU_TestInfo tests[] = {
	{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
	{ "Test Iniciar UN proceso", test_iniciar_un_proceso },
	{ "Test Escribir en proceso",test_escribir_en_proceso},
	CU_TEST_INFO_NULL,
};

CUNIT_MAKE_SUITE(swap, "Test swap", init_suite, clean_suite, tests)
