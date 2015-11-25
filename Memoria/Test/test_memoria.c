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
	int a_rgc = 3;
	char* temp = string_new();
	string_append(&temp,
			"./Memoria;/home/utnso/tp-2015-2c-tpso/Memoria/config_memoria.cfg;test");

	char** a_rgv = string_split(temp, ";");
	a_rgc = 3;
	leerArchivoDeConfiguracion(a_rgc, a_rgv);
	inicializacionDesdeCero();
	return 0;
}

static int clean_suite() {
	return 0;
}






static void test_debe_devolver_hola_mundo() {
	CU_ASSERT_STRING_EQUAL("Hola Mundo", decirHolaMundo());
}

static void test_variables_configuracion(){
	CU_ASSERT_STRING_EQUAL(configuracion->algoritmo_reemplazo, "LRU");
}

static void test_iniciar_4_procesos_con_22_paginas_en_memoria() {
	inicializacionDesdeCero();
	iniciarConfiguracionTLBHabilitada();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_PID* campoIniciar;
	campoIniciar = crearPID();
	t_PID* campoIniciar1;
	campoIniciar1 = crearPID();
	t_PID* campoIniciar2;
	campoIniciar2 = crearPID();
	t_PID* campoIniciar3;
	campoIniciar3 = crearPID();

	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int cant1 = 4, cant2 = 5, cant3 = 6, cant4 = 7, contadorVariableIdMarco = 0;
	int socketMentiroso = 7;
	int tamanioFinalTablaDePag, a;
	int contadorCantProc1 = 0, contadorCantProc2 = 0, contadorCantProc3 = 0, contadorCantProc4 = 0;
	campoIniciar = iniciar_falso(PID1, cant1, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar->PID, PID1);
	campoIniciar1 = iniciar_falso(PID2, cant2, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar1->PID, PID2);
	campoIniciar2 = iniciar_falso(PID3, cant3, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar2->PID, PID3);
	campoIniciar3 = iniciar_falso(PID4, cant4, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar3->PID, PID4);

	tamanioFinalTablaDePag = list_size(listaTablaDePag);

	for (a = 0; a < tamanioFinalTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);

		switch (campoTablaDePag->idProc) {
		case (1): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 1);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc1);

			contadorCantProc1++;
			break;
		}
		case (2): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 2);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc2);
			contadorCantProc2++;

			break;
		}
		case (3): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 3);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc3);
			contadorCantProc3++;

			break;
		}
		case (4): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 4);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc4);
			contadorCantProc4++;

			break;
		}


		}
		contadorVariableIdMarco++;
	}

	CU_ASSERT_EQUAL(tamanioFinalTablaDePag, 22);


}

static void test_probar_escribir_memoria_con_hardcodeo_de_TLB(){
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7;
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
//	t_TablaDePaginas * campoTablaDePag;
//	campoTablaDePag = iniciarTablaDePaginas();
//	t_marco * campoMemoria;
//	campoMemoria = iniciarMarco();
//	t_contenido_pagina* campoEscribir;
//	//warning diferentes estructuras, entonces corrijo, son similares, pero importa el orden
//	campoEscribir = iniciarContenidoPagina();
	t_escribir_falso* estructuraDevolucionEscribirFalso;
	estructuraDevolucionEscribirFalso = crearEscribirFalso();

	hardcodearValoresEnTLB(PID1,1,1);
	hardcodearValoresEnTLB(PID2,5,1);
	hardcodearValoresEnTLB(PID3,9,0);


	estructuraDevolucionEscribirFalso=escribir_falso(PID1, 1,contenido1, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID2, 1,contenido2, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID2);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 + 1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID3, 0,contenido3, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4+5+0);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 0);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID3, 5,contenido3Bis, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 +5+5);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 5);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID4, 3,contenido4, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 +5+6+3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID4, 4,contenido4, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 +5+6+4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);



}

static void testRespuestaTraerDeSwapUnaPaginaDeUnProcesoPrueba() {
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7, flagEscritura = 1, flagNoEscritura = 0;
	int tamanioMemoria;
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
//	t_TablaDePaginas * campoTablaDePag;
//	campoTablaDePag = iniciarTablaDePaginas();
	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();
	t_contenido_pagina* campoEscribir;
	campoEscribir = iniciarContenidoPagina();

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID1, 1, contenido1, flagEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID1);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido1);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID2, 1, contenido2, flagEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID2);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido2);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID3, 0, contenido3, flagNoEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID3);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 0);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido3);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID4, 2, contenido4, flagNoEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID4);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 2);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido4);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID4, 3, contenido3Bis, flagEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID4);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 3);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido3Bis);



	tamanioMemoria = list_size(listaMemoria);

	CU_ASSERT_EQUAL(tamanioMemoria, 5);

	campoMemoria = list_get(listaMemoria, 0);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 1);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 1);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, "escritura1");

	campoMemoria = list_get(listaMemoria, 1);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+1);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 2);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido2);

	campoMemoria = list_get(listaMemoria, 2);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+5+0);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 3);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3);

	campoMemoria = list_get(listaMemoria, 3);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+5+6+2);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 4);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido4);

	campoMemoria = list_get(listaMemoria, 4);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+5+6+3);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 5);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3Bis);



}

