/*
 * Dynamic array of int.
 * Typical use: store a changing number of items with O(1) amortized append.
 * Ownership: the array owns its `data` allocation; call array_free exactly once.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntArray;

int array_init(IntArray *a) {
    a->data = NULL;
    a->size = 0;
    a->capacity = 0;
    return 0;
}

int array_append(IntArray *a, int value) {
    if (a->size == a->capacity) {
        size_t new_capacity = (a->capacity == 0) ? 4 : a->capacity * 2;
        int *tmp = realloc(a->data, new_capacity * sizeof *tmp);
        if (tmp == NULL) return -1;
        a->data = tmp;
        a->capacity = new_capacity;
    }
    a->data[a->size++] = value;
    return 0;
}

int array_get(const IntArray *a, size_t index, int *out) {
    if (index >= a->size) return -1;
    *out = a->data[index];
    return 0;
}

int array_set(IntArray *a, size_t index, int value) {
    if (index >= a->size) return -1;
    a->data[index] = value;
    return 0;
}

/* Remove element at index; later elements shift left. */
int array_remove(IntArray *a, size_t index, int *removed) {
    if (index >= a->size) return -1;
    if (removed != NULL) *removed = a->data[index];
    for (size_t i = index; i + 1 < a->size; i++)
        a->data[i] = a->data[i + 1];
    a->size--;
    return 0;
}

void array_free(IntArray *a) {
    free(a->data);
    a->data = NULL;
    a->size = a->capacity = 0;
}

int main(void) {
    IntArray a;
    array_init(&a);
    if (array_append(&a, 10) < 0 || array_append(&a, 20) < 0) {
        array_free(&a);
        return 1;
    }
    int value;
    if (array_get(&a, 1, &value) == 0) printf("%d\n", value);
    array_free(&a);
    return 0;
}
