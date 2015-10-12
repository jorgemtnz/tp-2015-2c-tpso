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

static void test_iniciar_4_procesos_en_memoria(){
	inicializacionDesdeCero();
	int PID1= 1, PID2= 2, PID3= 3, PID4= 4;
	int cant1= 4, cant2= 5, cant3= 6, cant4= 7;
	int socketMentiroso = 7;
	int tamanioFinalTablaDePag;
	iniciar(PID1, cant1, socketMentiroso);
	iniciar(PID2, cant2, socketMentiroso);
	iniciar(PID3, cant3, socketMentiroso);
	iniciar(PID4, cant4, socketMentiroso);


	tamanioFinalTablaDePag = list_size(listaTablaDePag);

	CU_ASSERT_EQUAL(tamanioFinalTablaDePag, 4);
}

static CU_TestInfo tests[] = {
	{ "Test Hola Mundo", test_debe_devolver_hola_mundo }, {"Test iniciar 4 procesos en memoria", test_iniciar_4_procesos_en_memoria},
	CU_TEST_INFO_NULL,
};

CUNIT_MAKE_SUITE(memoria, "Test Memoria", init_suite, clean_suite, tests)
