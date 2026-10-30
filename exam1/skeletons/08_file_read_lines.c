#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

/* Example template: process a file one line at a time. */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 2;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) { perror("fopen"); return 1; }

    char *line = NULL;
    size_t cap = 0;
    ssize_t nread;
    size_t line_number = 0;

    while ((nread = getline(&line, &cap, fp)) != -1) {
        line_number++;
        /* Replace this with assignment-specific processing. */
        printf("%zu: %s", line_number, line);
        (void)nread;
    }

    if (ferror(fp)) perror("getline");
    free(line);
    if (fclose(fp) == EOF) { perror("fclose"); return 1; }
    return 0;
}
