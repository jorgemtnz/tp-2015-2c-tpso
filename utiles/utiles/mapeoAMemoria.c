#include "mapeoAMemoria.h"

// Manejo de Archivos

int abreArchivo(char* dirArchivo) {
	int fd;
	if ((fd = open(dirArchivo, O_RDONLY)) == -1) {
		//Si no se pudo abrir, imprimir el error y abortar;
		perror("[ERROR] Funcion OPEN: Error al abrir el archivo \n");
		exit(-1);
	}
	return fd;
}

int tamanio_archivo(int fd) {
	struct stat buf;
	fstat(fd, &buf);
	return buf.st_size;
}

void cierraArchivo(int fdArchivo) {
	if (close(fdArchivo) == -1) {
		perror("[ERROR] Funcion CLOSE: Error al cerrar el archivo\n");
		exit(-1);
	}
}
// FIN Funciones Archivos


// Funciones mapeo
void* mapeaAMemoria(int tamanioPagina, int fdArchivo, char* ptrDirArchivo) {
	void* ptrMapeo;
	if ((ptrMapeo = mmap( NULL, tamanioPagina, PROT_READ, MAP_SHARED, fdArchivo, 0))
		 == MAP_FAILED) {
		perror("[ERROR] Funcion MMAP: Error al mapear el archivo\n");
		exit(-1);
	}
	return ptrMapeo;
}

void mapeoAmemoria(char* dirArchivo, char** ptrComienzoMemoriaMapeada, int* ptrTamanioDePagina) {
	int archivo;
	int tamanio;
	char* ptrAMapeo;

	archivo = abreArchivo(dirArchivo);
	tamanio = tamanio_archivo(archivo);
	ptrAMapeo = mapeaAMemoria(tamanio, archivo, dirArchivo);
	cierraArchivo(archivo);
	ptrTamanioDePagina = &tamanio;
	*ptrComienzoMemoriaMapeada = ptrAMapeo;
	//ptrComienzoMemoriaMapeada = strdup( *ptrAMapeo);
}

void imprimeMapeo(int tamanioDelArchivo, char* ptrAMapeo) {
	printf("Tamaño del archivo: %d\nContenido:'%s'\n", tamanioDelArchivo, ptrAMapeo);
}

void desMapea(int tamanio, char* ptrAMapeo) {
	if (munmap(ptrAMapeo, tamanio) == -1) {
		perror("[ERROR] Funcion MUNMAP: Error al desmapear memoria\n");
		exit(-1);
	}
}
