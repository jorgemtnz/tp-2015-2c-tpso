#include "CPU.h"
#include<math.h>

void levantarHilosCPU() {
	log_info(logger, "se va a levantar un HILO ");
	pthread_t tidHiloCPU[configuracion->cantidad_hilos];
	pthread_attr_t atributos[configuracion->cantidad_hilos + 1];
	int i;
	i = 0;

	//hilos+1 porque se va a hacer un hilo para calcular el porcentaje
	for (i = 0; i < configuracion->cantidad_hilos + 1; i++) {
		pthread_attr_init(&atributos[i]);
	}

	for (i = 1; i < configuracion->cantidad_hilos + 1; i++) {
		pthread_create(&tidHiloCPU[i], &atributos[i], (void*) hiloCPU, NULL);
	}
	i = 0;
	pthread_create(&tidHiloCPU[i], &atributos[i], (void*) hiloPorcentaje, NULL);

	for (i = 0; i < configuracion->cantidad_hilos + 1; i++) {
		pthread_join(tidHiloCPU[i], NULL);
	}
}

int hiloCPU() {
	log_info(logger, "comienza ejecucion de un HILO ");

	//agregar comportamiento para cada CPU. se debe reiniciar cada vez que se levante una CPU
	//se pondran algunas cosas de lo que actualmente esta en el main
	return 0;
}

int hiloPorcentaje() {

	return 0;
}

void calcularPorcentaje(){
	while(1){
		sleep(60); //duerme cada 60 segundos
		void sacaPorcentaje(t_cpu* cpu){
			// 60 instrucciones equivale al 100%
			cpu->porcentajeUso =(int) (cpu->cantInstEjecutadas* 1.7);
			cpu->cantInstEjecutadas =0;
		}
		list_iterate(procCPU->listaCPU,(void*) sacaPorcentaje)	;

	}
}

