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

    while (status)
    {
        mx_printstr("u$h>");
        line = read_line();
        args = mx_strsplit(mx_del_extra_spaces(line), ' ');
        status = execute(args);

        free(line);
        free(args);
    }
}


