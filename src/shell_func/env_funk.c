#include "ush.h"

void env_func(t_ush *ush, t_process *process)
{
    ush->local_status = process->status = 0;
}
