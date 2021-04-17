#include "ush.h"

void get_func_arg(t_main *interface, uint8_t *index)
{
    uint8_t count_args;

    //if(interface->triggers.pipe != false)
        
    for(count_args = 0; interface->line_arg.value[(*index) + count_args]; ++count_args) {
        if(!mx_strcmp(interface->line_arg.value[(*index) + count_args], "|")) {
            interface->triggers.pipe = true;
            break;
        }
        else if(!mx_strcmp(interface->line_arg.value[(*index) + count_args], ";"))
            break;
    }

    calloc_args(&interface->func_arg, count_args + 1);

    for(uint8_t i = 0; i < count_args; ++i) {
        write_arg(&interface->func_arg, interface->line_arg.value[(*index)++]);
    }

    // for(uint8_t i = 0; i < count_args;) {
    //     interface->func_arg.value[i++] = interface->line_arg.value[(*index)++];
    //     interface->func_arg.value[i] = NULL;
    // }
}
