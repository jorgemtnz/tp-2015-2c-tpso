/*
 * test_suite1.c
 *
 *  Created on: 12/4/2015
 *      Author: Alejandro Zalazar
 */

#include <string.h> /* memset */
#include <unistd.h> /* close */

#include "../src/Memoria.h"

static int init_suite() {
	return 0;
}

static int clean_suite() {
	return 0;
}

static void test_debe_devolver_hola_mundo() {
	CU_ASSERT_STRING_EQUAL("Hola Mundo", decirHolaMundo());
}

static void test_iniciar_4_procesos_con_22_paginas_en_memoria(){
	inicializacionDesdeCero();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	pid_t PID1= 1, PID2= 2, PID3= 3, PID4= 4;
	int cant1= 4, cant2= 5, cant3= 6, cant4= 7;
	int socketMentiroso = 7;
	int tamanioFinalTablaDePag,a;
	int contadorCantProc1 = 0,contadorCantProc2 =0,contadorCantProc3= 0, contadorCantProc4= 0;
	int contadorMarcoNegativo = 0;
	iniciar(PID1, cant1, socketMentiroso);
	iniciar(PID2, cant2, socketMentiroso);
	iniciar(PID3, cant3, socketMentiroso);
	iniciar(PID4, cant4, socketMentiroso);


	tamanioFinalTablaDePag = list_size(listaTablaDePag);

	for (a = 0; a < tamanioFinalTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if(campoTablaDePag->idMarco == -1) contadorMarcoNegativo ++;

		switch (campoTablaDePag->idProc) {
		case (1): {
			contadorCantProc1 ++;
			break;
		}
		case (2): {
			contadorCantProc2 ++;
			break;
		}
		case (3): {
			contadorCantProc3 ++;
			break;
		}
		case (4): {
			contadorCantProc4 ++;
			break;
		}
		}
	}

	CU_ASSERT_EQUAL(tamanioFinalTablaDePag, 22);
	CU_ASSERT_EQUAL(contadorMarcoNegativo, 22);

	CU_ASSERT_EQUAL(contadorCantProc1, 4);
	CU_ASSERT_EQUAL(contadorCantProc2, 5);
	CU_ASSERT_EQUAL(contadorCantProc3, 6);
	CU_ASSERT_EQUAL(contadorCantProc4, 7);
}

static void test_probar_memoria_sin_TLB(){
	inicializacionDesdeCero();
}

static CU_TestInfo tests[] = {
	{ "Test Hola Mundo", test_debe_devolver_hola_mundo }, {"Test iniciar 4 procesos con 22 paginas en memoria", test_iniciar_4_procesos_con_22_paginas_en_memoria},
	CU_TEST_INFO_NULL,
};

CUNIT_MAKE_SUITE(memoria, "Test Memoria", init_suite, clean_suite, tests)
