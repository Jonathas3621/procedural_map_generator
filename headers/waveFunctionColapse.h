#include <SDL.h>
#include <SDL_image.h>

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

// Construtor para a struct tile
tile *createTile(int id, int *t, int *l, int *b, int *r, char *imgPath, SDL_Renderer renderer);

// Libera a memória alocada para uma struct tile
void freeTile(tile *t);