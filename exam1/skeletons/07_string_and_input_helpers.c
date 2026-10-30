/*
 * Common string/input patterns.
 * getline() is POSIX (available on Linux). It allocates/resizes the line buffer.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Read one full line. Caller owns *line and should free it. */
int read_line(FILE *in, char **line, size_t *capacity) {
    ssize_t n = getline(line, capacity, in);
    if (n < 0) return -1;                 /* EOF or error */
    if (n > 0 && (*line)[n - 1] == '\n') (*line)[n - 1] = '\0';
    return (int)strlen(*line);
}

/* Trim leading/trailing whitespace in-place; returns pointer inside buffer. */
char *trim_whitespace(char *s) {
    while (isspace((unsigned char)*s)) s++;
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) s[--n] = '\0';
    return s;
}

/* Example: split mutable string on whitespace with strtok. */
void show_tokens(char *line) {
    for (char *tok = strtok(line, " \t\r\n"); tok; tok = strtok(NULL, " \t\r\n"))
        printf("[%s]\n", tok);
}
