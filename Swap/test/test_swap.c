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

	/*configuracion->puertoEscucha = 4;
	configuracion->nombreSwap = "swap.data";
	configuracion->cantidadPaginas = 54;
	configuracion->tamanioPagina = 12;
	configuracion->retardo_compactacion = 2;
	configuracion->retardo_swap = 2;*/
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
	CU_ASSERT_NOT_EQUAL(respuesta->PID, 34);

}


static CU_TestInfo tests[] = {
	{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
	{ "Test Iniciar UN proceso", test_iniciar_un_proceso },
	CU_TEST_INFO_NULL,
};

CUNIT_MAKE_SUITE(swap, "Test swap", init_suite, clean_suite, tests)
