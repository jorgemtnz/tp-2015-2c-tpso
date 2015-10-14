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

static void test_iniciar_un_proceso() {

	t_iniciar_swap* estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 3;
	estructuraIniciar->cantidadPaginas = 30;
	t_respuesta_iniciar_o_finalizar* respuesta;
	respuesta = crearDevolucionIniciarOFinalizar();
	respuesta = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
	CU_ASSERT_EQUAL(respuesta->PID, 3);

}

static void test_escribir_en_proceso() {
	t_iniciar_swap* estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
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
	CU_ASSERT_STRING_EQUAL(resultadoEscribir->contenido, "PROBANDO ESCRITURA EN EL PROCESO");
	CU_ASSERT_EQUAL(resultadoEscribir->PID, 3);
	CU_ASSERT_EQUAL(resultadoEscribir->numeroPagina, 5);
	CU_ASSERT_EQUAL(resultadoEscribir->resultado, OK);
}

static void test_leer_de_proceso() {
	t_iniciar_swap* estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
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
	t_devolucion_escribir_o_leer* resultadoLeer;
	resultadoLeer = crearDevolucionEscribirOLeer();
	escribir(listaDeProcesosCargados, procesoAEscribir);
	t_leerDeProceso *procesoRecibido;
	procesoRecibido = crearLeerDeProceso();
	procesoRecibido->PID = 3;
	procesoRecibido->numeroPaginaInicio = 5;
	procesoRecibido->numeroPaginaFin = 5;
	resultadoLeer = leer(procesoRecibido, listaDeProcesosCargados);
	CU_ASSERT_STRING_EQUAL(resultadoLeer->contenido, "PROBANDO ESCRITURA EN EL PROCESO");
	CU_ASSERT_EQUAL(resultadoLeer->PID, 3);
	CU_ASSERT_EQUAL(resultadoLeer->numeroPagina, 5);
	CU_ASSERT_EQUAL(resultadoLeer->resultado, OK);
}

static void test_finalizar_un_proceso() {
	t_iniciar_swap* estructuraIniciar;
	inicializarListas();
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 3;
	estructuraIniciar->cantidadPaginas = 3;
	t_respuesta_iniciar_o_finalizar* respuesta;
	respuesta = crearDevolucionIniciarOFinalizar();
	iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	respuesta = finalizar(3, listaDeProcesosCargados, listaDeEspaciosLibres);
	CU_ASSERT_EQUAL(respuesta->PID, 3);
	CU_ASSERT_EQUAL(respuesta->resultado, OK);

}

static void test_iniciar_proceso_que_no_entra() {
	t_iniciar_swap* estructuraIniciar;
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 3;
	estructuraIniciar->cantidadPaginas = 520;
	t_respuesta_iniciar_o_finalizar* respuesta;
	respuesta = crearDevolucionIniciarOFinalizar();
	respuesta = iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);
	CU_ASSERT_EQUAL(respuesta->PID, 3);
	CU_ASSERT_EQUAL(respuesta->resultado, ERROR);
}

