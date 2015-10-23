/*
 * test_suite1.c
 *
 *  Created on: 12/4/2015
 *      Author: Alejandro Zalazar
 */

#include <string.h> /* memset */
#include <unistd.h> /* close */

#include "../src/CPU.h"
//se ejecuta al comienzo del suite
static int init_suite() {
	int a_rgc = 3;
	char* temp = "./CPU;/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg;test";

	char** a_rgv = string_split(temp, ";");
	a_rgc = 3;

	leerArchivoDeConfiguracion(a_rgc, a_rgv);
	procCPU = crearProcCPU();

	return 0;
}
//se ejecuta al final del suite
static int clean_suite() {
	return 0;
}
static void test_debe_devolver_hola_mundo() {
	CU_ASSERT_STRING_EQUAL("Hola Mundo", decirHolaMundo());
}

static void test_carga_ok_archv_Confg() {
	CU_ASSERT_EQUAL(configuracion->vg_puertoMemoria, 5000);
	CU_ASSERT_EQUAL(configuracion->vg_puertoPlanificador, 4000);
	CU_ASSERT_EQUAL(configuracion->cantidad_hilos, 1);
	CU_ASSERT_EQUAL(configuracion->retardo, 28);
	CU_ASSERT_STRING_EQUAL(configuracion->vg_ipMemoria, "127.0.0.1");
	CU_ASSERT_STRING_EQUAL(configuracion->vg_ipPlanificador, "127.0.0.1");
}

static void test_crearProcesoCPU() {
	CU_ASSERT_EQUAL(procCPU->contadorIdCPU, 0);
	printf("test antes resultado\n");
}

static void test_crearCPU() {
	t_cpu* cpuPrimera = crearCPU();
	CU_ASSERT_EQUAL(cpuPrimera->cantInstEjecutadas, 0);
	CU_ASSERT_EQUAL(cpuPrimera->estadoEjecucion, 0);
	CU_ASSERT_EQUAL(list_size(procCPU->listaCPU), 0);
	CU_ASSERT_EQUAL(cpuPrimera->porcentajeUso, 0);
	CU_ASSERT_EQUAL(cpuPrimera->idCPU, 0);
//	destHiloCPU(cpuPrimera);

}

static void test_RESUL_INICIAR_PROC_OK_CPU() {
	t_cpu* cpuPrimera = crearCPU();
	char* buffer = malloc(sizeof(char));
	void* extra;
	t_PID* datosDesdememoria = malloc(sizeof(t_PID));
	datosDesdememoria->PID = 1;
	sprintf(buffer, "\%d", datosDesdememoria->PID);
	t_header* header = malloc(sizeof(header));
	header->tipoMensaje = RESUL_INICIAR_PROC_OK_CPU;
	header->tamanioMensaje = strlen(buffer);

	recibirMensajeVarios(header, buffer, extra, cpuPrimera);
	t_resultado_instruccion* resultadoIniciar = malloc(sizeof(t_resultado_instruccion));
	resultadoIniciar = list_get(cpuPrimera->mCodCPU->respEjec->resultadosInstrucciones, 0);
	CU_ASSERT_EQUAL(resultadoIniciar->tipoMensaje, RESUL_INICIAR_PROC_OK_CPU);

//	destHiloCPU(cpuPrimera);
//	destInstruccion(resultadoIniciar);
}

static void test_iniciarProcNoOkCPU() {
	t_cpu* cpuPrimera = crearCPU();
	char* buffer = malloc(sizeof(char));
	void* extra;
	t_PID* datosDesdememoria = malloc(sizeof(t_PID));
	datosDesdememoria->PID = 1;
	sprintf(buffer, "\%d", datosDesdememoria->PID);
	t_header* header = malloc(sizeof(header));
	header->tipoMensaje = RESUL_INICIAR_PROC_NO_OK_CPU;
	header->tamanioMensaje = strlen(buffer);
	recibirMensajeVarios(header, buffer, extra, cpuPrimera);

	t_resultado_instruccion* resultadoIniciar = malloc(sizeof(t_resultado_instruccion));
	resultadoIniciar = list_get(cpuPrimera->mCodCPU->respEjec->resultadosInstrucciones, 0);

	CU_ASSERT_EQUAL(resultadoIniciar->tipoMensaje, RESUL_INICIAR_PROC_NO_OK_CPU);

}

static void test_ejecutarResul_Fin() {
	t_cpu* cpuPrimera = crearCPU();

	cpuPrimera = ejecutarResul_Fin(cpuPrimera);

	t_resultado_instruccion* resultado = malloc(sizeof(t_resultado_instruccion));

	resultado = list_get(cpuPrimera->mCodCPU->respEjec->resultadosInstrucciones, 0);

	CU_ASSERT_EQUAL(resultado->tipoMensaje, RESUL_FIN_OK);
	CU_ASSERT_EQUAL(cpuPrimera->idCPU, 0);

}

