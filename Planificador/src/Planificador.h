#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

// +++++++++++++++++++++++++++++++++++++++ Includes +++++++++++++++++++++++++++++++++++++
#include "../test/test.h"
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
#include <utiles/files.h>
#include <utiles/protocolo.h>
// +++++++++++++++++++++++++++++++++++++++ Define +++++++++++++++++++++++++++++++++++++
//====================================================================================
#define NUMEROFUNCIONESCONSOLA 4


// +++++++++++++++++++++++++++++++++++++++ Estructuras +++++++++++++++++++++++++++++++++++++
typedef struct {
	char* puertoEscucha;
	char* algorimoPlanificacion;
	char* quantum;
} t_configuracion;

typedef struct {
	uint8_t cantidadCiclos;
	bool finalizoEntradaSalida;
	t_pcb* pcb;
} t_estado_entrada_salida;

typedef struct {
	uint8_t cantidadCiclos;
	t_pcb* pcb;
} t_pcb_entrada_salida;


//=======================================================================================



// +++++++++++++++++++++++++++++++++++++++ Prototipos +++++++++++++++++++++++++++++++++++++
//=======================================================================================
// Funciones Constructoras crea los malloc de las estructuras e inicializa
//============================================================

// Funciones Destructoras hace el free de las estructuras para las que se hizo un malloc
//========================================================================


// +++++++++++++++++++++++++++++++++++Funciones Auxiliares
void crearLogger();
void leerArchivoDeConfiguracion(int argc, char *argv[]);
int putsConsola (const char *msg);
int printConsola(const char *formato, ...);
bool cpuDesconectada(void *cpu);
char* obtenerNombreProc(char* ruta);
void cerrarArchivoEspacioDeDatos(int fd, t_log* logger);
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
int procesarMensajes(int socket, t_header* header, char* buffer, t_tipo_notificacion tipoNotificacion, void* extra, t_log* logger);
int procesarMensajesConsola(int socket, t_header* header, char* buffer);
void procesarMensajesSegunTipo(int socket, t_header* header, char* buffer);
void procesar_RESUL_EJECUCION_OK(int socket, t_header* header, t_respuesta_ejecucion* respuestaEjecucion);
void procesar_ENTRADA_SALIDA(int socket, t_header* header, t_respuesta_ejecucion* respuestaEjecucion);
//++++++++++++++++++++++++++++++++++++comandos +++++++++++++++++++++++++++++++++++++++
int correrPrograma(int socket, t_header* header, char* buffer);
int correrPath(int socket, t_header* header, char* buffer);
int finalizarPid(int socket, t_header* header, char* buffer);
int ps(int socket, t_header* header, char* buffer);
int cpu(int socket, t_header* header, char* buffer);
//++++++++++++++++++++++++++++++++++++Comando finalizar pid +++++++++++++++++++++++++++++++
bool existePID(int pid);
//++++++++++++++++++++++++++++++++++++planificacion +++++++++++++++++++++++++++++++++++++++
void crearPlanificacion(char* nombreAlgoritmo, char* quantum);
t_pcb* crearPcb(char* rutaArchivoMcod);
t_cpu_ref* crearCpuRef();
void ejecutarProceso(t_pcb* pcb);
uint8_t crearPid();
t_list* getColaDeFinalizados();
t_list* getColaDeEntradaSalida();
t_list* getColaDeListos();
t_list* getColaDeNuevos();
void ejecutarPlanificadorLargoPlazo();
t_cpu_ref* obtenerCPUDisponible();
void correrProcesoEnCpu(t_pcb* pcb, t_cpu_ref* cpu);
void ejecucionAFinalizado(t_pcb* pcb);
t_cpu_ref* obtenerCPUEjecutandoPcb(t_pcb* pcb);
void quitarProcesoDeCpu(t_cpu_ref* cpu);

void ejecucionAColaDeListos(t_pcb* pcb);
void imprimirRespuestasDeInstrucciones(t_respuesta_ejecucion* respuestaEjecucion);
//++++++++++++++++++++++++++++++++++++entrada salida +++++++++++++++++++++++++++++++++++++++
void *ejecutarEntradaSalida(void *param);
//++++++++++++++++++++++++++++++++++++global planificador +++++++++++++++++++++++++++++++++++++++
char* crearNombreCPU();
void registrarNuevaCPU(int socket);
bool cpuConSocket(void *cpu, int socket);
void desregistrarCPUConectada(int socket);

//++++++++++++++++++++++++++++++++++++ ps +++++++++++++++++++++++++++++++++++++++
//Genera los mensajes segun el anexo, para una lista de PCBs, y le pone el estado que viene como parametro
//Devuelve la lista de mensajes
t_list* generarMensajesPsLista(t_list* listaPcb, char* estado);
//Recorre los elementos del dictionary, las keys son: Listo, Ejecutando, Bloqueado, en el value te viene una t_list* listaPcb
//para cada elemento del t_dictionary se llama a generarMensajesPsLista(value, key) y
//se recorre la lista que te devuelve, y cada elemento se agrega a una lista resultado
//Devuelve la lista resultado
t_list* generarMensajesPsDictionary(t_dictionary* listas);
t_list* generarMensajesPsLista(t_list* listaPcb, char* estado);

//========================================================================================


// +++++++++++++++++++++++++++++++++++ Variables Globales +++++++++++++++++++++++++++++++++++

t_configuracion* configuracion;
t_dictionary* conexiones;
uint8_t* proximoPid;
t_planificacion* planificacion;
t_list* colaDeFinalizados;
t_list* colaDeListos;
t_list* colaDeNuevos;
t_list* colaDeEntradaSalida;
t_list* listaCPUs;

t_estado_entrada_salida estadoEntradaSalida;
pthread_mutex_t mutexEstadoEntradaSalida;
pthread_mutex_t mutexHayEntradaSalidaParaEjecutar;

int c;

//===========================================================================================




//test
char* decirHolaMundo();

#endif /* PLANIFICADOR_H_ */
