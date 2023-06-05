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

typedef struct button {
    SDL_Rect boddy;
    SDL_Texture *texture; 
    char *label;
} button;

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

void freeSdlContentMatrix(SDL_Surface ***image_matrix, SDL_Texture ***texture_matrix, int lines, int columns) {
    for(int d = 0; d < lines; d++) {
        for (int b = 0; b < columns; b++) {
            if(image_matrix[d][b]) SDL_FreeSurface(image_matrix[d][b]);
            if(texture_matrix[d][b]) SDL_DestroyTexture(texture_matrix[d][b]);
        }
    }
}

int loadImages(char *filename, SDL_Surface ***image_matrix, int lines, int columns) {
    for(int i = 0; i < lines; i++) {
        for(int j = 0; j < columns; j++) {
            sprintf(filename, "tiles/%d.png", rand()%80 + 1);
            image_matrix[i][j] = IMG_Load(filename);

            if(!image_matrix[i][j]) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a imagem %s: %s", filename, SDL_GetError());
                freeSdlContentMatrix(image_matrix, NULL, i, j);
                return -1;
            }
        }
    }
    return 0;
}

int loadTextures(SDL_Renderer *renderer, SDL_Surface ***image_matrix, SDL_Texture ***texture_matrix, int lines, int columns) {
    for(int i = 0; i < lines; i++) {
        for(int j = 0; j < columns; j++) {
            texture_matrix[i][j] = SDL_CreateTextureFromSurface(renderer, image_matrix[i][j]);

            if(!texture_matrix[i][j]) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar textura: %s", SDL_GetError());
                freeSdlContentMatrix(image_matrix, texture_matrix, i, j);
                return -1;
            }
        }
    }
    return 0;
}

void drawImage(SDL_Rect dstRect, position pos, int zoom, SDL_Renderer *renderer, SDL_Texture ***texture_matrix) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            SDL_RenderCopy(renderer, texture_matrix[i][j], NULL, &dstRect);
            dstRect.x += IMAGE_SIZE * zoom;

            SDL_RenderPresent(renderer);
            SDL_Delay(15);
        }
        dstRect.x = pos.x;
        dstRect.y += IMAGE_SIZE * zoom;
    }
}

void drawMenu(SDL_Renderer* renderer, button* btn, SDL_Rect* menuBar) {
    // Desenhar barra de menu
    SDL_SetRenderDrawColor(renderer, 52, 58, 64, 255);
    SDL_RenderFillRect(renderer, menuBar);

    // Desenhar botão de geração de mapa
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &(btn->boddy));
}

void clearDisplay(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

int main(int argc, char** argv) {

    int menuWidth =  WINDOW_WIDTH / 4;
    int backWidth = WINDOW_WIDTH - menuWidth;
    int zoom = 1;
    position pos = {0, 0};

    //INICIALIZAÇÃO DAS ESTRUTURAS

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
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

    //CRIAÇÃO DE RENDERIZÁVEIS
    int tam = 10;

    SDL_Surface*** image_matrix = malloc(tam * sizeof(SDL_Surface*));
    for(int c=0; c<tam; c++) image_matrix[c] = malloc(tam * sizeof(SDL_Surface*));

    SDL_Texture*** texture_matrix = malloc(tam * sizeof(SDL_Texture*));
    for(int c=0; c<tam; c++) texture_matrix[c] = malloc(tam * sizeof(SDL_Texture*));

    char filename[15];

    if(loadImages(filename, image_matrix, tam, tam)) {
        freeSdlDisplay(window, renderer);
        SDL_Quit();
        return -1;
    }

    if(loadTextures(renderer, image_matrix, texture_matrix, tam, tam)) {
        freeSdlDisplay(window, renderer);
        SDL_Quit();
        return -1;
    }

    SDL_Rect menuBar = {WINDOW_WIDTH - menuWidth, 0, menuWidth, WINDOW_HEIGHT};

    char label1[] = "Novo Mapa"; 
    button *genNewMap = createButton(menuBar.x + menuBar.w/4, menuBar.y + 50, menuBar.w/2, 90, label1, NULL);

    //RENDERIZAÇÃO

    // Limpar a tela
    clearDisplay(renderer);

    // Desenhar menu
    drawMenu(renderer, genNewMap, &menuBar);

    SDL_RenderPresent(renderer);

    zoom = backWidth / (10 * IMAGE_SIZE);
    pos.x = (backWidth - (zoom * IMAGE_SIZE * 10)) / 2;
    pos.y = (WINDOW_HEIGHT - (zoom * IMAGE_SIZE * 10)) / 2;

    SDL_Rect dstRect = {pos.x, pos.y, zoom*IMAGE_SIZE, zoom*IMAGE_SIZE};

    //SDL_Rect *dstRect, int zoom, SDL_Render renderer, SDL_Texture*** texture_matrix
    drawImage(dstRect, pos, zoom, renderer, texture_matrix);

    // Aguardar evento de saída
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN) {
            if(isPressed(genNewMap->boddy, event)) 

                clearDisplay(renderer);
                drawMenu(renderer, genNewMap, &menuBar);
                drawImage(dstRect, pos, zoom, renderer, texture_matrix);
                //printf("MouseX: %d, MouseY: %d", (int) event.button.x, (int)event.button.y);
        }
    }

    freeSdlContentMatrix(image_matrix, texture_matrix, tam, tam);

    for(int c=0; c<tam; c++) free(image_matrix[c]);
    free(image_matrix);

    for(int c=0; c<tam; c++) free(texture_matrix[c]);
    free(texture_matrix);

    freeSdlDisplay(window, renderer);
    SDL_Quit();
    return 0;
}