# este readme es para especificar, comentar, explicar algo sobre el proceso ->

ruta archivo config
/home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg

ruta archivo explicacion de CPU
https://drive.google.com/open?id=1ogAHcjRKXyzwrDWwJpotMKY2jAFI3P8D2isoDXZsR9A

Para solo probar las comunicaciones lo que se puede hacer es en envioYrecepcion.c dentro de las funciones de ejecucion y en el swtich que recibe los mensajes, ir a esas funciones de ejcucion, comentar la funcionalidad y  imprimir la estructura recibida y luego mandarle otra ya hardcodeada.

#va en arguments  del run configurations
./CPU /home/utnso/tp-2015-2c-tpso/CPU/config_cpu.cfg test

LD_LIBRARY_PATH
/home/utnso/tp-2015-2c-tpso/utiles/Debug:/home/utnso/tp-2015-2c-tpso/commons-library/Debug

en hilosCpu.c debo poner que se iguale a cero la cantidad de instrucciones ejecutadas, solo cuando la CPU ese disponible, hago un if donde pregunte por este valor que esta en la cpu. Y como ya tengo el list iterate se hace para cada cpu. Debo ademas sumarle el retardo de la cpu al calculo de la cpu. tengo que sumar el retardo de la memoria. Analizar como puedo acceder a ese timpo de retardo de la memoria, sin tener un contador para no tener espera activa. Quizas me pueda mandar memoria un mensaje al comienzo con su valor de retardo  y que este sumado al retardo que tiene del swap y guardarlo en la cpu, o tener en cuenta el tiempo en que se fue y el tiempo en que regreso.
