/*
 * Basic pthread template. Shared state is protected by a mutex.
 * Compile with: gcc -Wall -Wextra -pthread file.c -o file
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *items;
    size_t count;
    pthread_mutex_t lock;
} Shared;

void *worker(void *arg) {
    Shared *s = arg;
    pthread_mutex_lock(&s->lock);
    /* Critical section: access shared state here. */
    s->count++;
    pthread_mutex_unlock(&s->lock);
    return NULL;
}

int main(void) {
    Shared s = { .items = NULL, .count = 0 };
    if (pthread_mutex_init(&s.lock, NULL) != 0) return 1;

    enum { NTHREADS = 4 };
    pthread_t threads[NTHREADS];
    int created = 0;

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_create(&threads[i], NULL, worker, &s) != 0) {
            fprintf(stderr, "pthread_create failed\n");
            break;
        }
        created++;
    }
    for (int i = 0; i < created; i++) pthread_join(threads[i], NULL);

    printf("count = %zu\n", s.count);
    pthread_mutex_destroy(&s.lock);
    return created == NTHREADS ? 0 : 1;
}
