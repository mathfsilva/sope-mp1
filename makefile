all: xmod	

xmod:
	cc -Wall -ggdb3 -o xmod xmod.c file.c signals.c traverse.c
	
clean:
	rm -f xmod

