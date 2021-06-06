#include "ush.h"

/*
unset – The command deletes shell and environment variables.
*/
void unset_func(t_ush *ush, t_process *process)
{
    for(size_t index = 1; process->args[index]; ++index)
        unset_shell_variable(ush, process->args[index]);

    process->status = 0;
}
