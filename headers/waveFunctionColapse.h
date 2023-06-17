#include <stdlib.h>

#include <SDL.h>
#include <SDL_image.h>

// Representa posições em matrizes
typedef struct pos{
    int l;
    int c;
}pos;

// Estrutura que representa um tile. Tem como campos uma textura de tile, seu id
// e listas para todos os tiles aceitos para cada lado
typedef struct tile{
    int id;
    int* t;
    int* l;
    int* b;
    int* r;
    SDL_Texture *tex;
}tile;

// Estrutura que representa uma posição na matriz do algoritmo WFC.
// Ela contém todas as possibilidades de tile para esta posição quando não colapsado
// e o tile escolhido após o colapso.
typedef struct node{
    int possibilities[80];
    int entropy;
    tile *t;
}node;

// Construtor para a struct tile
tile *createTile(int id, int *t, int *l, int *b, int *r, char *imgPath, SDL_Renderer renderer);

// Libera a memória alocada para uma struct tile
void freeTile(tile *t);

// Construtor para a struct node
node *createNode();

// Libera memória alocada para uma struct node
void freeNode(node *n);

// Quando o algoritmo adiciona um novo tile à matriz, a função atualiza 
// os tiles possíveis (entropia) nos nodes ao redor
void updateEntropy(node ***nodeMatrix, int w, int h, int x, int y);

// Encontra a menor entropia em uma matriz e retorna a posição de um dos
// nodes com esta entropia
pos findLowestEntropy(node ***nodeMatrix, int w, int h);

// Colapsa o node para a entropia mínima e atribui um tile possível para o node
void collapseNode(node ***nodeMatrix, pos pos);