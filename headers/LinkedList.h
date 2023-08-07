#ifndef STDLIB
    #define STDLIB
        #include <stdlib.h>
#endif

#ifndef STDIO
#define STDIO
    #include <stdio.h>
#endif

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *createNewNode(int value);

void freeNode(Node *node);

Node *getNodeByIndex(Node *head, int index);

Node *getRandNode(Node* head, int dim);

void insertAtHead(Node **head, Node *nodeToInsert);

void delNode(Node **head, Node *nodeToDelete);

Node* searchNode(Node* lista, int value);

void printList(Node* list);

void clearList(Node **list);