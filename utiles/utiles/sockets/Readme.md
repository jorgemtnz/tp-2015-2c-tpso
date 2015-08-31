Como usar con otros proyectos

Descargamos el proyecto
1) Hacemos un pull desde nuestro repo
2) Compilamos
3) Listo, ya podemos incluir el header en otros proyectos

Cuando queremos agregar la libreria a otro proyecto,
0) Tener abierto en el proyect explorer el proyecto requerido
1) Creamos un nuevo proyecto
2) Vamos a las propiedades del proyecto >  c/c++ General > Paths and Symbols > References
4) Seleccionamos el proyecto Sockets (tiene que estar abierto por el punto 0)
5) En el codigo de nuestro nuevo paquete llamamos a #include <src/sockets.h>
6) Compilamos

Para ejecutar el programa:
1) run > run connfigurations 
2) Vamos al ejecutable, y en Enviroment agregamos LD_LIBRARY_PATH y como value seleccionamos variables>workspace>/Sockets/Debug


Fuente = https://www.youtube.com/watch?v=s5ac8CPDkMg&feature=youtu.be

Para usar la funcion select
1) En el archivo que necesitemos, incluimos #include <sys/types.h>
2) Creamos una coleccion de files descrriptors mediante la declaracion fd_set miColeccion;

Operaciones con la coleccion
1) FD_ZERO(&miColeccion)       >> Llenamos la coleccion con ceros
2) FD_SET(unFD,&miColeccion)   >> Agregamos files descriptors (fds) a la coleccion
3) FD_CLR(unFD,&miColeccion)   >> Quitamos fds de la coleccion
4) FD_ISSET(unFD,&miColeccion) >> Pertenencia: indica si unFD esta seteado (pertenece a la coleccion)

Ejecutar programas compilados que usen librerias compartidas
1) Ir a la consola
2) cd /home/utnso
3) leafpad .bashrc
4) Al final de todo el archivo agregamos las lineas
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/TPOperativos/delEquipo/Debug
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/TPOperativos/commons-library/Debug

