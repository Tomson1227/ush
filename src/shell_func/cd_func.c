#include "ush.h"

/*
-s ?
−P Don’t follow symbolic links. In other words, when this option is specified, and you try to navigate to a symlink that points to a directory, cd will change into the directory.
- change back to the previous working directory
~ change the working directory to home
*/
void cd_func(t_ush *ush, t_process *process)
{
    uint32_t argc = count_args(process->args);
    uint32_t options = get_opt(process->args, "sP");
    uint8_t opt_s = READ_OPT(options, 1);
    uint8_t opt_P = READ_OPT(options, 2);
    char *path = getenv("PWD");

    if(argc == 1) {
        set_shell_variable(ush, "OLDPWD", path);
        path = getenv("HOME");
        set_shell_variable(ush, "PWD", path);
        chdir(path);
    }
    else if(!mx_strcmp(process->args[1], "-")) {
        chdir(getenv("OLDPWD"));
        set_shell_variable(ush, "OLDPWD", path);
        path = get_pwd();
        set_shell_variable(ush, "PWD", path);
        mx_strdel(&path);
    }
    else {
        if(chdir(process->args[1]) == -1) {
            strerror(errno);
            process->status = 1;
            return;
        }

        set_shell_variable(ush, "OLDPWD", path);
        path = get_pwd();

        set_shell_variable(ush, "PWD", path);
        mx_strdel(&path);
    }

    set_prompt(ush);
    process->status = 0;
}
