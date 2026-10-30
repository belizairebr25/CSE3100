/*
 * Minimal pipeline: command 1 stdout -> command 2 stdin.
 * Example argv arrays:
 *   char *a[] = {"cat", "input.txt", NULL};
 *   char *b[] = {"wc", "-l", NULL};
 *   return run_pipeline(a, b);
 *
 * The parent closes BOTH pipe ends and waits for BOTH children.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static int wait_for(pid_t pid, int *status) {
    while (waitpid(pid, status, 0) < 0) {
        if (errno == EINTR) continue;
        return -1;
    }
    return 0;
}

int run_pipeline(char *const left[], char *const right[]) {
    int fd[2];
    if (pipe(fd) < 0) { perror("pipe"); return -1; }

    pid_t p1 = fork();
    if (p1 < 0) { perror("fork"); close(fd[0]); close(fd[1]); return -1; }
    if (p1 == 0) {
        if (dup2(fd[1], STDOUT_FILENO) < 0) { perror("dup2"); _exit(126); }
        close(fd[0]); close(fd[1]);
        execvp(left[0], left);
        perror("execvp left");
        _exit(127);
    }

    pid_t p2 = fork();
    if (p2 < 0) {
        perror("fork");
        close(fd[0]); close(fd[1]);
        (void)wait_for(p1, &(int){0});
        return -1;
    }
    if (p2 == 0) {
        if (dup2(fd[0], STDIN_FILENO) < 0) { perror("dup2"); _exit(126); }
        close(fd[0]); close(fd[1]);
        execvp(right[0], right);
        perror("execvp right");
        _exit(127);
    }

    close(fd[0]); close(fd[1]);            /* critical: let reader see EOF */
    int s1, s2;
    if (wait_for(p1, &s1) < 0 || wait_for(p2, &s2) < 0) {
        perror("waitpid"); return -1;
    }
    if (WIFEXITED(s2)) return WEXITSTATUS(s2);
    if (WIFSIGNALED(s2)) return 128 + WTERMSIG(s2);
    return -1;
}
