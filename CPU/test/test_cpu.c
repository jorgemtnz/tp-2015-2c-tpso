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
	int a_rgc=3;
	char* temp = "./CPU;/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg;test";

	char** a_rgv = string_split(temp,";" );
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

static void test_carga_ok_archv_Confg(){
	CU_ASSERT_EQUAL(configuracion->vg_puertoMemoria,5000);
	CU_ASSERT_EQUAL(configuracion->vg_puertoPlanificador,4000);
	CU_ASSERT_EQUAL(configuracion->cantidad_hilos,1);
	CU_ASSERT_EQUAL(configuracion->retardo,28);
	CU_ASSERT_STRING_EQUAL(configuracion->vg_ipMemoria, "127.0.0.1");
	CU_ASSERT_STRING_EQUAL(configuracion->vg_ipPlanificador, "127.0.0.1");
}

static void test_crearProcesoCPU(){
	CU_ASSERT_EQUAL(procCPU->contadorIdCPU,0);
}

static void test_crearCPU(){
	 t_cpu* cpuPrimera = crearCPU();
	 CU_ASSERT_EQUAL(cpuPrimera->cantInstEjecutadas,0);
	 CU_ASSERT_EQUAL(cpuPrimera->estadoEjecucion,0);
	 CU_ASSERT_EQUAL(list_size(procCPU->listaCPU),0);
	 CU_ASSERT_EQUAL(cpuPrimera->porcentajeUso,0);
//	 	 CU_ASSERT_EQUAL(cpuPrimera->idCPU,0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//tests pertenecientes a este suite se deben agregar
static CU_TestInfo tests[] = {
		{ "Test Hola Mundo", test_debe_devolver_hola_mundo},{ "Test carga archvConfig",test_carga_ok_archv_Confg },
		{"test crear procesoCPU",test_crearProcesoCPU },{"test crear CPU",test_crearCPU }, CU_TEST_INFO_NULL };

CUNIT_MAKE_SUITE(cpu, "Test CPU", init_suite, clean_suite, tests)
