CC=gcc
PNGFLAGS=-LC:\Dev\lpng1639\library -IC:\Dev\lpng1639\include -lpng -lz
DEPURATION=-g

main: main.c
	$(CC) -o main main.c $(PNGFLAGS)

mainD: main.c
	$(CC) $(DEPURATION) main.c $(PNGFLAGS)

clear: main.exe
	del main.exe
