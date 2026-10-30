#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");

    if (!in || !out) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    size_t n;

    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, n, out) != n) {
            perror("fwrite");
            return EXIT_FAILURE;
        }
    }

    if (ferror(in)) {
        perror("fread");
        return EXIT_FAILURE;
    }

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
