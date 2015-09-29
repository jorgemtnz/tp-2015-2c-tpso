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

t_PCB* crearPCB() {
	int i;
	t_PCB* pcb = malloc(sizeof(t_PCB));
	if (pcb == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>pcb");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>pcb");
		exit(-1);
	}
	pcb->cantInstruc = 0;
	pcb->ptr_proximaInstruccion = NULL;
	for (i = 0; i < CANT_RUTA; i++)
		pcb->path[i] = '\0';

	return pcb;
}

t_mProc* crear_mProc() {
	t_mProc* mProc = malloc(sizeof(t_mProc));
	if (mProc == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>mProc");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>mProc");
		exit(-1);
	}
	mProc->idProc = 0;
	mProc->pcb = crearPCB();
	mProc->tiempoEjec = 0;

	return mProc;
}

t_cpu* crearCPU() {
	t_cpu* CPUHilo = malloc(sizeof(t_cpu));
	if (CPUHilo == NULL) {
		perror("[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		log_error(logger, "[ERROR] No se reservo memoria para CPU>..>CPUHilo");
		exit(-1);
	}
	CPUHilo->idCPU = 0;
	CPUHilo->proceso = crear_mProc();
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

void destPCB(t_PCB* unPCB){
	free(unPCB);
}

void dest_mProc(t_mProc* un_mProc){
	free(un_mProc);
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
