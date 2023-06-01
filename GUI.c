#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define IMAGE_SIZE 14

int main(int argc, char** argv) {

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
    char* filename;

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

    int menuWidth = WINDOW_WIDTH/5;
    int menuHeight = WINDOW_HEIGHT;

    SDL_Rect menuBar = {WINDOW_WIDTH - menuWidth, 0, menuWidth, menuHeight};

    SDL_SetRenderDrawColor(renderer, 173, 181, 189, 255);
    SDL_RenderFillRect(renderer, &menuBar);
    SDL_RenderPresent(renderer);

    SDL_Rect dstRect = {0, 0, IMAGE_SIZE, IMAGE_SIZE};

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            SDL_RenderCopy(renderer, texture_matrix[i][j], NULL, &dstRect);
            dstRect.x += IMAGE_SIZE;

            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }
        dstRect.x = 0;
        dstRect.y += IMAGE_SIZE;
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