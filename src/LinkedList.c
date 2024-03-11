#include "../include/LinkedList.h"

Node *createNewNode(int value){
    
    Node *temp = malloc(sizeof(Node));
    temp->value = value;
    temp->next = NULL;
    
    return temp;
}

void freeNode(Node *node) {
    free(node);
}

void insertAtHead(Node **head, Node *nodeToInsert){
    
    nodeToInsert->next = *head;
    *head = nodeToInsert;
}

Node *getNodeByIndex(Node *head, int index) {
    Node *temp = head;

    for(int c=0; temp; c++) {
        if(c == index) return temp;
        temp = temp->next;
    }

    return NULL;
}

Node *getRandNode(Node* head, int dim) {

    int index = rand() % dim;
    Node *node = getNodeByIndex(head, index);

    return node;
}

void delNode(Node **head, Node *nodeToDelete) {

    Node *temp = *head;

    //Encerra a função em caso de argumentos inválidos
    if(!temp || !nodeToDelete) return;

    //Algoritmo para quando se quer deletar o primeiro nó
    if(*head == nodeToDelete) {
        *head = (*head)->next;
        free(nodeToDelete);
        return; 
    }

    //Encontra o elemento anterior ao nó que se quer deletar
    while(temp->next != NULL && temp->next != nodeToDelete) {
        temp = temp->next;
    }

    //Se o nó a ser deletado não estiver na lista a função termina
    if(!(temp->next)) return;

    //Caso contrário, o elemento é apagado e a lista ajustada
    temp->next = temp->next->next;
    free(nodeToDelete);
}

Node* searchNode(Node* lista, int value) {

    //Se não houver elementos na lista, a função termina
    if(!lista) return NULL;

    Node* aux = lista;

    //Percorre a lista até encontrar o elemento desejado
    while(aux != NULL && aux->value != value){
        aux = aux->next;
    }
    return aux;
}

void printList(Node* list) {

    Node* no = list;

    while(no != NULL){
        printf("| v: %d", no->value);
        //printf("| v: %d en: %p", no->value, no);
        no = no->next;
    }
}

void clearList(Node **list) {
    while(*list) delNode(list, *list);
}

int getSize(Node *list) {

    if(!list) return 0;

    Node* aux = list;
    int tam = 0;

    while(aux) {
        aux = aux->next;
        tam++;
    }

    return tam;
}