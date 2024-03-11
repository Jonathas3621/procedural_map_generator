#include "../include/waveFunctionColapse.h"

tile *createTile(int id, Node *t, Node *l, Node *b, Node *r, char *imgPath, SDL_Renderer *renderer) {
    
    SDL_Surface *surface = IMG_Load(imgPath);
    if(!surface) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a imagem do tile %d: %s", id, SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!texture) {

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a textura do tile %d: %s", id, SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_FreeSurface(surface);

    tile *temp = malloc(sizeof(tile));
    if(!temp) {
        fprintf(stderr, "Erro ao criar tile %d", id);
        return NULL;
    }

    temp->id = id;
    temp->t = t;
    temp->l = l;
    temp->b = b;
    temp->r = r;
    temp->tex = texture;

    return temp;
}

void freeTile(tile *t) {
    if(!t) return;
    SDL_DestroyTexture(t->tex);
    clearList(&(t->t));
    clearList(&(t->b));
    clearList(&(t->r));
    clearList(&(t->l));
    free(t);
}

cell *createCell() {

    cell *n = malloc(sizeof(cell));
    if(!n) return NULL;

    n->possibilities = createNewNode(1);
    insertAtHead(&(n->possibilities), createNewNode(38));
    insertAtHead(&(n->possibilities), createNewNode(39));
    insertAtHead(&(n->possibilities), createNewNode(40));
    insertAtHead(&(n->possibilities), createNewNode(47));
    insertAtHead(&(n->possibilities), createNewNode(48));
    insertAtHead(&(n->possibilities), createNewNode(49));
    insertAtHead(&(n->possibilities), createNewNode(56));
    insertAtHead(&(n->possibilities), createNewNode(57));
    insertAtHead(&(n->possibilities), createNewNode(58));
    insertAtHead(&(n->possibilities), createNewNode(65));

    n->entropy = 11;
    n->tl = NULL;

    return n;
}

void freeCell(cell *n) {
    if(!n) return;
    //freeTile(n->tl);
    clearList(&(n->possibilities));
    free(n);
}

int fillCellGrid(cellGrid* grid) {

    if(!grid) return -1;

    orderedPair dim = grid->dim;

    // Cria células e as adiciona no grid
    for(int c = 0; c < dim.x; c++) {
        for(int i = 0; i < dim.y; i++) {

            grid->cellMatrix[c][i] = createCell();

            if(!grid->cellMatrix[c][i]) return 1;
        }
    }

    return 0;
}

cellGrid *createCellGrid(tile **tileList, orderedPair dim) {

    if(!tileList) return NULL;

    int c, i;
    // Aloca memória para um grid de células
    cellGrid *grid = malloc(sizeof(cellGrid));
    if(!grid) return NULL;

    // Aloca memória para uma matriz de células
    cell*** cellMatrix = malloc(dim.x*sizeof(cell*));
    if(!cellMatrix) return NULL;

    for(c = 0; c < dim.x; c++) {
        cellMatrix[c] = malloc(dim.y*sizeof(cell*));

        if(!cellMatrix) {
            for(i = 0; i < c; i++) free(cellMatrix[i]);
            free(cellMatrix);
            return NULL;
        }
    }

    // Preenche os campos do grid de células
    grid->cellMatrix = cellMatrix;
    grid->tileList = tileList;
    grid->dim = dim;

    // Para fins de segurança, os elementos da matriz são inicializados com NULL
    for(c = 0; c < dim.x; c++) {
        for(i = 0; i < dim.y; i++) {
            cellMatrix[c][i] = NULL;
        }
    }

    // Cria células e as adiciona no grid
    int isFilled = fillCellGrid(grid);
    if(isFilled == 1) {
        freeCellGrid(grid);
        grid = NULL;
    }

    return grid;
}

void cleanCellGrid(cellGrid *grid) {

    if(!grid) return;

    cell ***cellMatrix = grid->cellMatrix;
    orderedPair dim = grid->dim;

    for(int c = 0; c < dim.x; c++) {
        for(int i = 0; i < dim.y; i++) {
            freeCell(cellMatrix[c][i]);
            cellMatrix[c][i] = NULL;
        }
    }
}

void freeCellGrid(cellGrid *grid) {

    if(!grid) return;

    cleanCellGrid(grid);
    orderedPair dim = grid->dim;
    cell ***cellMatrix = grid->cellMatrix;

    for(int c = 0; c < dim.x; c++) {
        if(cellMatrix) free(cellMatrix[c]);
    }
    free(cellMatrix);
}

