#ifndef STDLIB
#define STDLIB
    #include <stdlib.h>
#endif

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#ifndef SDL_GENERAL
#define SDL_GENERAL

    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>

#endif

#ifndef STDIO
#define STDIO
    #include <stdio.h>
#endif

#ifndef UTILS
#define UTILS
    #include "utils.h"
#endif

#include <SDL2/SDL_video.h>

#define IMAGE_SIZE 14

// Estrutura usada para abstrair objetos clicáveis
typedef struct button {
    SDL_Rect boddy;
    SDL_Texture *texture; 
    char *label;
} button;

// Construstor da struct button
button* createButton(int x, int y, int w, int h, char *label, SDL_Texture *texture);

// Libera a memória alocada para structs button
void freeButton(button *btn);

// Confere se determinado botão foi clicado
int isPressed(SDL_Rect item, SDL_Event event);

// Libera a memória alocada para janelas (SDL_Window) e renderizadores (SDL_Renderer)
void freeSdlDisplay(SDL_Window *window, SDL_Renderer *renderer);

// Desenha o menu na tela
void drawMenu(SDL_Renderer* renderer, button* btn, SDL_Rect* menuBar);

// Limpa o renderizador, colocando uma cor predefinida de fundo
void clearDisplay(SDL_Renderer* renderer);

// Ajusta o zoom de modo que a imagem não ultrapasse nenhuma das margens
void imageScreenAdjustment(int *zoom, orderedPair tilesMatrixSize, orderedPair screenDim, int imageSize);

orderedPair centeringBlock(orderedPair dim, orderedPair displayArea);

int printTexture(SDL_Texture *texture, SDL_Renderer *renderer, orderedPair firstPosition, float zoom, orderedPair position);