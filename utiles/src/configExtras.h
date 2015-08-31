/*
 * configExtras.h
 *
 *  Created on: 31/7/2015
 *      Author: utnso
 */

#ifndef CONFIGEXTRAS_CONFIGEXTRAS_H_
#define CONFIGEXTRAS_CONFIGEXTRAS_H_

#include <stdlib.h>
#include <stdio.h>

// Retorna -1 si no se encuentra el archivo de configuracion en la ruta especificada
// retorna el fd si el archivo esta en la ruta especificada
int checkearRutaArchivoConfig(char* rutaArchivoConfig);

#endif /* CONFIGEXTRAS_CONFIGEXTRAS_H_ */
