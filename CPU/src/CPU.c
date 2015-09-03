#include "CPU.h"

int main(void) {

	logger = log_create("LOG_CPU.log", "CPU", false, LOG_LEVEL_INFO); //Inicializacion logger
	leerArchivoDeConfiguracion();
	printf("ip leida %s \n", configuracion->vg_ipMemoria);

	int socketPlanificador;
	int socketMemoria;

	conectar(configuracion->vg_ipPlanificador, string_itoa(configuracion->vg_puertoPlanificador), &socketPlanificador);
	conectar(configuracion->vg_ipMemoria, string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);

	escucharConexiones("0", socketPlanificador, socketMemoria, 0);

	return EXIT_SUCCESS;
}
