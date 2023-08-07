#ifndef STDIO
#define STDIO
    #include <stdio.h>
#endif

// Estrutura criada para utilização em matrizes. Pode representar
// posições, dimensões, vetores, etc.
typedef struct orderedPair{
    int x;
    int y;
} orderedPair;

int binSrch(int list[], int min, int max, int tg);

int search(int array[], int tg);

void printArray(int array[]);