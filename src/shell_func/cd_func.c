#include "ush.h"

void cd_func(t_main *interface)
{
    if(!interface->func_arg.value[1])
        interface->func_arg.value[1] = mx_strdup("/home/");

    // ~ -> /home/{USER}

    if(chdir(interface->func_arg.value[1]) == -1) {
        strerror(errno);
        interface->status = 0;
    }

    interface->status = 1;
}
