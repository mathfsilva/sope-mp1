all: xmod	

xmod: xmod.c file.h file.c
	cc -Wall -o xmod xmod.c file.h file.c
	
clean:
	rm -f xmod

