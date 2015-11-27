dependents:
	-cd ./commons-library/Debug && $(MAKE) all
	-cd ./utiles/Debug && $(MAKE) -f makefile2 all

dependentsClean:
	-cd ./commons-library/Debug && $(MAKE) clean
	-cd ./utiles/Debug && $(MAKE) -f makefile2 clean


all: dependents
	$(MAKE) -C Planificador all
	$(MAKE) -C CPU all
	$(MAKE) -C Memoria all
	$(MAKE) -C Swap all
clean: dependentsClean
	$(MAKE) -C Planificador clean
	$(MAKE) -C CPU clean
	$(MAKE) -C Memoria clean
	$(MAKE) -C Swap clean

