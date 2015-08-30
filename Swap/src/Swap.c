#include "Swap.h"

int main(void) {
	FILE *archivoSwap;
	char* nombreArchivo;
	char* rutaArchivoConfiguracion =
			"/home/utnso/tpso/TPSO/Swap/config_swap.cfg";

	archivoConfiguracion = config_create(rutaArchivoConfiguracion);
	nombreArchivo = config_get_string_value(archivoConfiguracion,
			"NOMBRE_SWAP");

	archivoSwap = fopen(nombreArchivo, "w");
	fputs("\0", archivoSwap);
	fclose(archivoSwap);
	return EXIT_SUCCESS;
}
