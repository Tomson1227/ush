#include "ush.h"

void cd_func(t_main *interface)
{
    if(interface->func_arg.number != 2) {
        mx_print_error("cd: to many args");
        interface->status = 0;
    }

    if(chdir(interface->func_arg.value[1]) == -1) {
        strerror(errno);
        interface->status = 0;
    }

    interface->status = 1;
}
