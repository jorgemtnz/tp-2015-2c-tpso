dependents:
	-cd ./commons-library/Debug && $(MAKE) all
	-cd ./utiles/Debug && $(MAKE) all

dependentsClean:
	-cd ./commons-library/Debug && $(MAKE) clean
	-cd ./utiles/Debug && $(MAKE) clean


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

