/*
 * files.h
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#ifndef UTILES_FILES_FILES_H_
#define UTILES_FILES_FILES_H_

#include "../commons/log.h"
#include "../commons/string.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *extraerNombreArchivo(char *text);

int abrirArchivoSoloLectura(char* pathArchivo, t_log* logger);

int abrirArchivoLecturaEscritura(char* pathArchivo, t_log* logger);

int abrirOCrearArchivoLecturaEscritura(char* pathArchivo, t_log* logger);

struct stat describirArchivo(char* archivo, t_log* logger);

int obtenerTamanioArchivo(char* path, t_log* logger);

ssize_t readLineN(int fd, void *buffer, size_t n);

bool existeArchivo(char* path);

void borrarArchivo(char *pathArchivo);


#endif /* UTILES_FILES_FILES_H_ */
