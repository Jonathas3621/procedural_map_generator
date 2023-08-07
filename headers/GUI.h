#ifndef STDLIB
#define STDLIB
    #include <stdlib.h>
#endif

#ifndef SDL_GENERAL
#define SDL_GENERAL

    #include <SDL.h>
    #include <SDL_image.h>

#endif

#ifndef STDIO
#define STDIO
    #include <stdio.h>
#endif

#ifndef UTILS
#define UTILS
    #include "utils.h"
#endif

#include <SDL_video.h>

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

// Libera matrizes de superfícies (SDL_Surface) e de texturas (SDL_Texture)
void freeSdlContentMatrix(SDL_Surface ***image_matrix, SDL_Texture ***texture_matrix, orderedPair tilesMatrixDim);

// Cria superfícies a partir dos tiles e as armazena em uma matriz
int loadImages(SDL_Surface ***image_matrix, orderedPair tilesMatrixDim);

// Cria uma matriz de texturas a partir de uma matriz de superfícies
int loadTextures(SDL_Renderer *renderer, SDL_Surface ***image_matrix, SDL_Texture ***texture_matrix, orderedPair tilesMatrixDim);

// Desenha os tiles nas posições corretas gerando uma imagem na tela
void drawImage(SDL_Rect dstRect, orderedPair pos, orderedPair tilesMatrixDim, int zoom, SDL_Renderer *renderer, SDL_Texture ***texture_matrix);

// Desenha o menu na tela
void drawMenu(SDL_Renderer* renderer, button* btn, SDL_Rect* menuBar);

// Limpa o renderizador, colocando uma cor predefinida de fundo
void clearDisplay(SDL_Renderer* renderer);

// Ajusta o zoom de modo que a imagem não ultrapasse nenhuma das margens
void imageScreenAdjustment(int *zoom, orderedPair tilesMatrixSize, orderedPair screenDim, int imageSize);

orderedPair takeFirstPosition(orderedPair displayArea, int zoom);

int printTexture(SDL_Texture *texture, SDL_Renderer *renderer, orderedPair firstPosition, int zoom, orderedPair position);