static void test_probar_leer_falso() {
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7;
	t_contenido_pagina* campoLeer;
	campoLeer = iniciarContenidoPagina();
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

	campoLeer = leer_falso(PID1, 1, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID1);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido1);

	campoLeer = leer_falso(PID2, 1, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID2);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido2);

	campoLeer = leer_falso(PID3, 0, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID3);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 0);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido3);

	campoLeer = leer_falso(PID4, 3, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID4);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 3);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido3Bis);

	campoLeer = leer_falso(PID4, 0, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, 11);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 11);

	campoLeer = leer_falso(PID4, 2, socketMentiroso, socketMentiroso);



	CU_ASSERT_EQUAL(campoLeer->PID, 4);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 2);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido4);

	campoLeer = leer_falso(PID1, 4, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, 11);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 11);

}


static void test_probar_finalizar_un_proceso_con_TLB() {
	t_PID* id1;
	id1 = crearPID();
	t_PID* id2;
	id2 = crearPID();
	t_PID* id3;
	id3 = crearPID();
	t_PID* id4;
	id4 = crearPID();

	t_PID* PID1;
	PID1 = crearPID();
	PID1->PID = 1;
	t_PID* PID2;
	PID2 = crearPID();
	PID2->PID = 2;
	t_PID* PID3;
	PID3 = crearPID();
	PID3->PID = 3;
	t_PID* PID4;
	PID4 = crearPID();
	PID4->PID = 4;

	int tamanioTablaDePag, tamanioMemoria,tamanioTLB, socketFalso = 8;


	id1 = finalizar_falso(PID1, socketFalso);

	id2 = finalizar_falso(PID2, socketFalso);

	id3 = finalizar_falso(PID3, socketFalso);

	id4 = finalizar_falso(PID4, socketFalso);



	CU_ASSERT_EQUAL(id1->PID, 1);
	CU_ASSERT_EQUAL(id2->PID, 2);
	CU_ASSERT_EQUAL(id3->PID, 3);
	CU_ASSERT_EQUAL(id4->PID, 4);

	tamanioTablaDePag = list_size(listaTablaDePag);
	tamanioMemoria = list_size(listaMemoria);
	tamanioTLB = list_size(listaTLB);



	CU_ASSERT_EQUAL(tamanioTablaDePag, 0);
	CU_ASSERT_EQUAL(tamanioMemoria, 0);
	CU_ASSERT_EQUAL(tamanioTLB,0);
}

static void traer_falso_a_fondo_con_Clock() {
	int tamanioMemoria,a=0;
	tamanioMemoria = list_size(listaMemoria);

	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();
	t_contenido_pagina* campoEscribir;
	campoEscribir = iniciarContenidoPagina();

	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->bitModificada = 1;
	campoMemoria->bitUso = 0;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->bitModificada = 1;
	campoMemoria->bitUso = 0;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->bitModificada = 1;
	campoMemoria->bitUso = 0;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->bitModificada = 1;
	campoMemoria->bitUso = 0;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->bitModificada = 1;
	campoMemoria->bitUso = 0;
	a++;

	configuracion->cantidadMarcos= 5;
	printf("\n%i\n",tamanioMemoria);
	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalsoFalso(4, 4,"escritura4", 1, 54,45);

	printf("\n%i\n",campoEscribir->PID);
	printf("\n%s\n",campoEscribir->contenido);


}

