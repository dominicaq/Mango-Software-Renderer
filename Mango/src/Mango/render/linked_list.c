#include "linked_list.h"
#include <stdlib.h>

LinkedList *list_create() {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->size = 0;
    return list;
}

void list_append(LinkedList *list, Vertex data[3], size_t index) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return;
    }

    for (int i = 0; i < 3; ++i) {
        newNode->data[i] = data[i];
    }
    newNode->next = NULL;
    newNode->plane_index = index;

    // If list is empty, new node becomes head
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        // Traverse to the end of the list
        Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Append the new node to the end
        current->next = newNode;
    }
    list->size++;
}

void list_destroy(LinkedList *list) {
    if (list == NULL) {
        return;
    }

    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
