#include "ush.h"

/*
export – The command sets environment variables.
*/
void export_func(t_ush *ush, t_process *process)
{
    for(size_t index = 1; process->args[index]; ++index) {
        int position;
        char *variable;

        if((position = mx_get_char_index(process->args[index], '=')) >= 0) {
            variable = mx_strndup(process->args[index], position);
            create_shell_variable(ush, variable, &process->args[index][position + 1], 1);
            mx_strdel(&variable);
        }
        else if((variable = get_shell_variable(ush, process->args[index]))) {
            export_shell_variable(ush, process->args[index]);
            mx_strdel(&variable);
        }
    }

    process->status = 0;
}
