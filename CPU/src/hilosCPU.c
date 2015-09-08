#include "CPU.h"

void levantarHilosCPU() {
	pthread_t tidHiloCPU[configuracion->cantidad_hilos];
	pthread_attr_t atributos[configuracion->cantidad_hilos];
	int i;

	//hilos
	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_attr_init(&atributos[i]);
	}

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_create(&tidHiloCPU[i], &atributos[i], hiloCPU, NULL);
	}

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_join(tidHiloCPU[i], NULL);
	}
}

void hiloCPU(){
	//agregar comportamiento para cada CPU. se debe reiniciar cada vez que se levante una CPU
}



