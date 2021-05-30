#include "ush.h"

void exit_func(t_ush *ush, t_process *process)
{
    if(process->args[1])
        ush->status = mx_atoi(process->args[1]);
    else    
        ush->status = ush->local_status;
}
