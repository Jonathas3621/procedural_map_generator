#include "headers/GUI.h"
#include "headers/waveFunctionColapse.h"

int main(int argc, char** argv) {

    //Tamanho de tela padrão
    orderedPair windowDim = {600, 1000};

    //Dimensões de estruturas da tela
    orderedPair menuDim = {windowDim.x, windowDim.y / 4};
    orderedPair displayDim = {windowDim.x, windowDim.y - menuDim.y};
    int btnHeight = 40;
    int spacing = 40;

    // variáveis gerais (Valores padrão)
    orderedPair pos = {0, 0};
    int zoom = 3;

    //INICIALIZAÇÃO DAS ESTRUTURAS

    //Inicializando o SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL: %s", SDL_GetError());
        return -1;
    }

    //Criando tela
    SDL_Window* window = SDL_CreateWindow("Gerador Procedural",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowDim.y,
        windowDim.x,
        SDL_WINDOW_SHOWN);
    if(!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar a janela: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    //Criando Renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erro ao criar o renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //CRIAÇÃO DE RENDERIZÁVEIS
    SDL_Rect menuBar = {windowDim.y - menuDim.y, 0, menuDim.y, menuDim.x};

    btnHeight = windowDim.y /20;

    //Botão para ativar nova geração
    char label1[] = "Novo Mapa"; 
    button *genNewMap = createButton(menuBar.x + spacing, menuBar.y + spacing, menuBar.w - 2*spacing, btnHeight, label1, NULL);

    //Botão de saída
    char label2[] = "Sair";
    button *exit = createButton(menuBar.x + spacing, menuBar.h - (spacing + btnHeight), menuBar.w - 2*spacing, btnHeight, label2, NULL);

    //Botão zoom in
    char label3[] = "+";
    button *zoomIn = createButton(menuBar.x + spacing, menuBar.h - 2*(spacing + btnHeight), (menuBar.w - 3*spacing)/2, btnHeight, label3, NULL);

    //Botão zoom out
    char label4[] = "-";
    button *zoomOut = createButton(windowDim.y - (spacing + (menuBar.w - 3*spacing)/2), menuBar.h - 2*(spacing + btnHeight), (menuBar.w - 3*spacing)/2, btnHeight, label4, NULL);

    // DECLARAÇÃO DOS TILES

    // Restrições da face superior
    Node *leafAndBottom = createNewNode(1);
    insertAtHead(&leafAndBottom, createNewNode(56));
    insertAtHead(&leafAndBottom, createNewNode(57));
    insertAtHead(&leafAndBottom, createNewNode(58));
    Node *leafAndBottom1 = createNewNode(1);
    insertAtHead(&leafAndBottom1, createNewNode(56));
    insertAtHead(&leafAndBottom1, createNewNode(57));
    insertAtHead(&leafAndBottom1, createNewNode(58));
    Node *leafAndBottom2 = createNewNode(1);
    insertAtHead(&leafAndBottom2, createNewNode(56));
    insertAtHead(&leafAndBottom2, createNewNode(57));
    insertAtHead(&leafAndBottom2, createNewNode(58));
    Node *leafAndBottom3 = createNewNode(1);
    insertAtHead(&leafAndBottom3, createNewNode(56));
    insertAtHead(&leafAndBottom3, createNewNode(57));
    insertAtHead(&leafAndBottom3, createNewNode(58));

    Node *bottomCornerLeft = createNewNode(47);
    insertAtHead(&bottomCornerLeft, createNewNode(38));
    Node *bottomCornerLeft1 = createNewNode(47);
    insertAtHead(&bottomCornerLeft1, createNewNode(38));

    Node *bottomMid = createNewNode(39);
    insertAtHead(&bottomMid, createNewNode(48));
    Node *bottomMid1 = createNewNode(39);
    insertAtHead(&bottomMid1, createNewNode(48));

    Node *bottomMid2 = createNewNode(39);
    insertAtHead(&bottomMid2, createNewNode(48));

    Node *bottomCornerRight = createNewNode(40);
    insertAtHead(&bottomCornerRight, createNewNode(49));
    insertAtHead(&bottomCornerRight, createNewNode(65));
    Node *bottomCornerRight1 = createNewNode(40);
    insertAtHead(&bottomCornerRight1, createNewNode(49));
    insertAtHead(&bottomCornerRight1, createNewNode(65));

    // Restrições da face esquerda
    Node *leafAndRight = createNewNode(1);
    insertAtHead(&leafAndRight, createNewNode(40));
    insertAtHead(&leafAndRight, createNewNode(49));
    insertAtHead(&leafAndRight, createNewNode(58));
    Node *leafAndRight1 = createNewNode(1);
    insertAtHead(&leafAndRight1, createNewNode(40));
    insertAtHead(&leafAndRight1, createNewNode(49));
    insertAtHead(&leafAndRight1, createNewNode(58));
    Node *leafAndRight2 = createNewNode(1);
    insertAtHead(&leafAndRight2, createNewNode(40));
    insertAtHead(&leafAndRight2, createNewNode(49));
    insertAtHead(&leafAndRight2, createNewNode(58));
    Node *leafAndRight3 = createNewNode(1);
    insertAtHead(&leafAndRight3, createNewNode(40));
    insertAtHead(&leafAndRight3, createNewNode(49));
    insertAtHead(&leafAndRight3, createNewNode(58));

    Node *rightCornerTop = createNewNode(38);
    insertAtHead(&rightCornerTop, createNewNode(39));
    Node *rightCornerTop1 = createNewNode(38);
    insertAtHead(&rightCornerTop1, createNewNode(39));

    Node *rightMid = createNewNode(47);
    insertAtHead(&rightMid, createNewNode(48));
    Node *rightMid1 = createNewNode(47);
    insertAtHead(&rightMid1, createNewNode(48));

    Node *rightMid2 = createNewNode(47);
    insertAtHead(&rightMid2, createNewNode(48));

    Node *rightCornerBottom = createNewNode(56);
    insertAtHead(&rightCornerBottom, createNewNode(57));
    insertAtHead(&rightCornerBottom, createNewNode(65));
    Node *rightCornerBottom1 = createNewNode(56);
    insertAtHead(&rightCornerBottom1, createNewNode(57));
    insertAtHead(&rightCornerBottom1, createNewNode(65));

    // Restrições da face inferior
    Node *leafAndTop = createNewNode(1);
    insertAtHead(&leafAndTop, createNewNode(38));
    insertAtHead(&leafAndTop, createNewNode(39));
    insertAtHead(&leafAndTop, createNewNode(40));
    Node *leafAndTop1 = createNewNode(1);
    insertAtHead(&leafAndTop1, createNewNode(38));
    insertAtHead(&leafAndTop1, createNewNode(39));
    insertAtHead(&leafAndTop1, createNewNode(40));
    Node *leafAndTop2 = createNewNode(1);
    insertAtHead(&leafAndTop2, createNewNode(38));
    insertAtHead(&leafAndTop2, createNewNode(39));
    insertAtHead(&leafAndTop2, createNewNode(40));
    Node *leafAndTop3 = createNewNode(1);
    insertAtHead(&leafAndTop3, createNewNode(38));
    insertAtHead(&leafAndTop3, createNewNode(39));
    insertAtHead(&leafAndTop3, createNewNode(40));

    Node *topCornerLeft = createNewNode(47);
    insertAtHead(&topCornerLeft, createNewNode(56));
    Node *topCornerLeft1 = createNewNode(47);
    insertAtHead(&topCornerLeft1, createNewNode(56));

    Node *topMid = createNewNode(48);
    insertAtHead(&topMid , createNewNode(57));
    insertAtHead(&topMid , createNewNode(65));
    Node *topMid1 = createNewNode(48);
    insertAtHead(&topMid1 , createNewNode(57));
    insertAtHead(&topMid1 , createNewNode(65));

    Node *topCornerRight = createNewNode(49);
    insertAtHead(&topCornerRight, createNewNode(58));
    Node *topCornerRight1 = createNewNode(49);
    insertAtHead(&topCornerRight1, createNewNode(58));

    Node *topCornerRight2 = createNewNode(49);
    insertAtHead(&topCornerRight2, createNewNode(58));
 
    // Restrições da face direita
    Node *leafAndLeft= createNewNode(1);
    insertAtHead(&leafAndLeft, createNewNode(38));
    insertAtHead(&leafAndLeft, createNewNode(47));
    insertAtHead(&leafAndLeft, createNewNode(56));
    Node *leafAndLeft1 = createNewNode(1);
    insertAtHead(&leafAndLeft1, createNewNode(38));
    insertAtHead(&leafAndLeft1, createNewNode(47));
    insertAtHead(&leafAndLeft1, createNewNode(56));
    Node *leafAndLeft2 = createNewNode(1);
    insertAtHead(&leafAndLeft2, createNewNode(38));
    insertAtHead(&leafAndLeft2, createNewNode(47));
    insertAtHead(&leafAndLeft2, createNewNode(56));
    Node *leafAndLeft3 = createNewNode(1);
    insertAtHead(&leafAndLeft3, createNewNode(38));
    insertAtHead(&leafAndLeft3, createNewNode(47));
    insertAtHead(&leafAndLeft3, createNewNode(56));

    Node *leftCornerTop = createNewNode(39);
    insertAtHead(&leftCornerTop, createNewNode(40));
    Node *leftCornerTop1 = createNewNode(39);
    insertAtHead(&leftCornerTop1, createNewNode(40));

    Node *leftMid = createNewNode(48);
    insertAtHead(&leftMid, createNewNode(49));
    insertAtHead(&leftMid, createNewNode(65));
    Node *leftMid1 = createNewNode(48);
    insertAtHead(&leftMid1, createNewNode(49));
    insertAtHead(&leftMid1, createNewNode(65));

    Node *leftCornerBottom = createNewNode(57);
    insertAtHead(&leftCornerBottom, createNewNode(58));
    Node *leftCornerBottom1 = createNewNode(57);
    insertAtHead(&leftCornerBottom1, createNewNode(58));

    Node *leftCornerBottom2 = createNewNode(57);
    insertAtHead(&leftCornerBottom2, createNewNode(58));

    // Paths para imagens
    char tile01[] = "tiles/1.png";
    char tile38[] = "tiles/38.png";
    char tile39[] = "tiles/39.png";
    char tile40[] = "tiles/40.png";
    char tile47[] = "tiles/47.png";
    char tile48[] = "tiles/48.png";
    char tile49[] = "tiles/49.png";
    char tile56[] = "tiles/56.png";
    char tile57[] = "tiles/57.png";
    char tile58[] = "tiles/58.png";
    char tile65[] = "tiles/65.png";

    /*
    38 39 40
    47 48 49
    56 57 58
    */

    // Array de tiles
    tile **matrixTile = malloc(11*sizeof(tile));

    // Criação dos tiles a serem utilizados
    matrixTile[0] = createTile(1, leafAndBottom, leafAndRight, leafAndTop, leafAndLeft, tile01, renderer);
    matrixTile[1] = createTile(38, leafAndBottom1, leafAndRight1, topCornerLeft, leftCornerTop, tile38, renderer);
    matrixTile[2] = createTile(39, leafAndBottom2, rightCornerTop, topMid, leftCornerTop1, tile39, renderer);
    matrixTile[3] = createTile(40, leafAndBottom3, rightCornerTop1, topCornerRight, leafAndLeft1, tile40, renderer);
    matrixTile[4] = createTile(47, bottomCornerLeft, leafAndRight2, topCornerLeft1, leftMid, tile47, renderer);
    matrixTile[5] = createTile(48, bottomMid, rightMid, topMid1, leftMid1, tile48, renderer);
    matrixTile[6] = createTile(49, bottomCornerRight, rightMid1, topCornerRight1, leafAndLeft2, tile49, renderer);
    matrixTile[7] = createTile(56, bottomCornerLeft1, leafAndRight3, leafAndTop1, leftCornerBottom, tile56, renderer);
    matrixTile[8] = createTile(57, bottomMid1, rightCornerBottom, leafAndTop2, leftCornerBottom1, tile57, renderer);
    matrixTile[9] = createTile(58, bottomCornerRight1, rightCornerBottom1, leafAndTop3, leafAndLeft3, tile58, renderer);
    matrixTile[10] = createTile(65, bottomMid2, rightMid2, topCornerRight2, leftCornerBottom2, tile65, renderer);

    // WFC configuração
    orderedPair dim = {10, 10};
    cellGrid *grid = createCellGrid(matrixTile, dim);

    //RENDERIZAÇÃO

    // Limpa a tela
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

    //Atualiza o renderer
    SDL_RenderPresent(renderer);
    
    // Variáveis de display
    orderedPair blockDim = {14*grid->dim.x*zoom, 14*grid->dim.y*zoom};
    orderedPair firstPosition = centeringBlock(blockDim, displayDim);

    //WFC em ação
    while(1) {

        orderedPair collapsedCellPos = WFC_Cycle(grid, displayDim, zoom);
        if(collapsedCellPos.x == -1) break;

        cell *collapsedCell = grid->cellMatrix[collapsedCellPos.x][collapsedCellPos.y];
        printTexture(collapsedCell->tl->tex, renderer, firstPosition, zoom, collapsedCellPos);
    }
/*    orderedPair* lowestEntropyPosition;
    orderedPair back = {windowHeight, backWidth};
    orderedPair firstPosition = takeFirstPosition(back, zoom);
    while(1) {
        lowestEntropyPosition = findLowestEntropy(matrixCell, 10, 10);
        if(!lowestEntropyPosition) break;
        printf("Encontrou menor entropia x: %d, y: %d\n", lowestEntropyPosition->x, lowestEntropyPosition->y);

        //printf("[%d][%d] -", lowestEntropyPosition->x, lowestEntropyPosition->y);
        cell* cel = matrixCell[lowestEntropyPosition->x][lowestEntropyPosition->y];
        Node* til = cel->possibilities;
        //printList(til);
        //printf("Entropia: %d", cel->entropy);
        //printf("]\n");

        if(collapseCell(matrixCell, matrixTile, *lowestEntropyPosition)) break;
        updateEntropy(matrixCell, 10, 10, lowestEntropyPosition->x, lowestEntropyPosition->y);

        printTexture((matrixCell[lowestEntropyPosition->x][lowestEntropyPosition->y])->tl->tex,
                    renderer, firstPosition, zoom, *lowestEntropyPosition);
        SDL_Delay(5);
    }

    free(lowestEntropyPosition);
*/
    //Ajusta a imagem à tela
    //imageScreenAdjustment(&zoom, tilesMatrixDim, screenDim, IMAGE_SIZE);

    //Centraliza a imagem na tela
    //pos.x = (backWidth - (zoom * IMAGE_SIZE * tilesMatrixDim.x)) / 2;
    //pos.y = (windowHeight - (zoom * IMAGE_SIZE * tilesMatrixDim.y)) / 2;

    //SDL_Rect dstRect = {pos.x, pos.y, IMAGE_SIZE, IMAGE_SIZE};

    // Desenha os tiles na tela
    //drawImage(dstRect, pos, tilesMatrixDim, zoom, renderer, texture_matrix);

    // Loop principal
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
        if(event.type == SDL_MOUSEBUTTONDOWN) {/*
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

            }*/
            if(isPressed(exit->boddy, event)) {
                break;
            }/*
            if(isPressed(zoomIn->boddy, event) && ((zoom+1) * IMAGE_SIZE * 10) < backWidth && ((zoom+1) * IMAGE_SIZE * 10) < windowHeight) {
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
                pos.x = (backWidth - (zoom * IMAGE_SIZE * 10)) / 2;
                pos.y = (windowHeight - (zoom * IMAGE_SIZE * 10)) / 2;

                //dstRect.x = pos.x;
               // dstRect.y = pos.y;

                //drawImage(dstRect, pos, tilesMatrixDim, zoom, renderer, texture_matrix);
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
                pos.x = (backWidth - (zoom * IMAGE_SIZE * 10)) / 2;
                pos.y = (windowHeight - (zoom * IMAGE_SIZE * 10)) / 2;

                //dstRect.x = pos.x;
                //dstRect.y = pos.y;

                //drawImage(dstRect, pos, tilesMatrixDim, zoom, renderer, texture_matrix);
            }
            */
        }
    }

    //LIBERAÇÃO DE MEMÓRIA ALOCADA

    //WFC
    freeCellGrid(grid);

    for(int c = 0; c < 10; c++) {
        freeTile(matrixTile[c]);
    }
    free(matrixTile);

    printf("Fim de programa!");
    return 0;
}