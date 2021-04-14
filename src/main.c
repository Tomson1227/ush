#include "ush.h"

static inline void init_args_struct(t_args *args)
{
    args->number = 0;
    args->value = NULL;
}

static inline t_main *init_main_struct(void)
{
    t_main *interface = (t_main *) calloc(sizeof(t_main), 1);
    
    if(!interface)  
        mx_print_error("allocation fail\n");

    interface->status = 1;
    init_args_struct(&interface->func_arg);
    init_args_struct(&interface->line_arg);
    interface->ush.ush_name = mx_strdup("u$h>");

    return interface;
}

static void split_line(t_main *interface, char *line)
{
    char *clean_line = mx_del_extra_spaces(line);
    interface->line_arg.number = mx_count_words(clean_line, ' ');
    interface->line_arg.value = mx_strsplit(clean_line, ' ');
}

int main(int argn, char *argv[])
{
    t_main *interface = init_main_struct();
    
    while (interface->status)
    {
        mx_printstr(interface->ush.ush_name);
        char *line = read_line();
        split_line(interface, line);
        execute(interface);

        mx_strdel(&line);
    }
    
    return 0;
}
