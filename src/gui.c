#include "ush.h"

void copi_arg(t_main *interface, uint8_t copi_till, uint8_t *index);

void get_func_arg(t_main *interface, uint8_t *index)
{
    uint8_t count_args;     

    for(count_args = 0; interface->line_arg.value[(*index) + count_args]; ++count_args) {
        if(!mx_strcmp(interface->line_arg.value[(*index) + count_args], "|")) {
            interface->triggers.pipe = true;
            break;
        }
        else if(!mx_strcmp(interface->line_arg.value[(*index) + count_args], ";"))
            break;
    }
   
    interface->func_arg.value = (char **) calloc(count_args + interface->result.number + 1, sizeof(char *));
    if(!interface->func_arg.value)
        mx_print_error("allocation fail");


    /* копируем данные */
    copi_arg(interface, count_args, index);


    interface->func_arg.number = count_args;
}


//   echo lskdfh ldskfj | echo


void copi_arg(t_main *interface, uint8_t copi_till, uint8_t *index) {
    uint8_t i = 0;
    for(; i < copi_till;) { 
        interface->func_arg.value[i++] = interface->line_arg.value[(*index)++]; 
        interface->func_arg.value[i] = NULL;
    }
    if(interface->triggers.pipe) {
        if(interface->line_arg.value[*index])
            (*index)++;
        for(uint8_t y = 0; i < copi_till + interface->result.number;) { 
            interface->func_arg.value[i] = interface->result.value[i - copi_till];
            interface->func_arg.value[++i] = NULL;
        }
    }
}