void updateEntropy(cellGrid *grid, orderedPair pos) {
    int c;
    int w = grid->dim.y, h = grid->dim.x;
    int x = pos.x, y = pos.y;
    cell ***cellMatrix = grid->cellMatrix;

    // Confere cell de cima
    if(x > 0 && !cellMatrix[x-1][y]->tl) {
        Node *cl = (cellMatrix[x-1][y])->possibilities;
        Node *temp = NULL;
        
        while(cl != NULL) {
            if(!searchNode((cellMatrix[x][y])->tl->t, cl->value)) {
                temp = cl;
                cl = cl->next;
                delNode(&((cellMatrix[x-1][y])->possibilities), temp);
                (cellMatrix[x-1][y])->entropy --;
            }
            else {
                cl = cl->next;
            }
        }
    }

    // Confere cell da esquerda
    if(y > 0 && !cellMatrix[x][y-1]->tl) {
        Node *cl = (cellMatrix[x][y-1])->possibilities;
        Node *temp = NULL;

        while(cl != NULL) {
            if(!searchNode((cellMatrix[x][y])->tl->l, cl->value)) {
                temp = cl;
                cl = cl->next;
                delNode(&((cellMatrix[x][y-1])->possibilities), temp);
                (cellMatrix[x][y-1])->entropy --;
            }
            else {
                cl = cl->next;
            }
        }
    }

    // Confere cell de baixo
    if(x<(h-1) && !cellMatrix[x+1][y]->tl) {
        Node *cl = (cellMatrix[x+1][y])->possibilities;
        Node *temp = NULL;

        while(cl != NULL) {
            if(!searchNode((cellMatrix[x][y])->tl->b, cl->value)) {
                temp = cl;
                cl = cl->next;
                delNode(&((cellMatrix[x+1][y])->possibilities), temp);
                (cellMatrix[x+1][y])->entropy --;
            }
            else {
                cl = cl->next;
            }
        }
    }

    // Confere cell da direita
    if(y<(w-1) && !cellMatrix[x][y+1]->tl) {
        Node *cl = (cellMatrix[x][y+1])->possibilities;
        Node *temp = NULL;

        while(cl != NULL) {
            if(!searchNode((cellMatrix[x][y])->tl->r, cl->value)) {
                temp = cl;
                cl = cl->next;
                delNode(&((cellMatrix[x][y+1])->possibilities), temp);
                (cellMatrix[x][y+1])->entropy --;
            }
            else {
                cl = cl->next;
            }
        }
    }
}

orderedPair findLowestEntropy(cellGrid *grid) {

    // Retorno para caso de erro
    orderedPair error = {-1,-1};

    // A função termina em caso de argumentos inválidos
    if(!grid) return error;

    // Variáveis de grid
    cell ***cellMatrix = grid->cellMatrix;
    orderedPair dim = grid->dim;
    int c, i, flag = 0;

    // Variáveis gerais
    int minEntropy = 11; //getSize(grid->tileList)
    orderedPair posi = error;

    // Algoritmo de procura por menor valor em matriz
    for(c=0; c<dim.x; c++) {
        for(i=0; i<dim.y; i++) {
            if((cellMatrix[c][i])->tl) continue;
            if(minEntropy >= (cellMatrix[c][i])->entropy) {
                minEntropy = (cellMatrix[c][i])->entropy;
                posi.x = c;
                posi.y = i;
            }
            flag = 1;
        }
    }

    return posi;
}

tile* findTileById(tile **matrixTile, int id) {
    for(int i = 0; i < 11; i++) {
        if(matrixTile[i]->id == id) return matrixTile[i];
    }
    return NULL;
}

int collapseCell(cellGrid *grid,  orderedPair pos) {

    //Encerra a função com código de erro em caso de argumentos inválidos
    if(!grid) return 1;

    tile **tileList = grid->tileList;
    cell *n = grid->cellMatrix[pos.x][pos.y];
    tile *temp = NULL;

    //Encerra a função com código de erro em caso de célula com entropia inválida
    if(n->entropy <= 0) return 1;

    //Pega um id aleatório dentre as possibilidades
    Node *finalTileNode = getRandNode(n->possibilities, n->entropy);

    //Encontra o tile correspondente ao id obtido
    temp = findTileById(tileList, finalTileNode->value);

    //Encerra a função com código de erro caso não haja tile com o id obtido
    if(!temp) return 1;

    //Colapsa a célula e atribui a ela o tile correspondete ao id encontrado
    n->entropy = 0;
    n->tl = temp;

    return 0;
}

orderedPair WFC_Cycle(cellGrid *grid, orderedPair displayDim, int zoom) {

    // Retorno para caso de erro
    orderedPair error = {-1,-1};

    // Variáveis de Grid
    cell ***cellMatrix = grid->cellMatrix;
    tile **tileList = grid->tileList;
    orderedPair dim = grid->dim;

    // Seleção de célula a ser colapsada
    orderedPair lowEntPosition = findLowestEntropy(grid);
    if(lowEntPosition.x == -1) return error;

    cell* cel = cellMatrix[lowEntPosition.x][lowEntPosition.y];

    printf("Encontrou menor entropia x: %d, y: %d\n", lowEntPosition.x, lowEntPosition.y);

    // Colapso da célula selecionada
    if(collapseCell(grid, lowEntPosition)) return error;

    // Propagação da informação
    Node* til = cel->possibilities;
    updateEntropy(grid, lowEntPosition);

    return lowEntPosition;
}