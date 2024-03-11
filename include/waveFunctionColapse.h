#ifndef STDLIB
#define STDLIB
    #include <stdlib.h>
#endif

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif

#ifndef SDL_GENERAL
#define SDL_GENERAL

    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>

#endif

#ifndef STDIO
#define STDIO
    #include <stdio.h>
#endif

#ifndef UTILS
#define UTILS
    #include "utils.h"
#endif

#include "LinkedList.h"

#define NUM_TILES 10

// Estrutura que representa um tile. Tem como campos uma textura de tile, seu id
// e listas para todos os tiles aceitos para cada lado
typedef struct tile{
    int id;
    Node* t;
    Node* l;
    Node* b;
    Node* r;
    SDL_Texture *tex;
}tile;

// Estrutura que representa uma posição na matriz do algoritmo WFC.
// Ela contém todas as possibilidades de tile para esta posição quando não colapsado
// e o tile escolhido após o colapso.
typedef struct cell{
    Node *possibilities;
    int entropy;
    tile *tl;
}cell;

// Estrutura que armazena uma matriz de células e suas dimensões
typedef struct cellGrid{
    orderedPair dim;
    cell ***cellMatrix;
    tile **tileList;
}cellGrid;

// Construtor para a struct tile
tile *createTile(int id, Node *t, Node *l, Node *b, Node *r, char *imgPath, SDL_Renderer *renderer);

// Libera a memória alocada para uma struct tile
void freeTile(tile *t);

// Construtor para a struct cell
cell *createCell();

// Libera memória alocada para uma struct cell
void freeCell(cell *n);

// Preenche um grid com células
int fillCellGrid(cellGrid* grid);

// Construtor do grid de células
cellGrid *createCellGrid(tile **tileList, orderedPair dim);

// Desaloca todos as células de um grid
void cleanCellGrid(cellGrid *grid);

// Libera a memória que foi alocada para uma struct cellGrid
void freeCellGrid(cellGrid *grid);

// Quando o algoritmo adiciona um novo tile à matriz, a função atualiza 
// os tiles possíveis (entropia) nos cells ao redor
void updateEntropy(cellGrid *grid, orderedPair pos);

// Encontra a menor entropia em uma matriz e retorna a posição de um dos
// cells com esta entropia
orderedPair findLowestEntropy(cellGrid *grid);

// Colapsa o cell para a entropia mínima e atribui um tile possível para o cell
int collapseCell(cellGrid *grid, orderedPair pos);

// Procura um tile pelo Id em uma matriz
tile* findTileById(tile **matrixTile, int id);

// Um ciclo do algoritmo WFC
orderedPair WFC_Cycle(cellGrid *grid, orderedPair displayDim, int zoom);