static CU_TestInfo tests[] = {
		{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
		{"Test carga archivo configuracion lee variables",test_variables_configuracion },
		{"Test inicar proceso",test_iniciar_4_procesos_con_22_paginas_en_memoria},
		{"Test escribir con TLB",test_probar_escribir_memoria_con_hardcodeo_de_TLB},
		{"Test probar traer de swap una pagina", testRespuestaTraerDeSwapUnaPaginaDeUnProcesoPrueba},
		{"Test traer con clock",traer_falso_a_fondo_con_Clock},
		{"Test probar leer",test_probar_leer_falso},
		{"Test probar finalizar los procesos",test_probar_finalizar_un_proceso_con_TLB},
		CU_TEST_INFO_NULL,
};

/*


static void test_iniciar_4_procesos_con_22_paginas_en_memoria() {
	inicializacionDesdeCero();
	t_TablaDePaginas* campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();
	t_PID* campoIniciar;
	campoIniciar = crearPID();
	t_PID* campoIniciar1;
	campoIniciar1 = crearPID();
	t_PID* campoIniciar2;
	campoIniciar2 = crearPID();
	t_PID* campoIniciar3;
	campoIniciar3 = crearPID();

	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int cant1 = 4, cant2 = 5, cant3 = 6, cant4 = 7, contadorVariableIdMarco = 0;
	int socketMentiroso = 7;
	int tamanioFinalTablaDePag, a;
	int contadorCantProc1 = 0, contadorCantProc2 = 0, contadorCantProc3 = 0, contadorCantProc4 = 0;
	campoIniciar = iniciar_falso(PID1, cant1, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar->PID, PID1);
	campoIniciar1 = iniciar_falso(PID2, cant2, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar1->PID, PID2);
	campoIniciar2 = iniciar_falso(PID3, cant3, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar2->PID, PID3);
	campoIniciar3 = iniciar_falso(PID4, cant4, socketMentiroso);
	CU_ASSERT_EQUAL(campoIniciar3->PID, PID4);

	tamanioFinalTablaDePag = list_size(listaTablaDePag);

	for (a = 0; a < tamanioFinalTablaDePag; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);

		switch (campoTablaDePag->idProc) {
		case (1): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 1);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc1);

			contadorCantProc1++;
			break;
		}
		case (2): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 2);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc2);
			contadorCantProc2++;

			break;
		}
		case (3): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 3);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc3);
			contadorCantProc3++;

			break;
		}
		case (4): {
			CU_ASSERT_EQUAL(campoTablaDePag->idProc, 4);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPresencia, 0);
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, contadorVariableIdMarco);
			CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, contadorCantProc4);
			contadorCantProc4++;

			break;
		}


		}
		contadorVariableIdMarco++;
	}

	CU_ASSERT_EQUAL(tamanioFinalTablaDePag, 22);


}

static void test_probar_escribir_memoria_sin_TLB(){
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7;
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
	t_contenido_pagina* campoEscribir;
	//warning diferentes estructuras, entonces corrijo, son similares, pero importa el orden
	campoEscribir = iniciarContenidoPagina();
	t_escribir_falso* estructuraDevolucionEscribirFalso;
	estructuraDevolucionEscribirFalso = crearEscribirFalso();


	estructuraDevolucionEscribirFalso=escribir_falso(PID1, 1,contenido1, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID2, 1,contenido2, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID2);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 + 1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 1);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID3, 0,contenido3, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4+5+0);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 0);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID3, 5,contenido3Bis, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 +5+5);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 5);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID4, 3,contenido4, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 +5+6+3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 3);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);


	estructuraDevolucionEscribirFalso=escribir_falso(PID4, 4,contenido4, socketMentiroso,socketMentiroso);

	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->PID, PID4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->idMarco,4 +5+6+4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->pagina, 4);
	CU_ASSERT_EQUAL(estructuraDevolucionEscribirFalso->socketSwap, socketMentiroso);



}

static void testRespuestaTraerDeSwapUnaPaginaDeUnProcesoPrueba() {
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7, flagEscritura = 1, flagNoEscritura = 0;
	int tamanioMemoria, a;
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
	t_contenido_pagina* campoEscribir;
	campoEscribir = iniciarContenidoPagina();

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID1, 1, contenido1, flagEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID1);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido1);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID2, 1, contenido2, flagEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID2);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido2);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID3, 0, contenido3, flagNoEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID3);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 0);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido3);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID4, 3, contenido3Bis, flagEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID4);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 3);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido3Bis);

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(PID4, 2, contenido4, flagNoEscritura, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID4);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 2);
	CU_ASSERT_STRING_EQUAL(campoEscribir->contenido, contenido4);

	tamanioMemoria = list_size(listaMemoria);

	CU_ASSERT_EQUAL(tamanioMemoria, 5);

	campoMemoria = list_get(listaMemoria, 0);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 1);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 1);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, "escritura1");

	campoMemoria = list_get(listaMemoria, 1);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+1);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 2);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido2);

	campoMemoria = list_get(listaMemoria, 2);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+5+0);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 3);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3);

	campoMemoria = list_get(listaMemoria, 3);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+5+6+3);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 4);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3Bis);

	campoMemoria = list_get(listaMemoria, 4);

	CU_ASSERT_EQUAL(campoMemoria->idMarco, 4+5+6+2);
	CU_ASSERT_EQUAL(campoMemoria->posicion, 5);
	CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido4);



}

static void test_probar_leer_falso() {
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int socketMentiroso = 7;
	t_contenido_pagina* campoLeer;
	campoLeer = iniciarContenidoPagina();
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

	campoLeer = leer_falso(PID1, 1, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID1);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido1);

	campoLeer = leer_falso(PID2, 1, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID2);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 1);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido2);

	campoLeer = leer_falso(PID3, 0, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID3);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 0);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido3);

	campoLeer = leer_falso(PID4, 3, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID4);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 3);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido3Bis);

	campoLeer = leer_falso(PID4, 2, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, PID4);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 2);
	CU_ASSERT_STRING_EQUAL(campoLeer->contenido, contenido4);

	campoLeer = leer_falso(PID4, 0, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, 11);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 11);

	campoLeer = leer_falso(PID1, 4, socketMentiroso, socketMentiroso);

	CU_ASSERT_EQUAL(campoLeer->PID, 11);
	CU_ASSERT_EQUAL(campoLeer->numeroPagina, 11);



}


static void test_probar_finalizar_un_proceso_sin_TLB() {
	t_PID* id1;
	id1 = crearPID();
	t_PID* id2;
	id2 = crearPID();
	t_PID* id3;
	id3 = crearPID();
	t_PID* id4;
	id4 = crearPID();

	t_PID* PID1;
	PID1 = crearPID();
	PID1->PID = 1;
	t_PID* PID2;
	PID2 = crearPID();
	PID2->PID = 2;
	t_PID* PID3;
	PID3 = crearPID();
	PID3->PID = 3;
	t_PID* PID4;
	PID4 = crearPID();
	PID4->PID = 4;

	int tamanioTablaDePag, tamanioMemoria, socketFalso = 8,a;
	t_TablaDePaginas * campoTablaDePag;
	campoTablaDePag = iniciarTablaDePaginas();


	id1 = finalizar_falso(PID1, socketFalso);

	id2 = finalizar_falso(PID2, socketFalso);

	id3 = finalizar_falso(PID3, socketFalso);

	id4 = finalizar_falso(PID4, socketFalso);



	CU_ASSERT_EQUAL(id1->PID, 1);
	CU_ASSERT_EQUAL(id2->PID, 2);
	CU_ASSERT_EQUAL(id3->PID, 3);
	CU_ASSERT_EQUAL(id4->PID, 4);

	tamanioTablaDePag = list_size(listaTablaDePag);
	tamanioMemoria = list_size(listaMemoria);

	printf("\n%i %i\n",tamanioTablaDePag,tamanioMemoria);

	CU_ASSERT_EQUAL(tamanioTablaDePag, 0);
	CU_ASSERT_EQUAL(tamanioMemoria, 0);
}


static CU_TestInfo tests[] = {
		{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
		{"Test carga archivo configuracion lee variables",test_variables_configuracion },
		{"Test iniciar 4 procesos con 22 paginas en memoria", test_iniciar_4_procesos_con_22_paginas_en_memoria},
		{"Test probar escribir en memoria",test_probar_escribir_memoria_sin_TLB},
		{"Test probar traer de swap una pagina", testRespuestaTraerDeSwapUnaPaginaDeUnProcesoPrueba},
		{"Test probar leer",test_probar_leer_falso},
		{"Test probar finalizar los procesos",test_probar_finalizar_un_proceso_sin_TLB},
		CU_TEST_INFO_NULL,
};

*/

CUNIT_MAKE_SUITE(memoria, "Test Memoria", init_suite, clean_suite, tests)

/*
static void traer_falso_a_fondo_con_LRU() {
	int tamanioMemoria,a=0;
	tamanioMemoria = list_size(listaMemoria);
	printf("\n%i\n",tamanioMemoria);
	t_marco * campoMemoria;
	campoMemoria = iniciarMarco();
	t_contenido_pagina* campoEscribir;
	campoEscribir = iniciarContenidoPagina();

	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->posicion = a;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->posicion = a;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->posicion = a;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->posicion = a;
	a++;
	campoMemoria = list_get(listaMemoria, a);
	campoMemoria->posicion = a;
	a++;

	configuracion->cantidadMarcos= 5;

	campoEscribir = respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(4, 4,"escritura4", 1, 54,45);

	printf("\n%i\n",campoEscribir->PID);
	printf("\n%s\n",campoEscribir->contenido);


}
*/
