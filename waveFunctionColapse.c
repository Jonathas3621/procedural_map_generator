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

node *createNode() {

    node *n = malloc(sizeof(node));
    for(int c=0; c < 80; c++) {
        (n->possibilities)[c] = 1;
    }

    n->entropy = 80;
    n->t = NULL;
}

void freeNode(node *n) {

    free(n);
}

void updateEntropy(node ***nodeMatrix, int w, int h, int x, int y) {
    int c;

    // Confere node de cima
    if(y > 0 && !nodeMatrix[y-1][x]->tile) {
        for(c=0; c< nodeMatrix[y-1][x]->entropy; c++) {
            ((nodeMatrix[y-1][x])->possibilities)[c] &= ((nodeMatrix[y][x])->t->t)[c];
        }
    }

    // Confere node da esquerda
    if(x > 0 && !nodeMatrix[y][x-1]->t) {
        for(c=0; c< nodeMatrix[y][x-1]->entropy; c++) {
            ((nodeMatrix[y][x-1])->possibilities)[c] &= ((nodeMatrix[y][x])->t->l)[c];
        }
    }

    // Confere node de baixo
    if(y<h && !nodeMatrix[y+1][x]->tile) {
        for(c=0; c< nodeMatrix[y+1][x]->entropy; c++) {
            ((nodeMatrix[y+1][x])->possibilities)[c] &= ((nodeMatrix[y][x])->t->b)[c];
        }
    }

    // Confere node da direita
    if(x<w && !nodeMatrix[y][x+1]->tile) {
        for(c=0; c< nodeMatrix[y][x+1]->entropy; c++) {
            ((nodeMatrix[y][x+1])->possibilities)[c] &= ((nodeMatrix[y][x])->t->r)[c];
        }
    }
}

pos findLowestEntropy(node ***nodeMatrix, int w, int h) {
    int c;
    int minEntropy = 80;
    pos posi = {0,0};

    for(c=0; c<w; c++) {
        for(i=0; i<h, i++) {
            if((nodeMatrix[c][i])->tile) continue;
            if(minEntropy >= (nodeMatrix[c][i])->entropy) {
                minEntropy = nodeMatrix[c][i])->entropy;
                pos.l = c;
                pos.c = i;
            }
        }
    }

    return posi;
}

void collapseNode(node ***nodeMatrix, pos pos) {

    node *n = nodeMatrix[pos.l][pos.c];

    int id = (n->possibilities)[rand() % 80];

    n->entropy = 1;
    n->t = NULL; // Tile com id 1
}