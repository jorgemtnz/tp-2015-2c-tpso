#include "CPU.h"

void levantarHilosCPU() {
	log_info(logger, "se va a levantar un HILO ");
	pthread_t tidHiloCPU[configuracion->cantidad_hilos];
	pthread_attr_t atributos[configuracion->cantidad_hilos];
	int i;

	//hilos
	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_attr_init(&atributos[i]);
	}

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_create(&tidHiloCPU[i], &atributos[i],(void*) hiloCPU, NULL);
	}

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_join(tidHiloCPU[i], NULL);
	}
}

int hiloCPU(){
	log_info(logger, "comienza ejecucion de un HILO ");
	//agregar comportamiento para cada CPU. se debe reiniciar cada vez que se levante una CPU
	//se pondran algunas cosas de lo que actualmente esta en el main
	return 0;
}



