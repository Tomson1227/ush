#include "ush.h"

char **get_args(char **args, uint8_t *index)
{
    uint8_t count_args;
    char **select_args = NULL;

    for(count_args = 0; args[(*index) + count_args]; ++count_args) {
        if(!mx_strcmp(args[(*index) + count_args], ";"))
            break;
    }
    
    select_args = (char **) malloc((count_args + 1)* sizeof(char *));
    
    if(!select_args)
        mx_print_error("allocation fail");
        
    for(uint8_t i = 0; i < count_args;) {
        select_args[i++] = args[(*index)++];
        select_args[i] = NULL;
    }

    return select_args;
}

void start_gui(void)
{
    char *line;
    char **args;
    int status = 1;
    t_main *main_struct = new_struct_t_main();

    while (status)
    {
        mx_printstr(main_struct->ush.ush_name);
        line = read_line();
        args = mx_strsplit(mx_del_extra_spaces(line), ' ');
        status = execute(args);

        free(line);
        free(args);
    }
}

t_main *new_struct_t_main(void) {
    t_main *new_struct = (t_main*)malloc(sizeof(t_main));

    new_struct->ush.ush_name = mx_strdup("u$h>");
    return new_struct;
}
