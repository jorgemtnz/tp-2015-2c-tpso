#include "CPU.h"
#include<math.h>

void levantarHilosCPU() {
	log_info(logger,identificaCPU(queHiloSoy()));
	log_info(logger, "se va a levantar un HILO ");
	pthread_t tidHiloCPU[configuracion->cantidad_hilos];
	pthread_t tidHiloporcentaje;
	pthread_attr_t atributosPorcentaje;
	pthread_attr_t atributos[configuracion->cantidad_hilos];
	int i = 0;

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_attr_init(&atributos[i]);
	}
	pthread_attr_init(&atributosPorcentaje);

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_create(&tidHiloCPU[i], &atributos[i], (void*) hiloCPU, NULL);
	}
	pthread_create(&tidHiloporcentaje, &atributosPorcentaje, (void*) hiloPorcentaje, NULL);

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_join(tidHiloCPU[i], NULL);
	}
	pthread_join(tidHiloporcentaje, NULL);
}

int hiloCPU() {
	log_info(logger,identificaCPU(queHiloSoy()));
	log_info(logger, "comienza ejecucion de un HILO ");

	t_cpu* cpu = crearCPU();

		list_add(procCPU->listaCPU, cpu);
        printf("Me estoy levantando soy la cpu , %s \n",cpu->nombre);
        printf("Creando CPU la id del hilo es %lu \n", cpu->idCPU);
		//conexiones = dictionary_create();

		int socketPlanificador;
		int socketMemoria;
		int resultConexion_mem = 0;
		int resultConexion_planif = 0;

		resultConexion_planif = conectar(configuracion->vg_ipPlanificador,
				string_itoa(configuracion->vg_puertoPlanificador),
				&socketPlanificador);
		if (resultConexion_planif == -1)
			log_info(logger,identificaCPU(queHiloSoy()));
			log_error(logger, "[ERROR]no se conecto el CPU al Planificador");

		//dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));
		cpu->socketPlanificador = socketPlanificador;
		enviarStruct(socketPlanificador, HANDSHAKE_CPU, getNombre());

		resultConexion_mem = conectar(configuracion->vg_ipMemoria,
				string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);
		if (resultConexion_mem == -1)
			log_info(logger,identificaCPU(queHiloSoy()));
			log_error(logger, "[ERROR]no se conecto el CPU a la memoria");

		//dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));
		cpu->socketMemoria = socketMemoria;

			escucharConexiones("0", socketPlanificador, socketMemoria, 0,
				procesarMensajes, NULL, logger);

	return 0;
}

int hiloPorcentaje() {
	log_info(logger,identificaCPU(queHiloSoy()));
	log_info(logger, "se ejecuta el calculo del porcentaje de las cpus");
	//este es el hilo 0
calcularPorcentaje();
	return 0;
}

void calcularPorcentaje(){
	int i=0;
	while(1){
		i++;
		sleep(60); //duerme cada 60 segundos
		void sacaPorcentaje(t_cpu* cpu){
			// 60 instrucciones equivale al 100%
			cpu->porcentajeUso =(int) (cpu->cantInstEjecutadas* 1.7)//+cpu->retardo_acumulado+configuracion->retardo;
			//if(cpu->estado==DISPONIBLE)
			cpu->cantInstEjecutadas =0;
			printf(KRED "++++++++++++++++++++++"RESET "++++++++"KRED "+++++++++++++++++++++++++++\n" RESET);
			printf("%s,\n",queCPUsoy(cpu));
			printf("Para el minuto %d, \n", i);
			printf("el porcentaje de uso es de  %d, porciento. \n", cpu->porcentajeUso);
			printf(KBLU "++++++++++++++++++++++"RESET "+++++++++"KBLU "++++++++++++++++++++++++++\n"RESET);
		}

		list_iterate(procCPU->listaCPU,(void*) sacaPorcentaje)	;

	}
}

