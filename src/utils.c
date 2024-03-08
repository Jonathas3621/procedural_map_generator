#include "headers/utils.h"

int binSrch(int list[], int min, int max, int tg) {
    
    int dim;
    int mid;
    
    dim = (max + 1) - min;
    mid = min + dim /2;

    if(list[mid] == tg) return mid;
    else if(list[mid] > tg) max = mid - 1;
    else min = mid;
    
    if(min >= max) return -1;
    
    return binSrch(list, min, max, tg);
}

int search(int array[], int tg) {
    
    int c;
    for(c=0; array[c] || array[c] == tg; c++);
    if(array[c] == tg) return 0;
    return 1;
}

void printArray(int array[]) {
    for(int c = 0; array[c]; c++) printf("%d <-> ", array[c]);
}