all: xmod

xmod: xmod.c
	g++ -o xmod xmod.c

clean:
	rm -f xmod