static void test_iniciar_y_finalizar_varios_procesos() {
	t_iniciar_swap* estructuraIniciar;
	inicializarListas();
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 3;
	estructuraIniciar->cantidadPaginas = 6;

	iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar2;
	estructuraIniciar2 = crearEstructuraIniciar();
	estructuraIniciar2->PID = 1;
	estructuraIniciar2->cantidadPaginas = 4;

	iniciar(estructuraIniciar2, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar3;
	estructuraIniciar3 = crearEstructuraIniciar();
	estructuraIniciar3->PID = 18;
	estructuraIniciar3->cantidadPaginas = 15;

	iniciar(estructuraIniciar3, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar4;
	estructuraIniciar4 = crearEstructuraIniciar();
	estructuraIniciar4->PID = 0;
	estructuraIniciar4->cantidadPaginas = 8;

	iniciar(estructuraIniciar4, listaDeEspaciosLibres, listaDeProcesosCargados);

	finalizar(1, listaDeProcesosCargados, listaDeEspaciosLibres);

	t_iniciar_swap* estructuraIniciar5;
	estructuraIniciar5 = crearEstructuraIniciar();
	estructuraIniciar5->PID = 10;
	estructuraIniciar5->cantidadPaginas = 2;

	iniciar(estructuraIniciar5, listaDeEspaciosLibres, listaDeProcesosCargados);

	finalizar(3, listaDeProcesosCargados, listaDeEspaciosLibres);

	t_iniciar_swap* estructuraIniciar6;
	estructuraIniciar6 = crearEstructuraIniciar();
	estructuraIniciar6->PID = 13;
	estructuraIniciar6->cantidadPaginas = 5;

	iniciar(estructuraIniciar6, listaDeEspaciosLibres, listaDeProcesosCargados);

	int a;
	l_procesosCargados* proceso;
	l_espacioLibre* espacioLibre;
	proceso = crearProceso();
	espacioLibre = crearEspacioLibre();

	a = 0;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 18);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 15);
	CU_ASSERT_EQUAL(proceso->ubicacion, 10);

	a = 1;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 0);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 8);
	CU_ASSERT_EQUAL(proceso->ubicacion, 25);

	a = 2;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 10);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 2);
	CU_ASSERT_EQUAL(proceso->ubicacion, 6);

	a = 3;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 13);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 5);
	CU_ASSERT_EQUAL(proceso->ubicacion, 0);

	a = 0;
	espacioLibre = list_get(listaDeEspaciosLibres, a);
	CU_ASSERT_EQUAL(espacioLibre->ubicacion, 5);
	CU_ASSERT_EQUAL(espacioLibre->cantPagsLibres, 1);

	a = 1;
	espacioLibre = list_get(listaDeEspaciosLibres, a);
	CU_ASSERT_EQUAL(espacioLibre->ubicacion, 8);
	CU_ASSERT_EQUAL(espacioLibre->cantPagsLibres, 2);

	a = 2;
	espacioLibre = list_get(listaDeEspaciosLibres, a);
	CU_ASSERT_EQUAL(espacioLibre->ubicacion, 33);
	CU_ASSERT_EQUAL(espacioLibre->cantPagsLibres, configuracion->cantidadPaginas - 33);

}

static void test_acomodar_espacios_libres() {
	t_iniciar_swap* estructuraIniciar;
	inicializarListas();
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 0;
	estructuraIniciar->cantidadPaginas = 6;

	iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar2;
	estructuraIniciar2 = crearEstructuraIniciar();
	estructuraIniciar2->PID = 1;
	estructuraIniciar2->cantidadPaginas = 2;

	iniciar(estructuraIniciar2, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar3;
	estructuraIniciar3 = crearEstructuraIniciar();
	estructuraIniciar3->PID = 18;
	estructuraIniciar3->cantidadPaginas = 10;

	iniciar(estructuraIniciar3, listaDeEspaciosLibres, listaDeProcesosCargados);

	finalizar(0, listaDeProcesosCargados, listaDeEspaciosLibres);
	finalizar(1, listaDeProcesosCargados, listaDeEspaciosLibres);

	t_iniciar_swap* estructuraIniciar4;
	estructuraIniciar4 = crearEstructuraIniciar();
	estructuraIniciar4->PID = 5;
	estructuraIniciar4->cantidadPaginas = 7;

	iniciar(estructuraIniciar4, listaDeEspaciosLibres, listaDeProcesosCargados);

	int a;
	l_procesosCargados* proceso;
	l_espacioLibre* espacioLibre;
	proceso = crearProceso();
	espacioLibre = crearEspacioLibre();

	a = 0;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 18);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 10);
	CU_ASSERT_EQUAL(proceso->ubicacion, 8);

	a = 1;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 5);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 7);
	CU_ASSERT_EQUAL(proceso->ubicacion, 0);

	a = 0;
	espacioLibre = list_get(listaDeEspaciosLibres, a);
	CU_ASSERT_EQUAL(espacioLibre->ubicacion, 7);
	CU_ASSERT_EQUAL(espacioLibre->cantPagsLibres, 1);

	a = 1;
	espacioLibre = list_get(listaDeEspaciosLibres, a);
	CU_ASSERT_EQUAL(espacioLibre->ubicacion, 18);
	CU_ASSERT_EQUAL(espacioLibre->cantPagsLibres, configuracion->cantidadPaginas - 18);

}

