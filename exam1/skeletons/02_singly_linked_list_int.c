/*
 * Singly linked list of int.
 * Insert at front: O(1); search/delete by value: O(n).
 * Ownership: list owns every node. `list_clear` frees nodes, not external data.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

int list_push_front(Node **head, int value) {
    Node *n = malloc(sizeof *n);
    if (n == NULL) return -1;
    n->value = value;
    n->next = *head;
    *head = n;
    return 0;
}

int list_push_back(Node **head, int value) {
    Node *n = malloc(sizeof *n);
    if (n == NULL) return -1;
    n->value = value;
    n->next = NULL;

    if (*head == NULL) {
        *head = n;
        return 0;
    }
    Node *cur = *head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = n;
    return 0;
}

/* Delete first matching value. Returns 1 if deleted, 0 if not found. */
int list_delete(Node **head, int value) {
    Node **link = head;                 /* pointer to the link we may change */
    while (*link != NULL) {
        if ((*link)->value == value) {
            Node *victim = *link;
            *link = victim->next;
            free(victim);
            return 1;
        }
        link = &(*link)->next;
    }
    return 0;
}

Node *list_find(Node *head, int value) {
    for (Node *cur = head; cur != NULL; cur = cur->next)
        if (cur->value == value) return cur;
    return NULL;
}

void list_print(const Node *head) {
    for (const Node *cur = head; cur != NULL; cur = cur->next)
        printf("%d -> ", cur->value);
    puts("NULL");
}

void list_clear(Node **head) {
    Node *cur = *head;
    while (cur != NULL) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

int main(void) {
    Node *head = NULL;
    if (list_push_front(&head, 2) < 0 ||
        list_push_front(&head, 1) < 0 ||
        list_push_back(&head, 3) < 0) {
        list_clear(&head);
        return 1;
    }
    list_print(head);
    list_delete(&head, 2);
    list_print(head);
    list_clear(&head);
    return 0;
}
