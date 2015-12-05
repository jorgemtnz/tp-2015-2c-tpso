#include "CPU.h"
#include<math.h>

void levantarHilosCPU() {
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "se va a levantar un HILO ");
	pthread_mutex_unlock(&mutexCPULogs);

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
	pthread_create(&tidHiloporcentaje, &atributosPorcentaje,
			(void*) hiloPorcentaje, NULL);

	for (i = 0; i < configuracion->cantidad_hilos; i++) {
		pthread_join(tidHiloCPU[i], NULL);
	}
	pthread_join(tidHiloporcentaje, NULL);
}

int hiloCPU() {

	t_cpu* cpu = crearCPU();

	list_add(procCPU->listaCPU, cpu);
	pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_info(logger, "comienza ejecucion de un HILO ");
	log_info(logger,
			string_from_format("Me estoy levantando soy la cpu , %s \n",
					cpu->nombre));
	pthread_mutex_unlock(&mutexCPULogs);
//	printf("Me estoy levantando soy la cpu , %s", cpu->nombre);
//	printf("Creando CPU la id del hilo es %lu \n", cpu->idCPU);
	//conexiones = dictionary_create();

	int socketPlanificador;
	int socketMemoria;
	int resultConexion_mem = 0;
	int resultConexion_planif = 0;

	resultConexion_planif = conectar(configuracion->vg_ipPlanificador,
			string_itoa(configuracion->vg_puertoPlanificador),
			&socketPlanificador);
	if (resultConexion_planif == -1)
		pthread_mutex_lock(&mutexCPULogs);
	log_info(logger, identificaCPU(queHiloSoy()));
	log_error(logger, "[ERROR]no se conecto el CPU al Planificador");
	pthread_mutex_unlock(&mutexCPULogs);
	//dictionary_put(conexiones, "Planificador", string_itoa(socketPlanificador));
	cpu->socketPlanificador = socketPlanificador;
	enviarStruct(socketPlanificador, HANDSHAKE_CPU, getNombre());

	resultConexion_mem = conectar(configuracion->vg_ipMemoria,
			string_itoa(configuracion->vg_puertoMemoria), &socketMemoria);
	if (resultConexion_mem == -1)
		log_info(logger, identificaCPU(queHiloSoy()));
	log_error(logger, "[ERROR]no se conecto el CPU a la memoria");

	//dictionary_put(conexiones, "Memoria", string_itoa(socketMemoria));
	cpu->socketMemoria = socketMemoria;

	escucharConexiones("0", socketPlanificador, socketMemoria, 0,
			procesarMensajes, NULL, logger);

	return 0;
}

int hiloPorcentaje() {

	//este es el hilo 0

	calcularPorcentaje();
	return 0;

}

void calcularPorcentaje() {
	int i = 0;
	while (1) {
		i++;
		retardo(60); //duerme cada 60 segundos

		void sacaPorcentaje(t_cpu* cpu) {
			// 60 instrucciones ejecutadas equivale al 100%
//en este if es para una instruccion que empezo antes de los 60s y terminara despues de estos
			if (cpu->estado == NO_TERMINO_RAFAGA
					&& cpu->terminaInstruccion == NO_TERMINO) {
////esta ejecutando aun una instruccion, pero no ha terminado, entonces la sumo, esta en memoria
				incrementaInstPorcentaje(cpu);
				calculaAcumulado(cpu);


////				cpu->retardoTotal = (uint8_t) dameDiferencia(
////						cpu->inicioInstruccion, cpu->finInstruccion);
//				printf("acumuladoSegundos %f\n", cpu->acumuladoSegundos);
//				cpu->porcentajeUso = (uint8_t)(cpu->acumuladoSegundos * 100)
//						/60;

//				resetValPorcentaje(cpu); //lo reseteo porque luego cuando termine la instruccion se contara
			}  //ya termino la rafaga
//				cpu->porcentajeUso = (uint8_t) ((cpu->cantInstEjecutadasPorcentaje * 100)
//						/ instEquivalenteCienPorciento(configuracion->retardo));
			pthread_mutex_lock(&mutexCPUPorcentaje);
			//	cpu->porcentajeUso = (cpu->acumuladoSegundos * 100)
			//							/60;

//			printf("cant Inst ejecutadas %d \n",
//					cpu->cantInstEjecutadasPorcentaje);
//			printf("acumuladoSegundos %f\n", cpu->acumuladoSegundos);
//			cpu->porcentajeUso = (cpu->acumuladoSegundos * 100) / 60;

//			if (cpu->porcentajeUso > 100)
//				cpu->porcentajeUso = 93.331133;
//			tiempo promedio que demora una instruccion
			double result = cpu->acumuladoSegundos
					/ cpu->cantInstEjecutadasPorcentaje;
			//cuantas instrucciones se podrian ejecutar en 60 segundos
			int cienPorcien =  redondea(result);
//			printf("max instrucciones que se pueden ejecutar %d \n",
//					cienPorcien);

			double resultadoPor = ((cpu->cantInstEjecutadasPorcentaje * 100)
					/ cienPorcien);
			double parte_entera;
			modf(resultadoPor, &parte_entera);
			cpu->porcentajeUso =(int)  parte_entera;

//			cpu->porcentajeUso = ((cpu->cantInstEjecutadasPorcentaje * 100)
//					/ instEquivalenteCienPorciento(configuracion->retardo));
			resetValPorcentaje(cpu);
			//calculo el inicio de la otra mitad de esta instruccion
						calculafecha(cpu);
			pthread_mutex_unlock(&mutexCPUPorcentaje);

			//muestra y loguea resultado
			puts(
					string_from_format(
							KRED "++++++++++++++++++++++"RESET "++++++++"KRED "+++++++++++++++++++++++++++\n" RESET));
			puts(string_from_format("%s,\n", queCPUsoy(cpu)));
			puts(string_from_format("Para el minuto %d, \n", i));
			puts(
					string_from_format(
							"el porcentaje de uso es de  %i porciento. \n",
							cpu->porcentajeUso));
			puts(
					string_from_format(
							KBLU "++++++++++++++++++++++"RESET "+++++++++"KBLU "++++++++++++++++++++++++++\n"RESET));

			pthread_mutex_lock(&mutexCPULogs);
			log_info(logger, identificaCPU(queHiloSoy()));
			log_info(logger, "se ejecuta el calculo del porcentaje de la cpu");
			log_info(logger, string_from_format("para el minuto %i \n", i));
			log_info(logger,
					string_from_format("porcentaje de %i \n",
							cpu->porcentajeUso));
			pthread_mutex_unlock(&mutexCPULogs);
		}

		list_iterate(procCPU->listaCPU, (void*) sacaPorcentaje);

	}
}

