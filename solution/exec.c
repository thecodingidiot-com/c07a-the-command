#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "shell.h"

int exec_simple(t_shell *sh, char **argv)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid < 0) {
        tci_printf("fork: failed\n");
        return (1);
    }
    if (pid == 0) {
        execvp(argv[0], argv);
        fprintf(stderr, "%s: command not found\n", argv[0]);
        _exit(127);
    }
    waitpid(pid, &status, 0);
    (void)sh;
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}
