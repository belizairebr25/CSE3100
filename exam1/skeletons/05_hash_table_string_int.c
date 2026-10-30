/*
 * Small string -> int hash table using separate chaining.
 * Keys are copied, so caller may reuse/free its input string after put().
 * This starter does not resize. Choose a bucket count suited to the data.
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    size_t bucket_count;
} HashTable;

static char *copy_string(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static size_t hash_string(const char *s) {
    /* djb2-style simple hash */
    size_t h = 5381;
    for (; *s; s++) h = h * 33u + (unsigned char)*s;
    return h;
}

int ht_init(HashTable *ht, size_t bucket_count) {
    if (bucket_count == 0) return -1;
    ht->buckets = calloc(bucket_count, sizeof *ht->buckets);
    if (!ht->buckets) return -1;
    ht->bucket_count = bucket_count;
    return 0;
}

int ht_put(HashTable *ht, const char *key, int value) {
    size_t i = hash_string(key) % ht->bucket_count;
    for (Entry *e = ht->buckets[i]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) { e->value = value; return 0; }
    }

    Entry *e = malloc(sizeof *e);
    if (!e) return -1;
    e->key = copy_string(key);
    if (!e->key) { free(e); return -1; }
    e->value = value;
    e->next = ht->buckets[i];
    ht->buckets[i] = e;
    return 0;
}

/* Returns 0 if found, -1 if missing. */
int ht_get(const HashTable *ht, const char *key, int *out) {
    size_t i = hash_string(key) % ht->bucket_count;
    for (Entry *e = ht->buckets[i]; e; e = e->next) {
        if (strcmp(e->key, key) == 0) { *out = e->value; return 0; }
    }
    return -1;
}

int ht_remove(HashTable *ht, const char *key) {
    size_t i = hash_string(key) % ht->bucket_count;
    Entry **link = &ht->buckets[i];
    while (*link) {
        if (strcmp((*link)->key, key) == 0) {
            Entry *old = *link;
            *link = old->next;
            free(old->key);
            free(old);
            return 1;
        }
        link = &(*link)->next;
    }
    return 0;
}

void ht_free(HashTable *ht) {
    for (size_t i = 0; i < ht->bucket_count; i++) {
        Entry *e = ht->buckets[i];
        while (e) {
            Entry *next = e->next;
            free(e->key); free(e); e = next;
        }
    }
    free(ht->buckets);
    ht->buckets = NULL; ht->bucket_count = 0;
}
