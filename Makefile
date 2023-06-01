CC=gcc
PNGFLAGS=-LC:\Dev\lpng1639\library -IC:\Dev\lpng1639\include -lpng -lz
DEPURATION=-g

SDL_PATH=-IC:\Dev\SDL2-2.26.5\x86_64-w64-mingw32\include -LC:\Dev\SDL2-2.26.5\x86_64-w64-mingw32\lib
SDL_FLAGS=-lmingw32 -lSDL2main -lSDL2

SDL_IMAGE_PATH=-IC:\Dev\SDL2_image-2.6.3\x86_64-w64-mingw32\include -LC:\Dev\SDL2_image-2.6.3\x86_64-w64-mingw32\lib
SDL_IMAGE_FLAGS=-lSDL2_image

main: main.c
	$(CC) -o main main.c $(PNGFLAGS)

main_d: main.c
	$(CC) $(DEPURATION) main.c $(PNGFLAGS)

gui: GUI.c
	$(CC) -o gui GUI.c $(SDL_PATH) $(SDL_IMAGE_PATH) $(SDL_FLAGS) $(SDL_IMAGE_FLAGS)

clear: *.exe
	del *.exe

clear_im: *.png
	del *.png

clear_all: *.exe *.png
	del *.exe *.png main.exe