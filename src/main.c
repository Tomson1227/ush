#include "ush.h"

t_main *init_main_struct(void)
{
    t_main *interface = (t_main *) calloc(sizeof(t_main), 1);
    
    if(!interface)  
        mx_print_error("allocation fail\n");

    interface->line = NULL;
    interface->args = NULL;
    interface->status = 1;
    interface->command_args = NULL;
    interface->ush.ush_name = mx_strdup("u$h>");

    return interface;
}

int main(int argn, char *argv[])
{
    t_main *interface = init_main_struct();
    
    while (interface->status)
    {
        mx_printstr(interface->ush.ush_name);
        interface->line = read_line();
        interface->args = mx_strsplit(mx_del_extra_spaces(interface->line), ' ');
        execute(interface);

        free(interface->line);
        free(interface->args);
    }
    
    return 0;
}

