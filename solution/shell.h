#ifndef SHELL_H
# define SHELL_H

# include "libtci.h"
# include "libtciutil.h"

typedef struct s_shell
{
    char    **envp;
    int     last_status;
    int     running;
}   t_shell;

/* builtins.c */
int     is_builtin(char const *name);
int     run_builtin(t_shell *sh, char **argv);

/* exec.c */
int     exec_simple(t_shell *sh, char **argv);

#endif
