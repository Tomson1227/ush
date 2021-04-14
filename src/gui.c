#include "ush.h"

void get_func_arg(t_main *interface, uint8_t *index)
{
    uint8_t count_args;

    for(count_args = 0; interface->line_arg.value[(*index) + count_args]; ++count_args) {
        if(!mx_strcmp(interface->line_arg.value[(*index) + count_args], ";"))
            break;
    }
    
    interface->func_arg.value = (char **) calloc(count_args + 1, sizeof(char *));
    
    if(!interface->func_arg.value)
        mx_print_error("allocation fail");
        
    for(uint8_t i = 0; i < count_args;) {
        interface->func_arg.value[i++] = interface->line_arg.value[(*index)++];
        interface->func_arg.value[i] = NULL;
    }

    interface->func_arg.number = count_args;
}
