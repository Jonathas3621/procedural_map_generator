#include "../include/GUI.h"

button* createButton(int x, int y, int w, int h, char *label, SDL_Texture *texture) {
    
    button* btn;
    btn = (button*) malloc(sizeof(button));
    if(!btn) {
        fprintf(stderr, "Erro ao criar botão");
        return NULL;
    }

    SDL_Rect rect = {x, y, w, h};
    btn->boddy = rect;

    btn->texture = texture;
    if(label) btn->label = label;

    return btn;
}

void freeButton(button *btn) {
    if(btn) free(btn);
}

int isPressed(SDL_Rect item, SDL_Event event) {
    int mouseX = event.button.x;
    int mouseY = event.button.y;
    int pressed = 0;

    if(mouseX > item.x && mouseX < (item.x + item.w) &&
        mouseY > item.y && mouseY < (item.y + item.h))
    {
        pressed = 1;
    }

    return pressed;
}

void freeSdlDisplay(SDL_Window *window, SDL_Renderer *renderer){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
}

void drawMenu(SDL_Renderer* renderer, button* btn, SDL_Rect* menuBar) {
    // Desenhar barra de menu
    SDL_SetRenderDrawColor(renderer, 38,38,46, 255);
    SDL_RenderFillRect(renderer, menuBar);

    // Desenhar botão de geração de mapa
    SDL_SetRenderDrawColor(renderer, 52,52,59, 255);
    SDL_RenderFillRect(renderer, &(btn->boddy));
}

void clearDisplay(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 29,27,34, 255);
    SDL_RenderClear(renderer);
}

void imageScreenAdjustment(int *zoom, orderedPair tilesMatrixSize, orderedPair screenDim, int imageSize) {
    SDL_DisplayMode currentMode;
    int displayIndex = 0;
    int proport;

    if(((float)tilesMatrixSize.x/(float)tilesMatrixSize.y) > 1) {

        proport = screenDim.x / (tilesMatrixSize.x * imageSize);
        *zoom = proport > 0 ? proport : 1; 
    }
    else {

        proport = screenDim.y / (tilesMatrixSize.y * imageSize);
        *zoom = proport > 0 ? proport : 1;
    }
}

orderedPair centeringBlock(orderedPair dim, orderedPair displayArea) {

    int remainingSpaceCol = displayArea.y - dim.y;
    int remainingSpaceLin = displayArea.x - dim.x;

    orderedPair firstPosition = {remainingSpaceLin/2, remainingSpaceCol/2};

    return firstPosition;
}

int printTexture(SDL_Texture *texture, SDL_Renderer *renderer, orderedPair firstPosition, float zoom, orderedPair position) {

    if(!texture) return 1;
    int scale = (int) 14 * zoom;

    SDL_Rect dstRect = {firstPosition.y + scale * position.y, firstPosition.x + scale * position.x, IMAGE_SIZE*zoom, IMAGE_SIZE*zoom};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_RenderPresent(renderer);
    
    return 0;
}