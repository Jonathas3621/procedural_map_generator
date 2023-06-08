#include "headers/GUI.h"

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
    orderedPair tilesMatrixDim = {65, 50}; //(columns(x)(w), lines(y)(h)) 
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