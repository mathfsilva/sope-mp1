all: xmod

xmod: xmod.c
	g++ -o xmod xmod

clean:
	rm -f xmod