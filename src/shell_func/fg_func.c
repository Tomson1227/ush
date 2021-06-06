#include "ush.h"

void fg_func(t_ush *ush, t_process *process)
{
    print_arg(process->args);
    process->status = 0;
}
