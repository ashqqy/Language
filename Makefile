all: 
	make -f ./Frontend/Makefile
	make -f ./Backend/Makefile
	cd Processor && make -f ./Makefile
