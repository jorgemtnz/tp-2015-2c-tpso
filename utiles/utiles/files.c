/*
 * files.c
 *
 *  Created on: 4/9/2015
 *      Author: utnso
 */

#include "files.h"

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

int abrirArchivoLecturaEscritura(char* pathArchivo, t_log* logger) {
	int fd = open(pathArchivo, O_RDWR, mode);
	if (fd == -1) {
		log_error(logger, "open rw: %s", strerror(errno));
		return fd;
	}
	return fd;
}


int abrirOCrearArchivoLecturaEscritura(char* pathArchivo, t_log* logger) {
	int fd = open(pathArchivo, O_RDWR | O_CREAT, mode);
	if (fd == -1) {
		log_error(logger, "open or create rw", strerror(errno));
		exit(1);
	}
	return fd;
}

int abrirArchivoSoloLectura(char* pathArchivo, t_log* logger) {
	int fd = open(pathArchivo, O_RDONLY, mode);
	if (fd == -1) {
		log_error(logger, "open read only", strerror(errno));
		return fd;
	}
	return fd;
}

char *extraerNombreArchivo(char *text) {

	if (text != NULL) {
		char **elementoPath = string_split(text, "/");

		char *ultimoElementoEncontrado = NULL;

		while (*elementoPath != NULL) {
			if(ultimoElementoEncontrado != NULL) {
				free(ultimoElementoEncontrado);
			}

			ultimoElementoEncontrado = string_duplicate(*elementoPath);
			free(*elementoPath);
			elementoPath++;
		}

		return ultimoElementoEncontrado;
	}

	return NULL;
}


struct stat* describirArchivo(char* archivo, t_log* logger) {
	struct stat* sbuf = malloc(sizeof(struct stat));
	if (stat(archivo, sbuf) == -1) {
		log_error(logger, "stat: archivo: %s, error: %s", archivo, strerror(errno));
		exit(1);
	}
	return sbuf;
}


int obtenerTamanioArchivo(char* path, t_log* logger) {
	struct stat* statArchivoEspacioDatos;
	statArchivoEspacioDatos = describirArchivo(path, logger);
    return statArchivoEspacioDatos != NULL ? statArchivoEspacioDatos->st_size:-1;
}



ssize_t readLineN(int fd, void *buffer, size_t n)
{
    ssize_t numRead;                    /* # of bytes fetched by last read() */
    size_t totRead;                     /* Total bytes read so far */
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;                       /* No pointer arithmetic on "void *" */

    totRead = 0;
    for (;;) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)         /* Interrupted --> restart read() */
                continue;
            else
                return -1;              /* Some other error */

        } else if (numRead == 0) {      /* EOF */
            if (totRead == 0)           /* No bytes read; return 0 */
                return 0;
            else                        /* Some bytes read; add '\0' */
                break;

        } else {                        /* 'numRead' must be 1 if we get here */
			if (totRead < n) {      /* Discard > (n - 1) bytes */
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    return totRead;
}

bool existeArchivo(char* path) {
	if( access( path, F_OK ) == -1 ) {
		return false;
	}
	return true;
}

void borrarArchivo(char *pathArchivo) {
	unlink(pathArchivo);
}

//da como resultado el md5 de un archivo a partir de la ruta de un archivo
char *dame_md5(char *archivo){
	int fd[2];
	int pidHijo;
	char *resultado;
	pipe(fd);
	char result[1000];
	memset(result,'\0',1000);
	if ( (pidHijo = fork() ) == -1){
		fprintf(stderr, "Fallo el FORK");
	} else if( pidHijo == 0) {
		close(1);
		dup2(fd[1], 1);
		close(fd[0]);
		execlp("/usr/bin/md5sum","md5sum",archivo,NULL);
	}
	wait(NULL);
	read(fd[0], result, sizeof(result));
	printf("%s",result);
	resultado=malloc(sizeof(result));
	strcpy(resultado,result);
	return resultado;
}

int file_get_size(char* filename){
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}
