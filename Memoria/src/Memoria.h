
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
	uint8_t tlbHabilitada; // 0 no -- 1 si
	uint8_t retardoMemoria;
	char* algoritmo_reemplazo;

} t_configuracion;

typedef struct { // estructura que se carga en la lista de memoria principal
	uint8_t idMarco; // la memoria identificara a cada marco a traves de este id
	char* contenido; // el texto que tendra esa posicion
	uint8_t posicion;
	uint8_t bitModificada;
	uint8_t bitUso;
	uint8_t posicionCargadoAMemoria;
} t_marco;

typedef struct {
	uint8_t idProc;
	uint8_t paginaDelProceso;// supongo que las paginas del proceso arrancan desde 1
	uint8_t idMarco; // si esta vacio va -1, lo que indica que se tiene que ir a buscar al swap
	uint8_t bitPagModificada; // si esta en memoria ver si fue modificada
	uint8_t bitPresencia; // 1 esta en mem, 0 no
	uint8_t posicion;
} t_TLB;

typedef struct {
	uint8_t idProc;
	uint8_t paginaDelProceso;// supongo que las paginas del proceso arrancan desde 1
	uint8_t idMarco; // si esta vacio va -1, lo que indica que se tiene que ir a buscar al swap
	uint8_t bitPagModificada; // si esta en memoria ver si fue modificada
	uint8_t bitPresencia; // 1 esta en mem, 0 no
} t_TablaDePaginas;

typedef struct {
	uint8_t idProc;
	uint8_t CantPag;
}t_iniciarProc;

typedef struct {
	uint8_t idMarco;
	uint8_t bitPresencia;
}t_marco_y_bit;

typedef struct {
	uint8_t idProc;
	char* contenido;
	uint8_t Pag;
}t_escrituraProc;

typedef struct {
	pid_t idProc;
	uint8_t pagIn;
	uint8_t pagFin;
	char* contenido;
}t_lectura;

typedef struct {
	pid_t idProc;
}t_finalizarProc;

typedef struct {
	uint8_t PID;
	uint8_t pagina;
	uint8_t socketSwap;
	uint8_t idMarco;
	char* contenido;
}t_escribir_falso;

typedef struct{
	t_marco* marco;
	uint8_t flag; //0 no lo encontro, 1 si lo encontro
}t_marco_con_flag;

