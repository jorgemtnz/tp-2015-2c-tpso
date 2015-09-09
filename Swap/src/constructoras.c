/*
 * constructoras.c
 *
 *  Created on: 8/9/2015
 *      Author: utnso
 */

#include "Swap.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------FUNCIONES CONTRUCTORAS-----------------------

l_procesosCargados* crearProceso(){
	l_procesosCargados* proceso = malloc(sizeof(l_procesosCargados));
		if (proceso == NULL) {
			perror("[ERROR] No se reservo memoria para Swap>..>proceso");
			log_error(logger, "[ERROR] No se reservo memoria para Swap>..>proceso");
			exit(-1);
		}
		proceso->PID = 0;
		proceso->cantPagsUso = 0;
		proceso->ubicacion = 0;

		return proceso;
}

l_espacioLibre* crearEspacioLibre(){
	l_espacioLibre* espacio = malloc(sizeof(l_espacioLibre));
			if (espacio == NULL) {
				perror("[ERROR] No se reservo memoria para Swap>..>espacioLibre");
				log_error(logger, "[ERROR] No se reservo memoria para Swap>..>espacioLibre");
				exit(-1);
			}
			espacio->cantPagsLibres = 0;
			espacio->ubicacion = 0;

			return espacio;

}
