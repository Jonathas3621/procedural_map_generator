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

    if (SDL_GetCurrentDisplayMode(displayIndex, &currentMode) == 0) {
        if(((float)tilesMatrixSize.x/(float)tilesMatrixSize.y) > 1) {
            *zoom = screenDim.x / (tilesMatrixSize.x * imageSize); 
        }
        else {
            *zoom = screenDim.y / (tilesMatrixSize.y * imageSize);
        }
        return;
    } else {
        printf("Erro ao obter a resolução de tela: %s\n", SDL_GetError());
    }
    *zoom = 1;
}

int main(int argc, char** argv) {

    //Tamanho de tela padrão
    int windowWidth = 1000;
    int windowHeight = 600;

    // variáveis gerais (Valores padrão)
    orderedPair pos = {0, 0};
    int zoom = 1;
    int btnHeight = 40;
    int spacing = 40;

    //INICIALIZAÇÃO DAS ESTRUTURAS

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL: %s", SDL_GetError());
        return -1;
    }

    //Criando tela
    SDL_Window* window = SDL_CreateWindow("Gerador Procedural",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar a janela: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Atualizando variáveis de tamanho de tela
    SDL_DisplayMode currentMode;
    if (!SDL_GetCurrentDisplayMode(0, &currentMode)) {
        windowWidth = currentMode.w;
        windowHeight = currentMode.h;
    }

    int menuWidth =  windowWidth / 4;
    int backWidth = windowWidth - menuWidth;

    //Criando Renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar o renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //CRIAÇÃO DE RENDERIZÁVEIS
    orderedPair tilesMatrixDim = {20, 20}; //(columns(x)(w), lines(y)(h)) 
    orderedPair screenDim = {backWidth, windowHeight};

    SDL_Surface*** image_matrix = malloc(tilesMatrixDim.y * sizeof(SDL_Surface*));
    for(int c=0; c<tilesMatrixDim.y; c++) image_matrix[c] = malloc(tilesMatrixDim.x * sizeof(SDL_Surface*));

    SDL_Texture*** texture_matrix = malloc(tilesMatrixDim.y * sizeof(SDL_Texture*));
    for(int c=0; c<tilesMatrixDim.y; c++) texture_matrix[c] = malloc(tilesMatrixDim.x * sizeof(SDL_Texture*));

    char filename[15];
    if(loadImages(image_matrix, tilesMatrixDim)) {
        freeSdlDisplay(window, renderer);
        SDL_Quit();
        return -1;
    }

    if(loadTextures(renderer, image_matrix, texture_matrix, tilesMatrixDim)) {
        freeSdlDisplay(window, renderer);
        SDL_Quit();
        return -1;
    }

    SDL_Rect menuBar = {windowWidth - menuWidth, 0, menuWidth, windowHeight};

    btnHeight = windowHeight /20;

    char label1[] = "Novo Mapa"; 
    button *genNewMap = createButton(menuBar.x + spacing, menuBar.y + spacing, menuBar.w - 2*spacing, btnHeight, label1, NULL);

    char label2[] = "Sair";
    button *exit = createButton(menuBar.x + spacing, menuBar.h - (spacing + btnHeight), menuBar.w - 2*spacing, btnHeight, label2, NULL);

    char label3[] = "+";
    button *zoomIn = createButton(menuBar.x + spacing, menuBar.h - 2*(spacing + btnHeight), (menuBar.w - 3*spacing)/2, btnHeight, label3, NULL);

    char label4[] = "-";
    button *zoomOut = createButton(windowWidth - (spacing + (menuBar.w - 3*spacing)/2), menuBar.h - 2*(spacing + btnHeight), (menuBar.w - 3*spacing)/2, btnHeight, label4, NULL);

    //RENDERIZAÇÃO

    // Limpar a tela
    clearDisplay(renderer);

    // Desenhar barra de menu
    SDL_SetRenderDrawColor(renderer, 38,38,46, 255);
    SDL_RenderFillRect(renderer, &menuBar);

    // Desenhar botões do menu
    SDL_SetRenderDrawColor(renderer, 52,52,59, 255);
    SDL_RenderFillRect(renderer, &(genNewMap->boddy));

    SDL_RenderFillRect(renderer, &(exit->boddy));

    SDL_RenderFillRect(renderer, &(zoomIn->boddy));

    SDL_RenderFillRect(renderer, &(zoomOut->boddy));

    //Atualiza o renderer
    SDL_RenderPresent(renderer);

    //Ajusta a imagem à tela
    imageScreenAdjustment(&zoom, tilesMatrixDim, screenDim, IMAGE_SIZE);

    //Centraliza a imagem na tela
    pos.x = (backWidth - (zoom * IMAGE_SIZE * tilesMatrixDim.x)) / 2;
    pos.y = (windowHeight - (zoom * IMAGE_SIZE * tilesMatrixDim.y)) / 2;

    SDL_Rect dstRect = {pos.x, pos.y, IMAGE_SIZE, IMAGE_SIZE};

    // Desenha os tiles na tela
    drawImage(dstRect, pos, tilesMatrixDim, zoom, renderer, texture_matrix);

    // Aguarda evento de saída
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN) {
            if(isPressed(genNewMap->boddy, event)) {

                clearDisplay(renderer);
                // Desenha barra de menu
                SDL_SetRenderDrawColor(renderer, 38,38,46, 255);
                SDL_RenderFillRect(renderer, &menuBar);

                // Desenha botões do menu
                SDL_SetRenderDrawColor(renderer, 52,52,59, 255);
                SDL_RenderFillRect(renderer, &(genNewMap->boddy));

                SDL_RenderFillRect(renderer, &(exit->boddy));

                SDL_RenderFillRect(renderer, &(zoomIn->boddy));

                SDL_RenderFillRect(renderer, &(zoomOut->boddy));

                drawImage(dstRect, pos, tilesMatrixDim, zoom,  renderer, texture_matrix);
            }
            if(isPressed(exit->boddy, event)) {
                break;
            }
            if(isPressed(zoomIn->boddy, event) && ((zoom+1) * IMAGE_SIZE * tilesMatrixDim.x) < backWidth && ((zoom+1) * IMAGE_SIZE * tilesMatrixDim.y) < windowHeight) {
                zoom += 1;

                clearDisplay(renderer);

                // Desenha barra de menu
                SDL_SetRenderDrawColor(renderer, 38,38,46, 255);
                SDL_RenderFillRect(renderer, &menuBar);

                // Desenha botões do menu
                SDL_SetRenderDrawColor(renderer, 52,52,59, 255);
                SDL_RenderFillRect(renderer, &(genNewMap->boddy));

                SDL_RenderFillRect(renderer, &(exit->boddy));

                SDL_RenderFillRect(renderer, &(zoomIn->boddy));

                SDL_RenderFillRect(renderer, &(zoomOut->boddy));

                // Desenha os tiles
                pos.x = (backWidth - (zoom * IMAGE_SIZE * tilesMatrixDim.x)) / 2;
                pos.y = (windowHeight - (zoom * IMAGE_SIZE * tilesMatrixDim.y)) / 2;

                dstRect.x = pos.x;
                dstRect.y = pos.y;

                drawImage(dstRect, pos, tilesMatrixDim, zoom, renderer, texture_matrix);
            }
            if(isPressed(zoomOut->boddy, event) && zoom > 1) {
                zoom -= 1;

                clearDisplay(renderer);

                // Desenhar barra de menu
                SDL_SetRenderDrawColor(renderer, 38,38,46, 255);
                SDL_RenderFillRect(renderer, &menuBar);

                // Desenhar botões do menu
                SDL_SetRenderDrawColor(renderer, 52,52,59, 255);
                SDL_RenderFillRect(renderer, &(genNewMap->boddy));

                SDL_RenderFillRect(renderer, &(exit->boddy));

                SDL_RenderFillRect(renderer, &(zoomIn->boddy));

                SDL_RenderFillRect(renderer, &(zoomOut->boddy));

                // Desenha os tiles
                pos.x = (backWidth - (zoom * IMAGE_SIZE * tilesMatrixDim.x)) / 2;
                pos.y = (windowHeight - (zoom * IMAGE_SIZE * tilesMatrixDim.y)) / 2;

                dstRect.x = pos.x;
                dstRect.y = pos.y;

                drawImage(dstRect, pos, tilesMatrixDim, zoom, renderer, texture_matrix);
            }
            
        }
    }

    // LIBERAÇÃO DE MEMÓRIA ALOCADA

    freeSdlContentMatrix(image_matrix, texture_matrix, tilesMatrixDim);

    for(int c=0; c<tilesMatrixDim.y; c++) free(image_matrix[c]);
    free(image_matrix);

    for(int c=0; c<tilesMatrixDim.y; c++) free(texture_matrix[c]);
    free(texture_matrix);

    freeSdlDisplay(window, renderer);
    SDL_Quit();
    return 0;
}