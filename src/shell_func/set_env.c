#include "ush.h"

void set_env(t_ush *ush, t_process *process)
{
    for(size_t index = 0; process->args[index]; ++index) {
        int position;
        char *variable, *value;

        if((position = mx_get_char_index(process->args[index], '=')) >= 0) {
            variable = mx_strndup(process->args[index], position);
            value = &process->args[index][position + 1];
            
            if(get_env_value(ush, variable))
                set_shell_variable(ush, variable, value);
            else
                create_shell_variable(ush, variable, value, 0);
            
            mx_strdel(&variable);
        }
    }

    process->status = 0;
}
