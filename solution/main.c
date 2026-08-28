#include <unistd.h>
#include <stdlib.h>
#include "shell.h"

static void trim_newline(char *line)
{
    size_t  len;

    len = tci_strlen(line);
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
}

static char **naive_split(char const *line)
{
    /* BUG (fixed later): whitespace-only splitting has no idea what a
     * quote is. echo "hi there" becomes THREE words -- echo, "hi, and
     * there" -- not the two the reader typed. */
    return (tciu_split(line, ' '));
}

static void free_argv(char **argv)
{
    int i;

    i = 0;
    while (argv[i]) {
        free(argv[i]);
        i++;
    }
    free(argv);
}

static int count_words(char **argv)
{
    int i;

    i = 0;
    while (argv[i])
        i++;
    return (i);
}

static void run_line(t_shell *sh, char const *line)
{
    char    **argv;

    argv = naive_split(line);
    if (!argv)
        return;
    if (count_words(argv) == 0) {
        free_argv(argv);
        return;
    }
    if (is_builtin(argv[0]))
        sh->last_status = run_builtin(sh, argv);
    else
        sh->last_status = exec_simple(sh, argv);
    free_argv(argv);
}

int main(void)
{
    t_shell sh;
    char    *line;
    int     interactive;

    sh.envp = NULL;
    sh.last_status = 0;
    sh.running = 1;
    interactive = isatty(STDIN_FILENO);
    while (sh.running) {
        if (interactive)
            tci_printf("$ ");
        line = tci_getline(STDIN_FILENO);
        if (!line) {
            if (interactive)
                tci_printf("exit\n");
            break;
        }
        trim_newline(line);
        run_line(&sh, line);
        free(line);
    }
    return (sh.last_status);
}
