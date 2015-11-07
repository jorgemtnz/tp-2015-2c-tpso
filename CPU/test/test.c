#include "CUnit/Basic.h"
#include "cunit_def.h"
//#include <utiles/sockets/sockets.h>
#include <utiles/sockets.h>

bool hayQueEjecutarTests(int argc, char *argv[]) {

	int i;
	for (i = 0; i < argc; ++i) {
		if (string_equals(argv[i], "test")) {
			//printf("Hay que ejecutar tests\n");
			return true;
		}
	}

	printf("No hay que ejecutar tests\n");
	return false;
}

int ejecutarTests() {
	printf("Ejecutando tests\n");
	CU_initialize_registry();

	CU_register_suites(suites);

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}
