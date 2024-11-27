#ifndef STDLIB
#define STDLIB
    #include <stdlib.h>
#endif

#ifndef STDIO
#define STDIO
    #include <stdio.h>
#endif

#ifndef LINKED_LIST
#define LINKED_LIST
    #include "LinkedList.h"
#endif

#include <jansson.h>
#include <dirent.h>

// Conta a quantidade de arquivos dentro de uma pasta
int countFiles(char *dir);

// Cria uma LinkedList a partir de um JSON array
Node *jsonArrayToLinkedList(json_t *json);

void getKeys(json_t *json);