#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../game/gameobject.h"

typedef struct Node {
    Vertex data[3];
    size_t plane_index;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} LinkedList;

LinkedList* list_create();
void list_append(LinkedList* list, Vertex data[3], size_t plane_index);
void list_destroy(LinkedList* list);

#endif /* LINKED_LIST_H */