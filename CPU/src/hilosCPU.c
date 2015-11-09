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

	t_cpu* cpu = crearCPU();
		resultadoFinal = string_new();
		list_add(procCPU->listaCPU, cpu);
        printf("soy la cpu N° %d, y mi nombre es, %s \n", cpu->idCPU,cpu->nombre);
		conexiones = dictionary_create();

		int socketPlanificador;
		int socketMemoria;
		int resultConexion_mem = 0;
		int resultConexion_planif = 0;


		resultConexion_planif = conectar(configuracion->vg_ipPlanificador,
				string_itoa(configuracion->vg_puertoPlanificador),
				&socketPlanificador);
		if (resultConexion_planif == -1)
			log_error(logger, "[ERROR]no se conecto el CPU al Planificador");

		dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));

		resultConexion_mem = conectar(configuracion->vg_ipMemoria,
				string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);
		if (resultConexion_mem == -1)
			log_error(logger, "[ERROR]no se conecto el CPU a la memoria");

		dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));

		// poner aca el levantar hilos y luego el
		escucharConexiones("0", socketPlanificador, socketMemoria, 0,
				procesarMensajes, NULL, logger);



	return 0;
}

int hiloPorcentaje() {
calcularPorcentaje();
	return 0;
}

void calcularPorcentaje(){
	int i=1;
	while(1){
		sleep(60); //duerme cada 60 segundos
		void sacaPorcentaje(t_cpu* cpu){
			// 60 instrucciones equivale al 100%
			cpu->porcentajeUso =(int) (cpu->cantInstEjecutadas* 1.7);
			cpu->cantInstEjecutadas =0;
			printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			printf("%s,\n",(queCPUsoy(cpu)));
			printf("Para el minuto %d, \n", i);
			printf("el porcentaje de uso es de  %d, porciento. \n", cpu->porcentajeUso);
			printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		}
		list_iterate(procCPU->listaCPU,(void*) sacaPorcentaje)	;

	}
}

