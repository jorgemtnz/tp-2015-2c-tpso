#include "configExtras.h"

int checkearRutaArchivoConfig(char* rutaArchivoConfig) {
	FILE* archivoConfig;
	archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig == NULL) {
		//no encontro el archivo
				return -1;
	} else {
	fclose(archivoConfig);
		return 0;
	}

}
