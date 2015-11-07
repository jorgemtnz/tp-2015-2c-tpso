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
	int contadorMarcoNegativo = 0;
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
		contadorVariableIdMarco++;

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
	}

	CU_ASSERT_EQUAL(tamanioFinalTablaDePag, 22);

}

static void test_probar_buscar_si_esta_en_memoria_sin_TLB(){
	iniciarConfiguracionTLBNoHabilitada();
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	// int cant1 = 4, cant2 = 5, cant3 = 6, cant4 = 7;

	int pag1 = 1, pag2 = 5, pag3 = 9, pag4 = 14,pag5 = 18;
	hardcodearTablaDePaginas(pag1,pag2,pag3,pag4,pag5);
	t_marco_y_bit* marcoBit1 = iniciarMarcoYBit();
	t_marco_y_bit* marcoBit2 = iniciarMarcoYBit();
	t_marco_y_bit* marcoBit3 = iniciarMarcoYBit();
	t_marco_y_bit* marcoBit4 = iniciarMarcoYBit();
	t_marco_y_bit* marcoBit5 = iniciarMarcoYBit();

//	 int idMarco1,idMarco2,idMarco3,idMarco4,idMarco5;

//warning asignacion incorrecta se devuelve  t_marco_y_bit* buscarSiEstaEnMemoria(int idProc, int nroPag)
	// y se le pasa a un int se corrije
	marcoBit1= buscarSiEstaEnMemoria(PID1,1);
	marcoBit2= buscarSiEstaEnMemoria(PID2,1);
	marcoBit3 = buscarSiEstaEnMemoria(PID3,0);
	marcoBit4 = buscarSiEstaEnMemoria(PID3,5);
	marcoBit5 = buscarSiEstaEnMemoria(PID4,3);

	CU_ASSERT_EQUAL(marcoBit1->idMarco,455);
	CU_ASSERT_EQUAL(marcoBit2->idMarco,456);
	CU_ASSERT_EQUAL(marcoBit3->idMarco,457);
	CU_ASSERT_EQUAL(marcoBit4->idMarco,458);
	CU_ASSERT_EQUAL(marcoBit5->idMarco,459);

}

static void escribir_en_marco_y_poner_bit_de_modificada(){
	//warning no se usa variable, entonces se comentan
//	int tamanioMemoria,a;
//	tamanioMemoria= list_size(listaMemoria);
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
	//warning no se usan variables, entonces comento
//	int pag1 = 1, pag2 = 5, pag3 = 9, pag4 = 14,pag5 = 18 ,tamanioMemoria;
	int PID1 = 1, PID2 = 2, PID3 = 3, PID4 = 4;
	int b=0;
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
	t_contenido_pagina* campoEscribir;
	//warning diferentes estructuras, entonces corrijo, son similares, pero importa el orden
	campoEscribir = iniciarEscrituraProc();


	escribir_falso(PID1, 1,contenido1, socketMentiroso);
	escribir_falso(PID2, 1,contenido2, socketMentiroso);
	escribir_falso(PID3, 0,contenido3, socketMentiroso);
	escribir_falso(PID3, 5,contenido3Bis, socketMentiroso);
	escribir_falso(PID4, 3,contenido4, socketMentiroso);
	campoEscribir = escribir_falso(PID4, 4,contenido4, socketMentiroso);

	CU_ASSERT_EQUAL(campoEscribir->PID, PID4);
	CU_ASSERT_EQUAL(campoEscribir->contenido, contenido4);
	CU_ASSERT_EQUAL(campoEscribir->numeroPagina, 4);

	for (a = 0; a < 4; a++) {
		campoTablaDePag = list_get(listaTablaDePag, a);
		b--;
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID1);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(1 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, b);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		} else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 455);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for (a = 0; a < 5; a++) {
		b --;
		campoTablaDePag = list_get(listaTablaDePag, a + 4);
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID2);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(1 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, b);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		}else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 456);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for (a = 0; a < 6; a++) {
		b--;
		campoTablaDePag = list_get(listaTablaDePag, a + 9);
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID3);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(0 != a && 5 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, b);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		} else if ( 5 != a){
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 457);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 458);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for (a = 0; a < 7; a++) {
		b--;
		campoTablaDePag = list_get(listaTablaDePag, a + 15);
		CU_ASSERT_EQUAL(campoTablaDePag->idProc, PID4);
		CU_ASSERT_EQUAL(campoTablaDePag->paginaDelProceso, a);
		if(3 != a){
		CU_ASSERT_EQUAL(campoTablaDePag->idMarco, b);
		CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 0);
		}else {
			CU_ASSERT_EQUAL(campoTablaDePag->idMarco, 459);
			CU_ASSERT_EQUAL(campoTablaDePag->bitPagModificada, 1);
		}
	}

	for(a=0;a<5;a++){
		campoMemoria= list_get(listaMemoria,a);
		switch(a){
		case 0: {
			CU_ASSERT_EQUAL(campoMemoria->idMarco,455);
			CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido1);
			break;
		}
		case 1: {
			CU_ASSERT_EQUAL(campoMemoria->idMarco,456);
			CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido2);
			break;
		}
		case 2: {
			CU_ASSERT_EQUAL(campoMemoria->idMarco,457);
			CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3);
			break;
		}
		case 3: {
			CU_ASSERT_EQUAL(campoMemoria->idMarco,458);
			CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido3Bis);
			break;
		}
		case 4: {
			CU_ASSERT_EQUAL(campoMemoria->idMarco,459);
			CU_ASSERT_STRING_EQUAL(campoMemoria->contenido, contenido4);
			break;
		}
		}
	}



}

