/*
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

static void test_iniciar_4_procesos_con_22_paginas_en_memoria() {
	inicializacionDesdeCero();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int cant1 = 4, cant2 = 5, cant3 = 6, cant4 = 7;
	int socketMentiroso = 7;
	int tamanioFinalTablaDePag, a;
	int contadorCantProc1 = 0, contadorCantProc2 = 0, contadorCantProc3 = 0, contadorCantProc4 = 0;
	int contadorMarcoNegativo = 0;
	iniciar(PID1, cant1, socketMentiroso);
	iniciar(PID2, cant2, socketMentiroso);
	iniciar(PID3, cant3, socketMentiroso);
	iniciar(PID4, cant4, socketMentiroso);

	tamanioFinalTablaDePag = list_size(listaTablaDePag);

	for (a = 0; a < tamanioFinalTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		if (campoTablaDePag->idMarco == -1)
			contadorMarcoNegativo++;

		switch (campoTablaDePag->idProc) {
		case (1): {
			contadorCantProc1++;
			break;
		}
		case (2): {
			contadorCantProc2++;
			break;
		}
		case (3): {
			contadorCantProc3++;
			break;
		}
		case (4): {
			contadorCantProc4++;
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

	for(a=0;a<4;a++){
	campoTablaDePag = list_get(listaTablaDePag, a);
	CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
	CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID1);
	CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
	CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
	}

	for(a=0;a<5;a++){
	campoTablaDePag = list_get(listaTablaDePag, a+4);
	CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
	CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID2);
	CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
	CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
	}

	for(a=0;a<6;a++){
	campoTablaDePag = list_get(listaTablaDePag, a+9);
	CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
	CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID3);
	CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
	CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
	}

	for(a=0;a<7;a++){
	campoTablaDePag = list_get(listaTablaDePag, a+15);
	CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
	CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID4);
	CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
	CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
	}

}

static void test_probar_buscar_si_esta_en_memoria_sin_TLB(){
	iniciarConfiguracionTLBNoHabilitada();
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	// int cant1 = 4, cant2 = 5, cant3 = 6, cant4 = 7;

	int pag1 = 1, pag2 = 5, pag3 = 9, pag4 = 14,pag5 = 18;
	hardcodearTablaDePaginasYMarcoMemoria(pag1,pag2,pag3,pag4,pag5);

	int idMarco1,idMarco2,idMarco3,idMarco4,idMarco5,idMarco6;


	idMarco1 = buscarSiEstaEnMemoria(PID1,1);
	idMarco2 = buscarSiEstaEnMemoria(PID2,1);
	idMarco3 = buscarSiEstaEnMemoria(PID3,0);
	idMarco4 = buscarSiEstaEnMemoria(PID3,5);
	idMarco5 = buscarSiEstaEnMemoria(PID4,3);
	idMarco6 = buscarSiEstaEnMemoria(PID1,-1);

	CU_ASSERT_EQUAL(idMarco1,455);
	CU_ASSERT_EQUAL(idMarco2,456);
	CU_ASSERT_EQUAL(idMarco3,457);
	CU_ASSERT_EQUAL(idMarco4,458);
	CU_ASSERT_EQUAL(idMarco5,459);
	CU_ASSERT_EQUAL(idMarco6,-1);

}

static void escribir_en_marco_y_poner_bit_de_modificada(){
	int tamanioMemoria,a;
	tamanioMemoria= list_size(listaMemoria);
	int idMarco1 = 455, idMarco2 = 456, idMarco3 = 457, idMarco4 = 458, idMarco5 = 459;
	char* contenido1;
	char* contenido2;
	char* contenido3;
	char* contenido3Bis;
	char* contenido4;
	contenido1 = "escritura1";
	contenido2 = "escritura2";
	contenido3 = "escritura3";
	contenido3Bis = "escritura3Bis";
	contenido4 = "escritura4";
	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();


	escribirEnMarcoYponerBitDeModificada(idMarco1,contenido1);
	escribirEnMarcoYponerBitDeModificada(idMarco2,contenido2);
	escribirEnMarcoYponerBitDeModificada(idMarco3,contenido3);
	escribirEnMarcoYponerBitDeModificada(idMarco4,contenido3Bis);
	escribirEnMarcoYponerBitDeModificada(idMarco5,contenido4);

	campoMemoria = list_get(listaMemoria, 0);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido1);
	campoMemoria = list_get(listaMemoria, 1);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido2);
	campoMemoria = list_get(listaMemoria, 2);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3);
	campoMemoria = list_get(listaMemoria, 3);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3Bis);
	campoMemoria = list_get(listaMemoria, 4);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido4);

}

static void test_probar_escribir_memoria_sin_TLB(){
	inicializacionDesdeCero();
	iniciarConfiguracionTLBNoHabilitada();

	int pag1 = 1, pag2 = 5, pag3 = 9, pag4 = 14,pag5 = 18;
	hardcodearTablaDePaginasYMarcoMemoria(pag1,pag2,pag3,pag4,pag5);

	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7;
	int a = 1;
	char* contenido1;
	char* contenido2;
	char* contenido3;
	char* contenido3Bis;
	char* contenido4;
	contenido1 = "escritura1";
	contenido2 = "escritura2";
	contenido3 = "escritura3";
	contenido3Bis = "escritura3Bis";
	contenido4 = "escritura4";
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();

	escribir(PID1, pag1,contenido1, socketMentiroso);
	escribir(PID2, pag2,contenido2, socketMentiroso);
	escribir(PID3, pag3,contenido3, socketMentiroso);
	escribir(PID3, pag4,contenido3Bis, socketMentiroso);
	escribir(PID4, pag5,contenido4, socketMentiroso);

	for (a = 0; a < 4; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);

		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID1);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(pag1 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		} else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 455);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for (a = 0; a < 5; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a + 4);
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID2);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(pag2 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 456);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for (a = 0; a < 6; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a + 9);
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID3);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(pag3 != a && pag4 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		} else if ( pag3 != a){
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 457);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 458);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for (a = 0; a < 7; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a + 15);
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID4);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(pag5 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, -1);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		}else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 459);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

}


static CU_TestInfo tests[] = {
	{ "Test Hola Mundo", test_debe_devolver_hola_mundo }, {"Test iniciar 4 procesos con 22 paginas en memoria", test_iniciar_4_procesos_con_22_paginas_en_memoria},{"Test busca 6 marcos y uno no encuentra", test_probar_buscar_si_esta_en_memoria_sin_TLB},
	{"Test escribe 5 contenidos en memoria",escribir_en_marco_y_poner_bit_de_modificada},{"Test probar escribir en memoria",test_probar_escribir_memoria_sin_TLB},
	CU_TEST_INFO_NULL,
};

CUNIT_MAKE_SUITE(memoria, "Test Memoria", init_suite, clean_suite, tests)
