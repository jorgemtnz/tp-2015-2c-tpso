
#include "CPU.h"


int main(void) {

	 logger = log_create("LOG_CPU.log", "CPU", false, LOG_LEVEL_INFO); //Inicializacion logger
	 leerArchivoDeConfiguracion();
     printf("ip leida %s \n", configuracion->vg_ipMemoria);


	return EXIT_SUCCESS;
}
