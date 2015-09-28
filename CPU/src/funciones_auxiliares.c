
#include "CPU.h"
//si es inicializar me devuelve el token sino -1 como error
int reconoceTokenInstruccionInicializar(char* string){
 if (string_equals(string, "inicializar"))
 return INICIAR_PROCESO_MEM;

 return -1;

}
