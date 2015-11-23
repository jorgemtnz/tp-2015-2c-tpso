
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
#include <signal.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <utiles/sockets.h>
#include <utiles/configExtras.h>
#include <unistd.h>
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
	int bitModificada;
	int bitUso;
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
	char* contenido;
	int Pag;
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

typedef struct {
	int PID;
	int pagina;
	int socketSwap;
	int idMarco;
	char* contenido;
}t_escribir_falso;

typedef struct{
	t_marco* marco;
	int flag; //0 no lo encontro, 1 si lo encontro
}t_marco_con_flag;

typedef struct{
	t_marco* marco;
	int indice;
}t_marco_con_indice;

// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================
t_iniciar_swap* crearEstructuraIniciar();
t_PID* crearPID();
t_configuracion* iniciarArchivoConfig();
t_leerDeProcesoPorEscribir* crearEstructuraLeerProcesoPorEscribir();
// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================

//++++++++++++++++++++++++++++++++++++Funciones Consola +++++++++++++++++++++++++++++++++++++++
void levantarConsola();
void mostrarComandos();
int idFuncion(char* funcion);
void aplicarFuncion(int idFuncion);
int procesarMensajesConsola(int socket, t_header* header, char* buffer);
void limpiarTLB();
void limpiarMemoria();
void volcarMemoria();
void atencionSIGUSR1();
void atencionSIGUSR2();
void asignarSeniales();

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void leerArchivoDeConfiguracion();
void iniciar(int idProc, int cantPag, int socketCPU);
void leer(int idProc, int pag, int socketSwap, int socketCPU);
void finalizar(t_PID* estructuraFinalizar,int socketSwap);
void inicializadoCorrecto(int idProc, int cantPag);
void escribir(int idProc, int nroPag, char* textoAEscribir, int socketSwap, int socketCPU);
t_TablaDePaginas* iniciarTablaDePaginas();
t_contenido_pagina * iniciarEscrituraProc();
t_TLB* iniciarTLB();
t_marco * iniciarMarco();
t_marco_con_flag * iniciarMarcoYFlag();
t_marco_con_indice* iniciarMarcoYIndice();
t_marco_y_bit* iniciarMarcoYBit();
void* interpretarPaquete(Paquete* unPaquete, int fdReceptor);
t_marco_y_bit* buscarSiEstaEnMemoria(int idProc, int nroPag); // retorna o el id o un -1 si no esta en memoria
void escribirEnMarcoYponerBitDeModificada(int idMarco, char* contenido);
void enviarIniciarASwap(t_iniciar_swap *estructura, int socketSwap);
void enviarFinalizarASwap(t_PID *estructura, int socketSwap);
void traerDeSwapUnaPaginaDeUnProceso(int idProc, int nroDePag,int socketSwap);
void traerDeSwapUnaPaginaDeUnProcesoPorEscribir(int idProc,int nroPag, char* textoAEscribir,int socketSwap);
void cargarNuevoMarcoAMemoria(char* contenido,int PID, int pag, int flagEscritura);
bool llegoAlMaximoDelProcesoLaMemoria(int idProc);
void sacarAlMasViejoUsadoDeMemoria(int socketSwap,int PIDACargar,char* contenidoACargar,int pagACargar, int flagEscritura);
void sacarAlMasViejoUsadoDelProcesoDeMemoria(char* contenidoACargar, int PIDACargar, int pagACargar,int flagEscritura,int socketSwap);
void sacarAlPrimeroDeMemoriaDelProceso(char* contenidoACargar, int PIDACargar, int pagACargar, int socketSwap);
void sacarAlPrimeroDeMemoria(int socketSwap, int PIDACargar, char* contenidoACargar, int pagACargar);
void sacarDeMemoriaSegunClockModificado(int socketSwap, int PIDACargar,
		char* contenidoACargar, int pagACargar, int flagEscritura);
void sacaProcesoDeMemoriaSegunClockModificado(char* contenidoACargar,
		int PIDACargar, int pagACargar, int flagEscritura, int socketSwap);
t_marco_con_flag* buscarUsoEnCeroModificadaEnUno() ;
t_marco_con_flag* buscarModificadaYUsoEnCero();
t_marco_con_flag* buscarModificadaYUsoEnCeroDeProceso(t_list* listaMarcoYIndices, int PID);
t_marco_con_flag* buscarUsoEnCeroModificadaEnUnoDeProceso(t_list* listaMarcoYIndices,int PID);
//warning no declarado aca, entonces lo agrego
void sacarAlPrimeroDeTLB() ;
void eliminarDeTLB(int idMenor);
t_list* buscarLosMarcosDeProcesoEnMemoriaConSusIndices(int PID);
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
t_escribir_falso* escribir_falso(int idProc, int nroPag, char* textoAEscribir, int socketSwap,int socketCPU);
t_PID* iniciar_falso(int idProc, int cantPag, int socketCPU);
t_contenido_pagina* leer_falso(int idProc, int pag, int socketSwap, int socketCPU);
t_PID* finalizar_falso(t_PID* estructuraFinalizar,int socketSwap);
t_contenido_pagina* respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(int idProc, int pag,
		char* contenido, int flagEscritura, int socketCPU, int socketSwap);
//warning no definido en test_memoria.c , entonces lo agrego aca.
char* decirHolaMundo() ;
//warning no definido en Memoria.c , entonces lo agrego aca.
void enviarFinalizarACPU(t_PID* estructuraFinalizar, int socketCPU);
t_leerDeProceso* crearEstructuraLeer();
t_contenido_pagina* iniciarContenidoPagina();
//warning no definido en funciones_Auxiliares.c , entonces lo agrego aca.
t_contenido_pagina * iniciarEscrituraProc() ;
t_escribir_falso* crearEscribirFalso();


int procesarMensajesConsola(int socket, t_header* header, char* buffer);

//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++

void inicializacionDesdeCero();
void iniciarConfiguracionTLBNoHabilitada();
void hardcodearTablaDePaginas(int pag1,int pag2,int pag3,int pag4,int pag5);
void iniciarConfiguracionTLBHabilitada();
void hardcodearValoresEnTLB(int PID,int id, int pag);



int getSocketCPU(int pid);
bool hayQueRegistrarPidCpu(int socket);
char* getKeyPidCpu(int pid);
void registrarPidCpu(int socket, int pid);
//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++



//===========================================================================================
t_configuracion* configuracion;
t_dictionary* conexiones;
// ----------- Contadores -------- //
int variableIdMarco,variableTLB,variableEnvejecimientoMarco,indiceClockM; // contador de paginas de la tabla de paginas

// ----------- Listas ------------ //
t_list* listaMemoria;
t_list* listaTLB;
t_list* listaTablaDePag;
t_list* listaIndices;

// ----------- Semaforos ---------- //
pthread_mutex_t mutexProcesadorMensajes;
pthread_mutex_t mutexParaInicializado;
pthread_mutex_t mutexListaMemoria;
pthread_mutex_t mutexListaTLB;
pthread_mutex_t mutexTablaPags;

#endif /* MEMORIA_H_ */
