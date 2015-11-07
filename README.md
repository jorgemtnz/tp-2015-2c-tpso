# tp-2015-2c-tpso

ruta para la VM  /home/utnso/tp-2015-2c-tpso

utiles y commons-library son bibliotecas compartidas
git status les va a mostrar en el branch en que estan. 
git show branch mostrara los branch que hay
git checkout nombreBranch para cambiar de un branch a otro branch
cuando hagan cambios en su compu, primero hacer el git add --all , luego el git commit -m "comentario"
luego el git pull nombreBranch, y finalmente el git push nombreBranch

para unir branchs; parado en el master
git merge nombreBranch    entonces el master se fusiona con el nombrebranch o hacerlo a la inversa y la fusion es al reves.
desde la rama que  quiero unir git rebase nombreBranch(a la que quiero ser fusionado) para fusionar y mantener los commits de forma secuencial.


correr proyecto desde eclipse

1.  Run/ Run Configurations..
2.  doble clic en C/C++ Aplication Environment
3. new
4. Name: LD_LIBRARY_PATH
5. Value:       -> Values.. 
6. y tipeo work y selecciono la que sale
me debe quedar escrito  ${workspace_loc}/commons/Debug:
repito el 5 y 6
7. me debe quedar   ${workspace_loc}/commons-library/Debug:${workspace_loc}/utiles/Debug
8. ok/Apply/close
9. o puedo copiar directamente  el 7  y listo :)

