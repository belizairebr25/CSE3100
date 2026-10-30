#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Read an entire file into a dynamically growing byte array.
 *
 * data_out: receives the allocated buffer (caller must free it)
 * size_out: receives the number of bytes read
 *
 * The buffer gets one extra '\0' byte for convenience when the file is text.
 * size_out does NOT include that terminator. Embedded NUL bytes are allowed;
 * use size_out rather than strlen() to determine the actual file length.
 */
static int read_file_to_array(const char *path,
                              unsigned char **data_out,
                              size_t *size_out) {
    int fd = -1;
    unsigned char *data = NULL;
    size_t size = 0;
    size_t capacity = 4096;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    data = malloc(capacity + 1); /* +1 for optional text terminator */
    if (data == NULL) {
        perror("malloc");
        close(fd);
        return -1;
    }

    for (;;) {
        /* Keep one byte available for the final '\0'. */
        if (size == capacity) {
            if (capacity > SIZE_MAX / 2 - 1) {
                fprintf(stderr, "file is too large to fit in memory\n");
                free(data);
                close(fd);
                return -1;
            }

            size_t new_capacity = capacity * 2;
            unsigned char *tmp = realloc(data, new_capacity + 1);
            if (tmp == NULL) {
                perror("realloc");
                free(data);
                close(fd);
                return -1;
            }
            data = tmp;
            capacity = new_capacity;
        }

        ssize_t n = read(fd, data + size, capacity - size);
        if (n > 0) {
            size += (size_t)n;
        } else if (n == 0) {
            break; /* end of file */
        } else if (errno == EINTR) {
            continue; /* interrupted; retry */
        } else {
            perror("read");
            free(data);
            close(fd);
            return -1;
        }
    }

    data[size] = '\0';

    if (close(fd) == -1) {
        perror("close");
        free(data);
        return -1;
    }

    *data_out = data;
    *size_out = size;
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 2;
    }

    unsigned char *contents = NULL;
    size_t length = 0;

    if (read_file_to_array(argv[1], &contents, &length) == -1) {
        return 1;
    }

    /* Replace this with the work your assignment requires. */
    printf("Read %zu bytes\n", length);
    /* Example for text only: printf("%s", (char *)contents); */

    free(contents);
    return 0;
}
