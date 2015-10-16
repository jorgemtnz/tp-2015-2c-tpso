/*
 * test_suite1.c
 *
 *  Created on: 12/4/2015
 *      Author: Alejandro Zalazar
 */

#include <string.h> /* memset */
#include <unistd.h> /* close */

#include "../src/CPU.h"

static int init_suite() {
	return 0;
}

static int clean_suite() {
	return 0;
}
static void test_debe_devolver_hola_mundo() {
	CU_ASSERT_STRING_EQUAL("Hola Mundo", decirHolaMundo());
}

static CU_TestInfo tests[] = {
		{ "Test Hola Mundo", test_debe_devolver_hola_mundo },
		CU_TEST_INFO_NULL };

CUNIT_MAKE_SUITE(cpu, "Test CPU", init_suite, clean_suite, tests)
