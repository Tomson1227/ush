#include "ush.h"

void cd_func(t_main *interface)
{
    if(interface->func_arg.number != 2)
        mx_print_error("cd to many ")

    if(chdir(interface->func_arg.value[1]) == -1) {
        strerror(errno);
    }

    interface->status = 1;
}
