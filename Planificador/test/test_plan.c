/*
 * test_suite1.c
 *
 *  Created on: 12/4/2015
 *      Author: Alejandro Zalazar
 */

#include <string.h> /* memset */
#include <unistd.h> /* close */

#include "../src/Planificador.h"

static int init_suite() {
	return 0;
}

static int clean_suite() {
	return 0;
}
static void test_debe_devolver_hola_mundo() {
	CU_ASSERT_STRING_EQUAL("Hola Mundo", decirHolaMundo());
}
static void test_ps(){
	t_list* listaEstados = list_create();
	t_list* listaPCB = list_create();
	char* StringAux = string_new();
	t_pcb* PCB1 = crearPcb("/home/utnso/corto.cod");
	list_add(listaPCB,PCB1);
	t_pcb* PCB2 = crearPcb("/home/utnso/corto.cod");
	list_add(listaPCB,PCB2);
	t_pcb* PCB3 = crearPcb("/home/utnso/corto.cod");
	list_add(listaPCB,PCB3);
	listaEstados = generarMensajesPsLista(listaPCB, "Ejecutando");
	StringAux= list_get(listaEstados,1);
	CU_ASSERT_STRING_EQUAL(StringAux,"Mproc 1: corto.cod -> Ejecutando");
	StringAux= list_get(listaEstados,2);
	CU_ASSERT_STRING_EQUAL(StringAux,"Mproc 2: corto.cod -> Ejecutando");
	StringAux= list_get(listaEstados,3);
	CU_ASSERT_STRING_EQUAL(StringAux,"Mproc 3: corto.cod -> Ejecutando");

}

t_list* generarMensajesPsLista(t_list* listaPcb, char* estado){
	int i;
	t_list* listaMensajes = list_create();
	t_pcb* PCB_Aux = malloc(sizeof(t_pcb));
	char* stringLista = string_new();
	char* nombreProc = string_new();
	for (i=0;i<list_size(listaPcb);i++){
		PCB_Aux = list_get(listaPcb,i);
		nombreProc = obtenerNombreProc(PCB_Aux->rutaArchivoMcod);
		string_append(&stringLista, "Mproc ");
		string_append(&stringLista, string_itoa(PCB_Aux->pid));
		string_append(&stringLista, ": ");
		string_append(&stringLista, nombreProc);
		string_append(&stringLista, " -> ");
		string_append(&stringLista, estado);
		list_add(listaMensajes, stringLista);
	}
free (stringLista);
free (PCB_Aux);
return listaMensajes;
}

char* obtenerNombreProc(char* ruta){
	int i;
	for (i=string_length(ruta); ruta[i]=='/'; i--){
	}
	return string_substring_from(ruta,i);
}

static CU_TestInfo tests[] = {
		{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
		{ "Test Comando PS" , test_ps},
		CU_TEST_INFO_NULL };

CUNIT_MAKE_SUITE(plan, "Test planificador", init_suite, clean_suite, tests)
