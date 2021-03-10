all: xmod	

xmod: xmod.c file.h file.c
	cc -Wall -o xmod xmod.c file.h file.c signals.h signals.c
	
clean:
	rm -f xmod

