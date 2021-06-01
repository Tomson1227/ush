#include "ush.h"

/*
unset – The command deletes shell and environment variables.
*/
void unset_func(t_ush *ush, t_process *process)
{
    process->status = 0;
}