static void test_algoritmo_compactacion() {
	t_iniciar_swap* estructuraIniciar;
	inicializarListas();
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 0;
	estructuraIniciar->cantidadPaginas = 6;

	iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar2;
	estructuraIniciar2 = crearEstructuraIniciar();
	estructuraIniciar2->PID = 1;
	estructuraIniciar2->cantidadPaginas = 10;

	iniciar(estructuraIniciar2, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar3;
	estructuraIniciar3 = crearEstructuraIniciar();
	estructuraIniciar3->PID = 18;
	estructuraIniciar3->cantidadPaginas = 6;

	iniciar(estructuraIniciar3, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_iniciar_swap* estructuraIniciar5;
	estructuraIniciar5 = crearEstructuraIniciar();
	estructuraIniciar5->PID = 8;
	estructuraIniciar5->cantidadPaginas = 28;

	iniciar(estructuraIniciar5, listaDeEspaciosLibres, listaDeProcesosCargados);

	finalizar(0, listaDeProcesosCargados, listaDeEspaciosLibres);
	finalizar(18, listaDeProcesosCargados, listaDeEspaciosLibres);

	t_iniciar_swap* estructuraIniciar4;
	estructuraIniciar4 = crearEstructuraIniciar();
	estructuraIniciar4->PID = 5;
	estructuraIniciar4->cantidadPaginas = 10;

	iniciar(estructuraIniciar4, listaDeEspaciosLibres, listaDeProcesosCargados);

	int a;
	l_procesosCargados* proceso;
	l_espacioLibre* espacioLibre;
	proceso = crearProceso();
	espacioLibre = crearEspacioLibre();

	a = 0;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 1);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 10);
	CU_ASSERT_EQUAL(proceso->ubicacion, 0);

	a = 1;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 8);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 28);
	CU_ASSERT_EQUAL(proceso->ubicacion, 10);

	a = 2;
	proceso = list_get(listaDeProcesosCargados, a);

	CU_ASSERT_EQUAL(proceso->PID, 5);
	CU_ASSERT_EQUAL(proceso->cantPagsUso, 10);
	CU_ASSERT_EQUAL(proceso->ubicacion, 38);

	a = 0;
	espacioLibre = list_get(listaDeEspaciosLibres, a);
	CU_ASSERT_EQUAL(espacioLibre->ubicacion, 48);
	CU_ASSERT_EQUAL(espacioLibre->cantPagsLibres, configuracion->cantidadPaginas - 48);

}

static void test_corto_cod() {
	t_iniciar_swap* estructuraIniciar;
	inicializarListas();
	estructuraIniciar = crearEstructuraIniciar();
	estructuraIniciar->PID = 2;
	estructuraIniciar->cantidadPaginas = 5;
	t_respuesta_iniciar_o_finalizar* respuesta;
	respuesta = crearDevolucionIniciarOFinalizar();
	iniciar(estructuraIniciar, listaDeEspaciosLibres, listaDeProcesosCargados);

	t_devolucion_escribir_o_leer* resultadoLeer1;
	resultadoLeer1 = crearDevolucionEscribirOLeer();
	t_devolucion_escribir_o_leer* resultadoLeer2;
	resultadoLeer2 = crearDevolucionEscribirOLeer();
	t_leerDeProceso *procesoRecibido;
	procesoRecibido = crearLeerDeProceso();
	procesoRecibido->PID = 2;
	procesoRecibido->numeroPaginaInicio = 0;
	procesoRecibido->numeroPaginaFin = 0;

	resultadoLeer1 = leer(procesoRecibido, listaDeProcesosCargados);

	procesoRecibido->PID = 2;
	procesoRecibido->numeroPaginaInicio = 1;
	procesoRecibido->numeroPaginaFin = 1;

	resultadoLeer2 = leer(procesoRecibido, listaDeProcesosCargados);

	respuesta = finalizar(2, listaDeProcesosCargados, listaDeEspaciosLibres);

	CU_ASSERT_EQUAL(respuesta->PID, 2);
	CU_ASSERT_STRING_EQUAL(resultadoLeer2->contenido, "\0");
	CU_ASSERT_STRING_EQUAL(resultadoLeer1->contenido, "\0");

}

static CU_TestInfo tests[] = { { "Test Hola Mundo", test_debe_devolver_hola_mundo }, { "Test Iniciar UN proceso", test_iniciar_un_proceso }, {
		"Test Escribir en proceso", test_escribir_en_proceso }, { "Test Leer de proceso", test_leer_de_proceso }, { "Test Finalizar un proceso",
		test_finalizar_un_proceso }, { "Test Proceso que no entra en Swap ", test_iniciar_proceso_que_no_entra }, { "Test Iniciar y finalizar varios procesos ",
		test_iniciar_y_finalizar_varios_procesos }, { "Test acomodar espacios libres ", test_acomodar_espacios_libres }, { "Test compactacion ",
		test_algoritmo_compactacion }, { "Test corto cod ", test_corto_cod },
CU_TEST_INFO_NULL, };

CUNIT_MAKE_SUITE(swap, "Test swap", init_suite, clean_suite, tests)
