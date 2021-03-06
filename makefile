all: xmod

xmod: xmod.c file.h file.c
	g++ -o xmod xmod.c file.h file.c

clean:
	rm -f xmod