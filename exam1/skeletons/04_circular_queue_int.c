/*
 * Fixed-capacity circular queue of int: first in, first out (FIFO).
 * Enqueue/dequeue are O(1). This version reports full instead of resizing.
 */
#include <stdlib.h>

typedef struct {
    int *data;
    size_t capacity;
    size_t head;       /* index of next item to remove */
    size_t size;
} IntQueue;

int queue_init(IntQueue *q, size_t capacity) {
    if (capacity == 0) return -1;
    q->data = malloc(capacity * sizeof *q->data);
    if (!q->data) return -1;
    q->capacity = capacity; q->head = 0; q->size = 0;
    return 0;
}

int queue_enqueue(IntQueue *q, int value) {
    if (q->size == q->capacity) return -1;
    size_t tail = (q->head + q->size) % q->capacity;
    q->data[tail] = value;
    q->size++;
    return 0;
}

int queue_dequeue(IntQueue *q, int *out) {
    if (q->size == 0) return -1;
    *out = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return 0;
}

void queue_free(IntQueue *q) {
    free(q->data);
    q->data = NULL; q->capacity = q->head = q->size = 0;
}
