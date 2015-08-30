#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // read write
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <src/commons/string.h>
#include <src/commons/collections/list.h>
#include <src/commons/txt.h>
#include <src/commons/config.h>
#include <src/commons/log.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <src/sockets/sockets.h>
// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================
#define NUMEROFUNCIONESCONSOLA 4


// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
//=======================================================================================



// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================

// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================


// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================


// ++++++++++++++++++++++++++++++++++++++Funciones de Consola
//============================================================================



// +++++++++++++++++++++++++++++++++++++++++++++Consola implementacion
//==================================================================================
int idFuncion(char* funcion);
void aplicarFuncion(int idFuncion);
void levantarConsola();
void mostrarComandos();
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
//========================================================================================


// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++
//===========================================================================================




t_log* logger; //VG del logger

#endif /* PLANIFICADOR_H_ */