static void test_buscar_los_id_de_proceso(){
	int PID1=1 ,PID2=2 ,PID3=3 ,PID4=4 ;
	int cont1=0,cont2=0,cont3=0,cont4=0;
	//warning id sin inicializar,entonces lo inicializo a 0
	int tamaniolista,a,id=0;
	//warning, no se usa la b, se comenta
//	int b=0;
	t_marco_y_bit* marcoYBit;
	marcoYBit = iniciarMarcoYBit();

	t_list* lista1;
	lista1 = list_create();
	t_list* lista2;
	lista2 = list_create();
	t_list* lista3;
	lista3 = list_create();
	t_list* lista4;
	lista4 = list_create();

	lista1 = buscarLosMarcoYBitDeProceso(PID1);
	tamaniolista = list_size(lista1);
	for (a = 0; a < tamaniolista; a++) {
		marcoYBit = list_get(lista1,a);
		if (marcoYBit->idMarco == 0) {
			cont1++;
		} else {
			CU_ASSERT_EQUAL(id, 455);
		}

	}
	lista2 = buscarLosMarcoYBitDeProceso(PID2);
	tamaniolista = list_size(lista2);
	for (a = 0; a < tamaniolista; a++) {
		marcoYBit = list_get(lista2,a);
		if (marcoYBit->bitPresencia == 0) {
			cont2++;
		} else {
			CU_ASSERT_EQUAL(id, 456);
		}

	}
	lista3 = buscarLosMarcoYBitDeProceso(PID3);
	tamaniolista = list_size(lista3);
	for (a = 0; a < tamaniolista; a++) {
		marcoYBit = list_get(lista3,a);
		if (marcoYBit->idMarco == 0) {
			cont3++;
		} else if (id == 457) {
			CU_ASSERT_EQUAL(id, 457);
		} else {
			CU_ASSERT_EQUAL(id, 458);
		}

	}
	lista4 = buscarLosMarcoYBitDeProceso(PID4);
	tamaniolista = list_size(lista4);
	for (a = 0; a < tamaniolista; a++) {
		marcoYBit = list_get(lista4,a);
		if (marcoYBit->bitPresencia == 0) {
			cont4++;
		} else {
			CU_ASSERT_EQUAL(id, 459);
		}

	}
	CU_ASSERT_EQUAL(cont1, 3);
	CU_ASSERT_EQUAL(cont2, 4);
	CU_ASSERT_EQUAL(cont3, 4);
	CU_ASSERT_EQUAL(cont4, 6);

}