static void test_RESUL_LEER_OK_CPU() {
	t_cpu* cpuPrimera = crearCPU();
	char* buffer = malloc(sizeof(char));
	void* extra;
	t_PID* datosDesdememoria = malloc(sizeof(t_PID));
	datosDesdememoria->PID = 1;
	sprintf(buffer, "\%d", datosDesdememoria->PID);
	t_header* header = malloc(sizeof(header));
	header->tipoMensaje = RESUL_LEER_OK_CPU;
	header->tamanioMensaje = strlen(buffer);
	recibirMensajeVarios(header, buffer, extra, cpuPrimera);

	t_resultado_instruccion* resultadoIniciar = malloc(sizeof(t_resultado_instruccion));
	resultadoIniciar = list_get(cpuPrimera->mCodCPU->respEjec->resultadosInstrucciones, 0);

	CU_ASSERT_EQUAL(resultadoIniciar->tipoMensaje, RESUL_LEER_OK_CPU);
}
static void test_ejecuta_FinProcesoMemoria() {
	t_cpu* cpu = crearCPU();
	t_PID* estructura = malloc(sizeof(t_PID));
	estructura = ejecuta_FinProcesoMemoria(cpu);
	CU_ASSERT_EQUAL(estructura->PID, 0);
	CU_ASSERT_EQUAL(cpu->estadoEjecucion, NO_USO);
}

static void test_ejecuta_LeerMemoria() {
	t_cpu* cpu = crearCPU();
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
	t_instruccion* instruccion = creaInstruccion();
	char* instruccion_origen = "inicializar 3";
	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = instruccion->instruccion_separada[0];
	estructura = ejecuta_LeerMemoria(instruccion->instruccion_separada[1], cpu);
	CU_ASSERT_EQUAL(estructura->PID, 0);
	CU_ASSERT_EQUAL(cpu->estadoEjecucion, NO_USO);
	CU_ASSERT_EQUAL(estructura->numeroPagina, 4);
}

static void test_ejecuta_IniciarProceso() {
	t_iniciar_swap* estructura = malloc(sizeof(t_iniciar_swap));
	t_cpu* cpu = crearCPU();
	t_instruccion* instruccion = creaInstruccion();
	char* instruccion_origen = "inicializar 3";
	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = instruccion->instruccion_separada[0];
	estructura = ejecuta_IniciarProceso(instruccion->instruccion_separada[1], cpu);
	CU_ASSERT_EQUAL(estructura->PID, 0);
	CU_ASSERT_EQUAL(cpu->estadoEjecucion, NO_USO);
	CU_ASSERT_EQUAL(estructura->cantidadPaginas, 10); //DATO HARCODEADO OJO CUANDO LO CAMBIEN
}

static void test_ejecuta_EscribirMemoria() {
	t_cpu* cpu = crearCPU();
	t_contenido_pagina* estructura = malloc(sizeof(t_contenido_pagina));
	t_instruccion* instruccion = creaInstruccion();
	char* instruccion_origen = "inicializar 3";
	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = instruccion->instruccion_separada[0];
	estructura = ejecuta_EscribirMemoria(instruccion->instruccion_separada[1], cpu);
	CU_ASSERT_EQUAL(estructura->PID, 0);
	CU_ASSERT_EQUAL(cpu->estadoEjecucion, NO_USO);
	CU_ASSERT_EQUAL(estructura->numeroPagina, 4); //DATOS HARCODEADOS OJO CUANDO SE CAMBIEN
	CU_ASSERT_STRING_EQUAL(estructura->contenido, "Hola");
}

static void test_ejecuta_EntradaSalida() {
	t_cpu* cpu = crearCPU();
	t_entrada_salida* estructura = malloc(sizeof(t_entrada_salida));
	t_instruccion* instruccion = creaInstruccion();
	char* instruccion_origen = "inicializar 3";
	instruccion->instruccion_separada = separaInstruccion(instruccion_origen);
	instruccion->ptrComienzoInstruccion = instruccion->instruccion_separada[0];
	estructura = ejecuta_EntradaSalida(instruccion->instruccion_separada[1], cpu);
	CU_ASSERT_EQUAL(estructura->PID, 0);
	CU_ASSERT_EQUAL(cpu->estadoEjecucion, NO_USO); //DATOS HARCODEADOS OJO CUANDO SE CAMBIEN

	CU_ASSERT_STRING_EQUAL(estructura->expresion, "mProc 0 en entrada-salida de tiempo 28");
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//tests pertenecientes a este suite se deben agregar
static CU_TestInfo tests[] = { { "Test Hola Mundo", test_debe_devolver_hola_mundo }, { "Test carga archvConfig", test_carga_ok_archv_Confg }, {
		"test crear procesoCPU", test_crearProcesoCPU }, { "test crear CPU", test_crearCPU },
		{ "test RESUL_INICIAR_PROC_OK_CPU", test_RESUL_INICIAR_PROC_OK_CPU }, { "test_iniciarProcNoOkCPU", test_iniciarProcNoOkCPU }, { "test RESUL_FIN",
				test_ejecutarResul_Fin }, { "test_RESUL_LEER_OK_CPU", test_RESUL_LEER_OK_CPU }, { "test_ejecuta_FinProcesoMemoria",
				test_ejecuta_FinProcesoMemoria }, { " test_ejecuta_LeerMemoria", test_ejecuta_LeerMemoria }, { "test_ejecuta_IniciarProceso",
				test_ejecuta_IniciarProceso }, { "test_ejecuta_EscribirMemoria", test_ejecuta_EscribirMemoria }, { "test_ejecuta_EntradaSalida",
				test_ejecuta_EntradaSalida }, CU_TEST_INFO_NULL };

CUNIT_MAKE_SUITE(cpu, "Test CPU", init_suite, clean_suite, tests)
