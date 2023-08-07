#include "headers/GUI.h"

button* createButton(int x, int y, int w, int h, char *label, SDL_Texture *texture) {
    
    button* btn;
    btn = malloc(sizeof(button));
    if(!btn) {
        fprintf(stderr, "Erro ao criar botão");
        return NULL;
    }

    SDL_Rect rect = {x, y, w, h};
    btn->boddy = rect;

    if(texture) btn->texture = texture;
    if(label) btn->label = label;

    return btn;
}

void freeButton(button *btn) {

    if(btn) free(btn);
}

int isPressed(SDL_Rect item, SDL_Event event) {
    int mouseX = event.button.x;
    int mouseY = event.button.y;

    if(mouseX > item.x && mouseX < (item.x + item.w) &&
        mouseY > item.y && mouseY < (item.y + item.h))
    {
        return 1;
    }

    return 0;
}

void freeSdlDisplay(SDL_Window *window, SDL_Renderer *renderer){
    if(window) SDL_DestroyWindow(window);
    if(renderer) SDL_DestroyRenderer(renderer);
}

void freeSdlContentMatrix(SDL_Surface ***image_matrix, SDL_Texture ***texture_matrix, orderedPair tilesMatrixDim) {
    for(int d = 0; d < tilesMatrixDim.y; d++) {
        for (int b = 0; b < tilesMatrixDim.x; b++) {
            if(image_matrix[d][b]) SDL_FreeSurface(image_matrix[d][b]);
            if(texture_matrix[d][b]) SDL_DestroyTexture(texture_matrix[d][b]);
        }
    }
}

int loadImages(SDL_Surface ***image_matrix, orderedPair tilesMatrixDim) {
    char filename[15];

    for(int i = 0; i < tilesMatrixDim.y; i++) {
        for(int j = 0; j < tilesMatrixDim.x; j++) {
            sprintf(filename, "tiles/%d.png", rand()%80 + 1);
            image_matrix[i][j] = IMG_Load(filename);

            if(!image_matrix[i][j]) {
                orderedPair pair = {i,j};
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a imagem %s: %s", filename, SDL_GetError());
                freeSdlContentMatrix(image_matrix, NULL, pair);
                return -1;
            }
        }
    }
    return 0;
}

int loadTextures(SDL_Renderer *renderer, SDL_Surface ***image_matrix, SDL_Texture ***texture_matrix, orderedPair tilesMatrixDim) {
    for(int i = 0; i < tilesMatrixDim.y; i++) {
        for(int j = 0; j < tilesMatrixDim.x; j++) {
            texture_matrix[i][j] = SDL_CreateTextureFromSurface(renderer, image_matrix[i][j]);

            if(!texture_matrix[i][j]) {
                orderedPair pair = {i,j};
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar textura: %s", SDL_GetError());
                freeSdlContentMatrix(image_matrix, texture_matrix, pair);
                return -1;
            }
        }
    }
    return 0;
}

void drawImage(SDL_Rect dstRect, orderedPair pos, orderedPair tilesMatrixDim, int zoom, SDL_Renderer *renderer, SDL_Texture ***texture_matrix) {
    dstRect.w *= zoom;
    dstRect.h *= zoom;

    for(int i = 0; i < tilesMatrixDim.y; i++) {
        for(int j = 0; j < tilesMatrixDim.x; j++) {
            SDL_RenderCopy(renderer, texture_matrix[i][j], NULL, &dstRect);
            dstRect.x += IMAGE_SIZE * zoom;

            SDL_RenderPresent(renderer);
            //SDL_Delay(1);
        }
        dstRect.x = pos.x;
        dstRect.y += IMAGE_SIZE * zoom;
    }
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

orderedPair takeFirstPosition(orderedPair displayArea, int zoom) {

    if(10 * 14 * zoom > displayArea.x || 10 * 14 * zoom > displayArea.y) zoom = 1;

    int remainingSpaceCol = displayArea.y - 10 * 14 * zoom;
    int remainingSpaceLin = displayArea.x - 10 * 14 * zoom;

    orderedPair firstPosition = {remainingSpaceLin/2, remainingSpaceCol/2};

    return firstPosition;
}


int printTexture(SDL_Texture *texture, SDL_Renderer *renderer, orderedPair firstPosition, int zoom, orderedPair position) {

    if(!texture) return 1;
    int scale = 14 * zoom;

    SDL_Rect dstRect = {firstPosition.y + scale * position.y, firstPosition.x + scale * position.x, IMAGE_SIZE*zoom, IMAGE_SIZE*zoom};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_RenderPresent(renderer);
    
    return 0;
}