static void test_traer_contenido_de_marco(){
	int id1=455, id2=456, id3=457, id4=458, id5=459;
	char* contenido1;
	char* contenido2;
	char* contenido3;
	char* contenido4;
	char* contenido5;

	contenido1 = traerContenidoDeMarco(id1);
	contenido2 = traerContenidoDeMarco(id2);
	contenido3 = traerContenidoDeMarco(id3);
	contenido4 = traerContenidoDeMarco(id4);
	contenido5 = traerContenidoDeMarco(id5);

	CU_ASSERT_STRING_EQUAL(contenido1,"escritura1");
	CU_ASSERT_STRING_EQUAL(contenido2,"escritura2");
	CU_ASSERT_STRING_EQUAL(contenido3,"escritura3");
	CU_ASSERT_STRING_EQUAL(contenido4,"escritura3Bis");
	CU_ASSERT_STRING_EQUAL(contenido5,"escritura4");



}

static void test_llego_maximo_de_marco_del_proceso(){
	int PID1=1,PID2=2,PID3=3,PID4=4;
	configuracion->maximosMarcosPorProceso = 2;
	bool respuesta1;
	bool respuesta2;
	bool respuesta3;
	bool respuesta4;
	int resp1,resp2,resp3,resp4;


	respuesta1 =llegoAlMaximoDelProcesoLaMemoria(PID1);
	respuesta2 =llegoAlMaximoDelProcesoLaMemoria(PID2);
	respuesta3 =llegoAlMaximoDelProcesoLaMemoria(PID3);
	respuesta4 =llegoAlMaximoDelProcesoLaMemoria(PID4);

	if(!respuesta1){
		resp1 = 1;
	}
	if(!respuesta2){
		resp2=1;
	}
	if(!respuesta4){
		resp4=1;
	}

	if(respuesta3){
		resp3=1;
	}

	CU_ASSERT_EQUAL(resp1, 1);
	CU_ASSERT_EQUAL(resp2, 1);
	CU_ASSERT_EQUAL(resp3, 1);
	CU_ASSERT_EQUAL(resp4, 1);

}

static void test_probar_finalizar_un_proceso_sin_TLB(){
	t_PID PID1;
	PID1.PID=1;
	t_PID PID2;
	PID2.PID=2;
	t_PID PID3;
	PID3.PID=3;
	t_PID PID4;
	PID4.PID=4;
	int tamanioTablaDePag,tamanioMemoria,socketFalso=8;
	t_PID* id1;
	id1 = crearPID();
	t_PID* id2;
	id2 = crearPID();
	t_PID* id3;
	id3 = crearPID();
	t_PID* id4;
	id4 = crearPID();

	id1=finalizar_falso(&PID1,socketFalso);

	id2=finalizar_falso(&PID2,socketFalso);

	id3=finalizar_falso(&PID3,socketFalso);

	id4=finalizar_falso(&PID4,socketFalso);

	CU_ASSERT_EQUAL(id1->PID, 1);
	CU_ASSERT_EQUAL(id2->PID, 2);
	CU_ASSERT_EQUAL(id3->PID, 3);
	CU_ASSERT_EQUAL(id4->PID, 4);

	tamanioTablaDePag = list_size(listaTablaDePag);
	tamanioMemoria = list_size(listaMemoria);

	CU_ASSERT_EQUAL(tamanioTablaDePag, 0);
	CU_ASSERT_EQUAL(tamanioMemoria, 0);
}


static CU_TestInfo tests[] = {
	{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
	{"Test carga archivo configuracion lee variables",test_variables_configuracion },
	{"Test iniciar 4 procesos con 22 paginas en memoria", test_iniciar_4_procesos_con_22_paginas_en_memoria},{"Test busca 6 marcos y uno no encuentra", test_probar_buscar_si_esta_en_memoria_sin_TLB},
	{"Test escribe 5 contenidos en memoria",escribir_en_marco_y_poner_bit_de_modificada},{"Test probar escribir en memoria",test_probar_escribir_memoria_sin_TLB},{"Test busca los id de los procesos",test_buscar_los_id_de_proceso},
	{"Test traer contenido",test_traer_contenido_de_marco},{"Test probar si llego al max de proc", test_llego_maximo_de_marco_del_proceso},

	{"Test finalizar los 4 procesos",test_probar_finalizar_un_proceso_sin_TLB},
	CU_TEST_INFO_NULL,
};

CUNIT_MAKE_SUITE(memoria, "Test Memoria", init_suite, clean_suite, tests)
