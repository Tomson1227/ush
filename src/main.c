#include "ush.h"

int main(int argn, char *argv[])
{
    t_main *interface = NULL;
    init_main_struct(&interface);

    while (interface->status) {
        mx_printstr(interface->ush.ush_name);
        read_line(interface);
        execute(interface);
        clean_args_struct(&interface->line_arg);
    }
    
    return 0;
}
