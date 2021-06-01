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

    if(argc == 1) {
        if(chdir("/home/") == -1) {
            strerror(errno);
            process->status = 1;
        }
    }
    else {
        if(chdir(process->args[1]) == -1) {
            strerror(errno);
            process->status = 0;
        }
    }

    set_prompt(ush);
    process->status = 0;
}