typedef struct{
	t_marco* marco;
	uint8_t indice;
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
uint8_t idFuncion(char* funcion);
void aplicarFuncion(uint8_t idFuncion);
uint8_t procesarMensajesConsola(uint8_t socket, t_header* header, char* buffer);
void limpiarTLB();
void limpiarMemoria();
void volcarMemoria();
void atencionSIGUSR1();
void atencionSIGUSR2();
void asignarSeniales();

// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
//============================================================================
void revisarMemoria();
void mostrarTablaDePag();
void mostrarTLB();
void mostrarMemoria();
void reemplazar_tablaDePag(uint8_t index,t_TablaDePaginas* campoTablaDePag);
void reemplazar_TLB(uint8_t index,t_TLB* campoTLB);
void reemplazar_Memoria(uint8_t index,t_marco* campoMarco);
void leerArchivoDeConfiguracion();
void iniciar(uint8_t idProc, uint8_t cantPag, uint8_t socketCPU);
void leer(uint8_t idProc, uint8_t pag, uint8_t socketSwap, uint8_t socketCPU);
void finalizar(t_PID* estructuraFinalizar,uint8_t socketSwap);
void inicializadoCorrecto(uint8_t idProc, uint8_t cantPag);
void escribir(uint8_t idProc, uint8_t nroPag, char* textoAEscribir, uint8_t socketSwap, uint8_t socketCPU);
t_TablaDePaginas* iniciarTablaDePaginas();
t_contenido_pagina * iniciarEscrituraProc();
t_TLB* iniciarTLB();
t_marco * iniciarMarco();
t_marco_con_flag * iniciarMarcoYFlag();
t_marco_con_indice* iniciarMarcoYIndice();
t_marco_y_bit* iniciarMarcoYBit();
void* uint8_terpretarPaquete(Paquete* unPaquete, uint8_t fdReceptor);
t_marco_y_bit* buscarSiEstaEnMemoria(uint8_t idProc, uint8_t nroPag); // retorna o el id o un -1 si no esta en memoria
void escribirEnMarcoYponerBitDeModificada(uint8_t idMarco, char* contenido);
void enviarIniciarASwap(t_iniciar_swap *estructura, uint8_t socketSwap);
void enviarFinalizarASwap(t_PID *estructura, uint8_t socketSwap);
void traerDeSwapUnaPaginaDeUnProceso(uint8_t idProc, uint8_t nroDePag,uint8_t socketSwap);
void traerDeSwapUnaPaginaDeUnProcesoPorEscribir(uint8_t idProc,uint8_t nroPag, char* textoAEscribir,uint8_t socketSwap);
void cargarNuevoMarcoAMemoria(char* contenido,uint8_t PID, uint8_t pag, uint8_t flagEscritura);
bool llegoAlMaximoDelProcesoLaMemoria(uint8_t idProc);
void sacarAlMasViejoUsadoDeMemoria(uint8_t socketSwap,uint8_t PIDACargar,char* contenidoACargar,uint8_t pagACargar, uint8_t flagEscritura);
uint8_t sacarAlMasViejoUsadoDelProcesoDeMemoria(char* contenidoACargar, uint8_t PIDACargar, uint8_t pagACargar,uint8_t flagEscritura,uint8_t socketSwap);
uint8_t sacarAlPrimeroDeMemoriaDelProceso(char* contenidoACargar, uint8_t PIDACargar, uint8_t pagACargar, uint8_t socketSwap);
void sacarAlPrimeroDeMemoria(uint8_t socketSwap, uint8_t PIDACargar, char* contenidoACargar, uint8_t pagACargar);
void sacarDeMemoriaSegunClockModificado(uint8_t socketSwap, uint8_t PIDACargar,
		char* contenidoACargar, uint8_t pagACargar, uint8_t flagEscritura);
uint8_t sacaProcesoDeMemoriaSegunClockModificado(char* contenidoACargar,
		uint8_t PIDACargar, uint8_t pagACargar, uint8_t flagEscritura, uint8_t socketSwap);
t_marco_con_flag* buscarUsoEnCeroModificadaEnUno() ;
t_marco_con_flag* buscarModificadaYUsoEnCero();
t_marco_con_flag* buscarModificadaYUsoEnCeroDeProceso(uint8_t PID);
t_marco_con_flag* buscarUsoEnCeroModificadaEnUnoDeProceso(uint8_t PID);
//warning no declarado aca, entonces lo agrego
void sacarAlPrimeroDeTLB() ;
void eliminarDeTLB(uint8_t idMenor);
t_list* buscarLosMarcosDeProcesoEnMemoriaConSusIndices(uint8_t PID);
char* traerContenidoDeMarco(uint8_t idMarco);
void cargarNuevoEnTLB(uint8_t PID,uint8_t pag,uint8_t id);
void enviarACPUContenidoPaginaDeUnProcesoPorLeer(t_contenido_pagina* lecturaMandarCpu, uint8_t socketCPU);
bool estaLlenaLaMemoria();
uint8_t verificarBitDeModificada(t_marco* campoMarco, char* contenidoACargar, uint8_t PIDaCargar, uint8_t pagACargar,uint8_t flagEscritura,uint8_t socketSwap);
t_list* buscarLosMarcoYBitDeProceso(uint8_t idProc);
t_list* buscarLosMarcosDeProcesoEnMemoria( uint8_t PID);
void eliminarDeMemoria(uint8_t id);
void eliminarDeTablaDePaginas(uint8_t id);
void eliminarDeTLBSiEstaPorNuevoId(uint8_t idMenor);
void eliminarDeTablaDePaginasDefinitivamente(uint8_t id);
void eliminarDeTLBDefinitivamente(uint8_t id);
void enviarASwapEliminarProceso(uint8_t idProc);
void enviarASwapContenidoPaginaDesactualizada(uint8_t idProc, uint8_t pagina, char* contenido, uint8_t socketSwap);
void enviarRtaIniciarFalloCPU (t_PID * estructura, uint8_t socketCPU);
void enviarRtaEscribirACPU(t_contenido_pagina *estructura, uint8_t socketCPU);
void enviarIniciarAlSwap(t_iniciar_swap *estructura, uint8_t socketSwap);
void enviarEscribirAlSwap(t_contenido_pagina *estructura, uint8_t socketSwap);

void enviarRtaIniciarOkCPU (t_PID * estructura, uint8_t socketCPU);
void respuestaTraerDeSwapUnaPaginaDeUnProceso(uint8_t idProc, uint8_t pag, char* contenido, uint8_t flagEscritura,uint8_t socketCPU, uint8_t socketSwap);
t_escribir_falso* escribir_falso(uint8_t idProc, uint8_t nroPag, char* textoAEscribir, uint8_t socketSwap,uint8_t socketCPU);
t_PID* iniciar_falso(uint8_t idProc, uint8_t cantPag, uint8_t socketCPU);
t_contenido_pagina* leer_falso(uint8_t idProc, uint8_t pag, uint8_t socketSwap, uint8_t socketCPU);
t_PID* finalizar_falso(t_PID* estructuraFinalizar,uint8_t socketSwap);
t_contenido_pagina* respuestaTraerDeSwapUnaPaginaDeUnProcesoFalso(uint8_t idProc, uint8_t pag,
		char* contenido, uint8_t flagEscritura, uint8_t socketCPU, uint8_t socketSwap);
//warning no definido en test_memoria.c , entonces lo agrego aca.
char* decirHolaMundo() ;
//warning no definido en Memoria.c , entonces lo agrego aca.
void enviarFinalizarACPU(t_PID* estructuraFinalizar, uint8_t socketCPU);
t_leerDeProceso* crearEstructuraLeer();
t_contenido_pagina* iniciarContenidoPagina();
//warning no definido en funciones_Auxiliares.c , entonces lo agrego aca.
t_contenido_pagina * iniciarEscrituraProc() ;
t_escribir_falso* crearEscribirFalso();
t_contenido_pagina* respuestaTraerDeSwapUnaPaginaDeUnProcesoFalsoFalso(uint8_t idProc, uint8_t pag,
		char* contenido, uint8_t flagEscritura, uint8_t socketCPU, uint8_t socketSwap);
t_marco_con_flag* sacaProcesoDeMemoriaSegunClockModificadoFalso(char* contenidoACargar, uint8_t PIDACargar, uint8_t pagACargar, uint8_t flagEscritura, uint8_t socketSwap);
t_marco_con_flag* sacarDeMemoriaSegunClockModificadoFalso(uint8_t socketSwap, uint8_t PIDACargar,
		char* contenidoACargar, uint8_t pagACargar, uint8_t flagEscritura);

uint8_t procesarMensajesConsola(uint8_t socket, t_header* header, char* buffer);

//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
//++++++++++++++++++++++++++++++++++++funciones envio +++++++++++++++++++++++++++++++++++++++

void inicializacionDesdeCero();
void iniciarConfiguracionTLBNoHabilitada();
void iniciarConfiguracionTLBHabilitada();
void hardcodearValoresEnTLB(uint8_t PID,uint8_t id, uint8_t pag);



uint8_t getSocketCPU(uint8_t pid);
bool hayQueRegistrarPidCpu(uint8_t socket);
char* getKeyPidCpu(uint8_t pid);
void registrarPidCpu(uint8_t socket, uint8_t pid);
//========================================================================================

// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++



//===========================================================================================
t_configuracion* configuracion;
t_dictionary* conexiones;
// ----------- Contadores -------- //
uint8_t variableIdMarco,variableTLB,variableEnvejecimientoMarco,indiceClockM,variableParaFifo; // contador de paginas de la tabla de paginas
uint8_t aux;
// ----------- Listas ------------ //
t_list* listaMemoria;
t_list* listaTLB;
t_list* listaTablaDePag;
t_list* listaIndices;

// ----------- Semaforos ---------- //
pthread_mutex_t mutexListaMemoria;
pthread_mutex_t mutexListaTLB;
pthread_mutex_t mutexTablaPags;

#endif /* MEMORIA_H_ */
