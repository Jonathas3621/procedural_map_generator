#include "../include/jsonHandler.h"

int countFiles(char *dirPath) {
    DIR *dir;
    struct dirent *ent;
    int file_count = 0;

    // Abre o diretório
    dir = opendir(dirPath);

    // Verifica se o diretório foi aberto com sucesso
    if (dir != NULL) {
        // Percorre todos os arquivos no diretório
        while ((ent = readdir(dir)) != NULL) {
            // Ignora os diretórios . e ..
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {
                file_count++; // Incrementa o contador de arquivos
            }
        }
        closedir(dir); // Fecha o diretório
    } else {
        // Se houver um erro ao abrir o diretório
        perror(""); // Imprime a mensagem de erro
        return -1;
    }

    return file_count;
}

Node *jsonArrayToLinkedList(json_t *json) {

    if(!json) return NULL;

    size_t index;
    json_t *value;

    Node *list;

    json_array_foreach(json, index, value) {
        int int_v = (int) json_number_value(value);

        if(!index) list = createNewNode(int_v);
        else insertAtHead(&list, createNewNode(int_v));
    }

    return list;
}

void getKeys(json_t *json) {
    size_t index;
    json_t *value;
    int count = 0;
    char **keyList;

    json_array_foreach(json, index, value) {
        count++;
    }

    printf("count: %d\n", count);
}