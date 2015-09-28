#include "CPU.h"
//agregar comportamiento en cada break
void enviar(int tipoDeMensaje, void* t_estructura, int fdDestinatario) {

	switch (tipoDeMensaje) {
	case (INICIAR_PROCESO_MEM): {

		break;
	}
	case (ESCRIBIR_MEM): {

		break;
	}
	case (LEER_MEM): {

		break;
	}
	case (FIN_PROCESO_MEM): {

		break;
	}

	}

}

void* recibir(int fdReceptor) {
	size_t tamanioMensaje;

	recibirPorSocket(fdReceptor, &tamanioMensaje, sizeof(int));

	void* buffer = malloc(tamanioMensaje);

	recibirPorSocket(fdReceptor, buffer, tamanioMensaje);
	Paquete* unPaquete = deserializar(buffer, tamanioMensaje); //primer deserializado
//da un tratamiento al mensaje en especifico
	interpretarPaquete(unPaquete, fdReceptor);

	return NULL;
}
//agregar comportamiento en cada break
void* interpretarPaquete(Paquete* unPaquete, int fdReceptor) {
	void* result; // estructura que espero recibir

	switch (unPaquete->tipoDeMensaje) {
	case (RESUL_ERROR): {

	}
		break;

	case (RESUL_ESCRIBIR): {

		break;
	}
	case (RESUL_FIN): {

		break;
	}
	case (RESUL_INICIAR_PROC): {

		break;
	}
	case (RESUL_INSTR_EJEC): {

		break;
	}
	case (RESUL_LEER): {

		break;
	}
	case (RESUL_OK): {

		break;
	}


	}

	return result;
}

