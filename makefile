all: xmod	

xmod: xmod.c file.h file.c	
	cc -Wall xmod xmod.c file.h file.c
	
clean: 
	rm -f xmod
