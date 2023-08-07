#ifndef STDLIB
#define STDLIB
    #include <stdlib.h>
#endif

#ifndef SDL_GENERAL
#define SDL_GENERAL

    #include <SDL.h>
    #include <SDL_image.h>

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

// Construtor para a struct tile
tile *createTile(int id, Node *t, Node *l, Node *b, Node *r, char *imgPath, SDL_Renderer *renderer);

// Libera a memória alocada para uma struct tile
void freeTile(tile *t);

// Construtor para a struct cell
cell *createCell();

// Libera memória alocada para uma struct cell
void freeCell(cell *n);

// Quando o algoritmo adiciona um novo tile à matriz, a função atualiza 
// os tiles possíveis (entropia) nos cells ao redor
void updateEntropy(cell ***cellMatrix, int w, int h, int x, int y);

// Encontra a menor entropia em uma matriz e retorna a posição de um dos
// cells com esta entropia
orderedPair* findLowestEntropy(cell ***cellMatrix, int w, int h);

// Colapsa o cell para a entropia mínima e atribui um tile possível para o cell
int collapseCell(cell ***cellMatrix, tile **matrixTile, orderedPair pos);