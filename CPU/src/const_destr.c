#include "CPU.h"

t_map* crearMapeo(){
 t_map* map = malloc(sizeof(t_map));
  if(map==NULL){
	  perror("[ERROR] No se reservo memoria para CPU>..>map");
	  		log_error(logger,
	  				"[ERROR] No se reservo memoria para CPU>..>map");
	  		exit(-1);
  }
  map->ptrCMemoriaMap = '\0';
  map->ptrTamPagina = NULL;

return map;
}

t_configuracion* crearConfiguracion(){
	t_configuracion* configuracion = malloc(sizeof(t_configuracion));
	if(configuracion ==NULL){
		  perror("[ERROR] No se reservo memoria para CPU>..>configuracion");
		  		log_error(logger,
		  				"[ERROR] No se reservo memoria para CPU>..>configuracion");
		  		exit(-1);
	  }
	configuracion->cantidad_hilos=0;
	configuracion->retardo=0;
	configuracion->vg_ipMemoria = '\0';
	configuracion->vg_ipPlanificador = '\0';
	configuracion->vg_puertoMemoria =0;
	configuracion->vg_puertoPlanificador=0;

	return configuracion;
}

t_PCB* crearPCB(){
	int i;
	t_PCB* pcb =malloc(sizeof(t_PCB));
	if(pcb ==NULL){
			  perror("[ERROR] No se reservo memoria para CPU>..>pcb");
			  		log_error(logger,
			  				"[ERROR] No se reservo memoria para CPU>..>pcb");
			  		exit(-1);
		  }
	pcb->cantInstruc=0;
	pcb->ptr_proximaInstruccion=NULL;
    for(i=0; i< CANT_RUTA; i++ )
    	pcb->path[i]='\0';

return pcb;
}
