all: xmod	

xmod:
	cc -Wall -o xmod xmod.c file.c signals.c transverse.c
	
clean:
	rm -f xmod

