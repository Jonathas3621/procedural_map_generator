CC=gcc
PNGFLAGS=-LC:\Dev\lpng1639\library -IC:\Dev\lpng1639\include -lpng -lz
DEPURATION=-g

main: main.c
	$(CC) -o main main.c $(PNGFLAGS)

main_d: main.c
	$(CC) $(DEPURATION) main.c $(PNGFLAGS)

clear: main.exe
	del main.exe

clear_im: *.png
	del *.png

clear_d: a.exe
	del a.exe

clear_all: a.exe *.png main.exe
	del a.exe *.png main.exe