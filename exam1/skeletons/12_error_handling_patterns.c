/*
 * Copy/paste patterns: allocation, cleanup labels, and robust writes.
 * These are snippets; they are not one standalone program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/* realloc pattern: preserve old pointer if allocation fails. */
int grow_buffer(char **buffer, size_t new_size) {
    char *tmp = realloc(*buffer, new_size);
    if (tmp == NULL) return -1;
    *buffer = tmp;
    return 0;
}

/* write() may write fewer bytes than requested; loop until all are written. */
int write_all(int fd, const void *buf, size_t len) {
    const char *p = buf;
    while (len > 0) {
        ssize_t n = write(fd, p, len);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) return -1;
        p += n;
        len -= (size_t)n;
    }
    return 0;
}

/* Cleanup-label pattern for functions with multiple resources. */
int example_cleanup(void) {
    int result = -1;
    FILE *fp = NULL;
    char *buffer = NULL;

    fp = fopen("input.txt", "r");
    if (!fp) goto cleanup;

    buffer = malloc(1024);
    if (!buffer) goto cleanup;

    /* Do work here; set result = 0 on success. */
    result = 0;

cleanup:
    free(buffer);                         /* free(NULL) is safe */
    if (fp) fclose(fp);
    return result;
}
