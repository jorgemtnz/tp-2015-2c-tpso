dependents:
	-cd /home/utnso/tp-2015-2c-tpso/commons-library/Debug && $(MAKE) all
	-cd /home/utnso/tp-2015-2c-tpso/utiles/Debug && $(MAKE) all

dependentsClean:
	-cd /home/utnso/tp-2015-2c-tpso/commons-library/Debug && $(MAKE) clean
	-cd /home/utnso/tp-2015-2c-tpso/utiles/Debug && $(MAKE) clean


all: dependents
	$(MAKE) -C Planificador all

clean: dependentsClean
	$(MAKE) -C Planificador clean
