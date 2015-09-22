/*
 * espacioDeDatos.h
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#ifndef UTILES_ESPACIODEDATOS_H_
#define UTILES_ESPACIODEDATOS_H_


#include <commons/log.h>
#include <commons/string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "files.h"

char* nuevoEspacioDeDatos(char* path, char* tamanioEspacioDatos, t_log* logger);

char* crearEspacioDeDatos(int fd, int tamanioEspacioDatos, t_log* logger);

void eliminarEspacioDeDatos(char* data, int tamanioEspacioDatos, t_log* logger);

int abrirArchivoEspacioDatos(char* archivoMmap, t_log* logger);

struct stat describirArchivoEspacioDatos(char* archivoMmap, t_log* logger);

void cerrarArchivoEspacioDeDatos(int fd, t_log* logger);

void escribirEnEspacioDatos(char* espacioDatos, char* contenido, int offset, int longitudAEscribir);

char * leerEspacioDatos(char *espacioDatos, int offset, int cantidadALeer) ;

void crearArchivoMmap(char *pathArchivo, char* tamanioArchivoEnBytes);

void borrarArchivoMmapParaTest(char *pathArchivo) ;

#endif /* UTILES_ESPACIODEDATOS_H_ */
