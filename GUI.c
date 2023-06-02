#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define IMAGE_SIZE 14

typedef struct position {
    int x;
    int y;
} position;

int main(int argc, char** argv) {

    int menuWidth =  WINDOW_WIDTH / 4;
    int backWidth = WINDOW_WIDTH - menuWidth;
    int zoom = 1;
    position pos = {0, 0};

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL: %s", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Gerador Procedural",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if(!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar a janela: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar o renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Surface* image_matrix[10][10];
    SDL_Texture* texture_matrix[10][10];
    char filename[15];

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            sprintf(filename, "tiles/%d.png", rand()%80 + 1);
            image_matrix[i][j] = IMG_Load(filename);

            if(!image_matrix[i][j]) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a imagem %s: %s", filename, SDL_GetError());
                for(int d = 0; d < i; d++) {
                    for (int b = 0; b < j; b++) {
                        SDL_FreeSurface(image_matrix[d][b]);
                    }
                }
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return -1;
            }

            texture_matrix[i][j] = SDL_CreateTextureFromSurface(renderer, image_matrix[i][j]);

            if(!texture_matrix[i][j]) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a textura da imagem %s: %s", filename, SDL_GetError());
                for(int d = 0; d < i; d++) {
                    for (int b = 0; b < j; b++) {
                        SDL_FreeSurface(image_matrix[d][b]);
                        SDL_DestroyTexture(texture_matrix[d][b]);
                    }
                }
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return -1;
            }
        }
    }

    // Limpar a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect menuBar = {WINDOW_WIDTH - menuWidth, 0, menuWidth, WINDOW_HEIGHT};

    SDL_SetRenderDrawColor(renderer, 52, 58, 64, 255);
    SDL_RenderFillRect(renderer, &menuBar);
    SDL_RenderPresent(renderer);

    zoom = backWidth / (10 * IMAGE_SIZE);
    pos.x = (backWidth - (zoom * IMAGE_SIZE * 10)) / 2;
    pos.y = (WINDOW_HEIGHT - (zoom * IMAGE_SIZE * 10)) / 2;

    SDL_Rect dstRect = {pos.x, pos.y, zoom*IMAGE_SIZE, zoom*IMAGE_SIZE};

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            SDL_RenderCopy(renderer, texture_matrix[i][j], NULL, &dstRect);
            dstRect.x += IMAGE_SIZE * zoom;

            SDL_RenderPresent(renderer);
            SDL_Delay(50);
        }
        dstRect.x = pos.x;
        dstRect.y += IMAGE_SIZE * zoom;
    }

    // Aguardar evento de saída
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

    for(int d = 0; d < 10; d++) {
        for (int b = 0; b < 10; b++) {
            SDL_FreeSurface(image_matrix[d][b]);
            SDL_DestroyTexture(texture_matrix[d][b]);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
    return 0;
}