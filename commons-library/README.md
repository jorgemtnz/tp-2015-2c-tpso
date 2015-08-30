Las librerias commons ya están instaladas localmente. Hay que ver cuando otro
 miembro descargue esto. Si las puede usar.
se debe dar clic derecho en el proyecto que se quiere usar y luego en C/C++ ir a paths & symbols
y al botón add para agregar las commons.

Problema: instalar las commons descargarlas, no se instalan.
consecuencia: si se hace un clone de las commons, crea un .git 
dentro de estas, que permite que luego desde consola parados 
en las /commons se instalen, con el comando correspondiente del tutorial instalación.
“para su utilizaciòn desde eclipse coloque commons-library dentro del workspace, 
luego desde el eclipse cree un proyecto de biblioteca dinámica y coloque como 
nombre de esta commons-library. Esto va a hacer que eclipse cree la configuración
para utilizar este proyecto como una biblioteca dinámica. Si se requiere que un 
proyecto utilice a esta, siga los pasos para configurar la biblioteca compartida como
 parte de este y como path de los include coloque el contenido de la carpet src. Por lo 
que los includes quedarían como:
include<commons/log.h>
solo agrego la carpeta src al proyecto de la libreria., le pongo click derecho en el
 proyecto/propiedades/ c/c++build/settings/miscellaneous/ opción -fPIC.
luego en el proyecto que las vaya a usar , clic derecho propiedades/c/c++general/paths
 and symbols y marco opción commons-library. listo ya ahora si me reconoce
 include<commons/log.h>  o este otro include<commons/collections/list.h>

observación: en el video no se copian los test al proyecto de la biblioteca compartida.
 Precisamente en los test es donde me daba el error anteriormente