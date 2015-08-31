# tp-2015-2c-tpso
para tp, fase inicial del proyecto

ruta para la VM  /home/utnso/tp-2015-2c-tpso

utiles y commons-library son bibliotecas compartidas


cuando hagan cambios en su compu, primero hacer el git add --all , luego el git commit -m "comentario"
luego el git pull , y finalmente el git push

correr proyecto desde eclipse

Run/ Run Configurations..
doble clic en C/C++ Aplication
Environment
new
Name: LD_LIBRARY_PATH
Value:       -> Values.. y tipeo work y selecciono la que sale
me debe quedar escrito  ${workspace_loc}/commons/Debug:
repito el 6
me debe quedar   ${workspace_loc}/commons-library/Debug:${workspace_loc}/utiles/Debug
ok/Apply/close

