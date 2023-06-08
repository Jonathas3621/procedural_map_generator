#include "headers/waveFunctionColapse.h"

tile *createTile(int id, int *t, int *l, int *b, int *r, char *imgPath, SDL_Renderer renderer) {
    
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

    SDL_DestroyTexture(t->tex);
    free(t);
}