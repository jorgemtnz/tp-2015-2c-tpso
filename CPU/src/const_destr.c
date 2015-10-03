#include "CPU.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------FUNCIONES CONTRUCTORAS-------------------------
t_instruccion* creaInstruccion(){
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	if (instruccion==NULL){
		perror("[ERROR] No se reservo memoria para CPU>..>instruccion");
				log_error(logger, "[ERROR] No se reservo memoria para CPU>..>instruccion");
				exit(-1);
	}
	//instruccion->instruccion_separada[0]=NULL;
	instruccion->ptrParteLeida='\0';
	instruccion->ptrComienzoInstruccion='\0';
	return instruccion;
}

t_map* crearMapeo() {
	t_map* map = malloc(sizeof(t_map));
	if (map == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>map");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>map");
		exit(-1);
	}
	map->ptrCMemoriaMap = '\0';
	map->ptrTamPagina = 0;
    //map->bufferInstrucciones[0]=NULL;
    map->cantidadInstrucciones =0;
	return map;
}

t_configuracion* crearConfiguracion() {
	t_configuracion* configuracion = malloc(sizeof(t_configuracion));
	if (configuracion == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>configuracion");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>configuracion");
		exit(-1);
	}
	configuracion->cantidad_hilos = 0;
	configuracion->retardo = 0;
	configuracion->vg_ipMemoria = '\0';
	configuracion->vg_ipPlanificador = '\0';
	configuracion->vg_puertoMemoria = 0;
	configuracion->vg_puertoPlanificador = 0;

	return configuracion;
}

t_cpu* crearCPU() {
	t_cpu* CPUHilo = malloc(sizeof(t_cpu));
	if (CPUHilo == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		exit(-1);
	}
	CPUHilo->idCPU = 0;
	CPUHilo->estado = DISPONIBLE;

	return CPUHilo;
}

t_ProcCPU* crearProcCPU() {

	t_ProcCPU* procCPU = malloc(sizeof(t_ProcCPU));
	if (procCPU == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>procCPU");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>procCPU");
		exit(-1);
	}
	procCPU->contadorIdCPU = 0;
	procCPU->listaCPU = list_create();

	return procCPU;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------FUNCIONES DESTRUCTORAS------------------------------
void destMap(t_map* unMap) {
	free(unMap);
}

void destConfig(t_configuracion* unaConfig) {
	free(unaConfig);
}

void destHiloCPU(t_cpu* unHiloCPU){
   free(unHiloCPU);
}

void destProcCPU(t_ProcCPU* unCPU){
	list_destroy_and_destroy_elements(unCPU->listaCPU, (void*)destHiloCPU);
	free(unCPU);
}

void destInstruccion(t_instruccion* unaInstruccion){
	free(unaInstruccion);
}

void destVectorInstruccion(char** vectorInstruccion){
	free ( vectorInstruccion);
}
