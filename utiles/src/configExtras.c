#include "configExtras.h"

int checkearRutaArchivoConfig(char* rutaArchivoConfig) {
	FILE * archivoConfig = NULL;
	archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig == NULL) {
		return -1;
	} else {
		return 0;
	}
}
