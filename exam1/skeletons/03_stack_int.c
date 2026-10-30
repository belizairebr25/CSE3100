/* Stack of int: last in, first out (LIFO). */
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntStack;

int stack_init(IntStack *s) {
    s->data = NULL; s->size = 0; s->capacity = 0;
    return 0;
}

int stack_push(IntStack *s, int value) {
    if (s->size == s->capacity) {
        size_t cap = s->capacity ? s->capacity * 2 : 8;
        int *tmp = realloc(s->data, cap * sizeof *tmp);
        if (!tmp) return -1;
        s->data = tmp; s->capacity = cap;
    }
    s->data[s->size++] = value;
    return 0;
}

/* Returns 0 on success, -1 if empty. */
int stack_pop(IntStack *s, int *out) {
    if (s->size == 0) return -1;
    *out = s->data[--s->size];
    return 0;
}

int stack_peek(const IntStack *s, int *out) {
    if (s->size == 0) return -1;
    *out = s->data[s->size - 1];
    return 0;
}

void stack_free(IntStack *s) {
    free(s->data);
    s->data = NULL; s->size = s->capacity = 0;
}
