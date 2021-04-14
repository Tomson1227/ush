#include "ush.h"

int fg_func(t_main *interface)
{
    mx_print_strarr(interface->command_args, " ");

    return 1;
}

int exit_func(t_main *interface)
{
    return 0;
}

int unset_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int export_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int env_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int pwd_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int whitch_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int echo_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int cd_func(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int search_command(t_main *interface)
{
    (void)interface->command_args;

    return 1;
}

int (*builtin_func[]) (t_main *) = {
    &exit_func,
    &unset_func,
    &export_func,
    &env_func,
    &cd_func,
    &pwd_func,
    &whitch_func,
    &echo_func,
    &pwd_func,
    &fg_func,
    &search_command
};

void execute(t_main *interface) // REV 1.0
{
    char *commands[] = {
        "exit",
        "unset",
        "export",
        "env",
        "cd",
        "pwd",
        "whitch",
        "echo",
        "pwd",
        "fg",
        NULL
    };

    char **command_args = NULL;

    for(uint8_t index = 0; interface->args[index];) {
        interface->command_args = get_args(interface->args, &index);
        
        if(!interface->command_args)
            return;

        uint8_t i = 0;

        for(; mx_strcmp(interface->command_args[0], commands[i]) && commands[i]; ++i);
        
        interface->status = builtin_func[i](interface);

        if(!interface->status)
            return;
    }
    
    return;
}
