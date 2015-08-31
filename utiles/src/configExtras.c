#include "configExtras.h"

int checkearRutaArchivoConfig(char* rutaArchivoConfig) {
	FILE* archivoConfig;
	archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig == NULL) {
		return -1;
	} else {
		return archivoConfig->_fileno;
	}

}
