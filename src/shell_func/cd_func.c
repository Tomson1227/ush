#include "ush.h"

void cd_func(t_ush *ush, t_process *process)
{
    if(!process->args[1]) {
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
