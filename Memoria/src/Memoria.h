
 //Memoria.h
#ifndef MEMORIA_H_
#define MEMORIA_H_

// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // read write
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/txt.h>
#include <commons/config.h>
#include <commons/log.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <utiles/sockets.h>
#include <utiles/configExtras.h>
#include "../Test/test.h"


// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================



// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
//=======================================================================================

typedef struct {

	uint32_t puertoEscucha;
	uint32_t puertoSwap;
	char* ipSwap;
	char* nombreMemoria;
	uint8_t maximosMarcosPorProceso;
	uint8_t cantidadMarcos;
	uint8_t tamanioMarcos;
	uint8_t entradasTlb;
	uint8_t tlbHabilitada;
	uint8_t retardoMemoria;
	char* algoritmo_reemplazo;

} t_configuracion;

typedef struct { // estructura que se carga en la lista de memoria principal
	int idMarco; // la memoria identificara a cada marco a traves de este id
	char* contenido; // el texto que tendra esa posicion
	int posicion;
} t_marco;

typedef struct {
	int idProc;
	int paginaDelProceso;// supongo que las paginas del proceso arrancan desde 1
	int idMarco; // si esta vacio va -1, lo que indica que se tiene que ir a buscar al swap
	int bitPagModificada; // si esta en memoria ver si fue modificada
	int bitPresencia; // 1 esta en mem, 0 no
	int posicion;
} t_TLB;

typedef struct {
	int idProc;
	int paginaDelProceso;// supongo que las paginas del proceso arrancan desde 1
	int idMarco; // si esta vacio va -1, lo que indica que se tiene que ir a buscar al swap
	int bitPagModificada; // si esta en memoria ver si fue modificada
	int bitPresencia; // 1 esta en mem, 0 no
} t_TablaDePaginas;

typedef struct {
	int idProc;
	int CantPag;
}t_iniciarProc;

typedef struct {
	int idMarco;
	int bitPresencia;
}t_marco_y_bit;

typedef struct {
	int idProc;
	int Pag;
	char* contenido;
}t_escrituraProc;

typedef struct {
	pid_t idProc;
	int pagIn;
	int pagFin;
	char* contenido;
}t_lectura;

typedef struct {
	pid_t idProc;
}t_finalizarProc;

// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
t_iniciar_swap* crearEstructuraIniciar();
t_PID* crearPID();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================


// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion();
void iniciar(int idProc, int cantPag, int socketCPU);
void leer(int idProc, int pag, int socketSwap, int socketCPU);
void finalizar(t_PID* estructuraFinalizar,int socketSwap);
void inicializadoCorrecto(int idProc, int cantPag);
void escribir(int idProc, int nroPag, char* textoAEscribir, int socketSwap, int socketCPU);
t_TablaDePaginas* iniciarTablaDePaginas();
t_escrituraProc* iniciarEscrituraProc();
t_TLB* iniciarTLB();
t_marco * iniciarMarco();
t_config* iniciarArchivoConfig();
t_marco_y_bit* iniciarMarcoYBit();
void* interpretarPaquete(Paquete* unPaquete, int fdReceptor);
t_marco_y_bit* buscarSiEstaEnMemoria(int idProc, int nroPag); // retorna o el id o un -1 si no esta en memoria
void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido);
void enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap);
void enviarFinalizarASwap(t_PID *estructura, int socketSwap);
void traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag,int socketSwap);
void traerDeSwapUnaPaginaDeUnProcesoPorEscribir(int idProc,int nroPag,int socketSwap);
void cargarNuevoMarcoAMemoria(char* contenido,int PID, int pag);
bool llegoAlMaximoDelProcesoLaMemoria(int idProc);
void sacarAlMasViejoUsadoDeMemoria(int socketSwap,int PIDACargar,char* contenidoACargar,int pagACargar, int flagEscritura);
void sacarAlMasViejoUsadoDelProcesoDeMemoria(char* contenidoACargar, int PIDACargar, int pagACargar,int flagEscritura,int socketSwap);
void sacarAlPrimeroDeMemoriaDelProceso(char* contenidoACargar, int PIDACargar, int pagACargar, int socketSwap);
void sacarAlPrimeroDeMemoria(int socketSwap, int PIDACargar, char* contenidoACargar, int pagACargar);
char* traerContenidoDeMarco(int idMarco);
void cargarNuevoEnTLB(int PID,int pag,int id);
void enviarACPUContenidoPaginaDeUnProcesoPorLeer(t_contenido_pagina* lecturaMandarCpu, int socketCPU);
bool estaLlenaLaMemoria();
void verificarBitDeModificada(t_marco* campoMarco, char* contenidoACargar, int PIDaCargar, int pagACargar,int flagEscritura,int socketSwap);
t_list* buscarLosMarcoYBitDeProceso(int idProc);
t_list* buscarLosMarcosDeProcesoEnMemoria( int PID);
void eliminarDeMemoria(int id);
void eliminarDeTablaDePaginas(int id);
void eliminarDeTLBSiEstaPorNuevoId(int idMenor);
void eliminarDeTablaDePaginasDefinitivamente(int id);
void eliminarDeTLBDefinitivamente(int id);
void enviarASwapEliminarProceso(int idProc);
void enviarASwapContenidoPaginaDesactualizada(int idProc, int pagina, char* contenido, int socketSwap);
void enviarRtaIniciarFalloCPU (t_PID * estructura, int socketCPU);
void enviarRtaEscribirACPU(t_contenido_pagina *estructura, int socketCPU);
void enviarIniciarAlSwap(t_iniciar_swap *estructura, int socketSwap);
void enviarEscribirAlSwap(t_contenido_pagina *estructura, int socketSwap);
void enviarRtaIniciarOkCPU (t_PID * estructura, int socketCPU);
void respuestaTraerDeSwapUnaPaginaDeUnProceso(int idProc, int pag, char* contenido, int flagEscritura,int socketCPU, int socketSwap);
t_contenido_pagina* escribir_falso(int idProc, int nroPag, char* textoAEscribir, int socketSwap);
t_iniciar_swap* iniciar_falso(int idProc, int cantPag, int socketCPU);
t_PID* finalizar_falso(t_PID* estructuraFinalizar,int socketSwap);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++

void inicializacionDesdeCero();
void iniciarConfiguracionTLBNoHabilitada();
void hardcodearTablaDePaginas(int pag1,int pag2,int pag3,int pag4,int pag5);
//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++



//===========================================================================================
t_configuracion* configuracion;
t_log* logger;
t_dictionary* conexiones;
// ----------- Contadores -------- //
int variableIdMarco,variableTLB,variableEnvejecimientoMarco; // contador de paginas de la tabla de paginas

// ----------- Listas ------------ //
t_list* listaMemoria;
t_list* listaTLB;
t_list* listaTablaDePag;

// ----------- Semaforos ---------- //
pthread_mutex_t mutexProcesadorMensajes;
pthread_mutex_t mutexParaInicializado;

#endif /* MEMORIA_H_ */
