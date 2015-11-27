dependents:
	-cd /home/utnso/tp-2015-2c-tpso/commons-library/Debug && $(MAKE) all
	-cd /home/utnso/tp-2015-2c-tpso/utiles/Debug && $(MAKE) all

dependentsClean:
	-cd /home/utnso/tp-2015-2c-tpso/commons-library/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2015-2c-tpso/utiles/Debug && $(MAKE) clean


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

